
#ifndef GCODEVIEW_H
#define GCODEVIEW_H

#include <QOpenGLWidget>
#include <QOpenGLFramebufferObject>
//#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QCloseEvent>
#include <QImage>
#include <QOffscreenSurface>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFramebufferObject>

#include <QWaitCondition>

#include "gcodeProcessor/GCodeProcessor.hpp"
#include "scene3d.h"
#include "glview/renderscene.h"
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <QThread>
#include <QProcess>
#include "common/controlInterface/progressdialog.h"
#include "common/utilities/tlogger.h"
#include "common/controlInterface/messageDialog.h"
#define ENABLE_SPLITTED_VERTEX_BUFFER true
#define ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS true
#define ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE true
#define HAS_GLSAFE true
#define FULL_SCREEN true

typedef Eigen::Transform<float, 3, Eigen::Affine, Eigen::DontAlign> Transform3f;
using namespace Anker;


#ifdef HAS_GLSAFE
extern void glAssertRecentCallImpl(const char* file_name, unsigned int line, const char* function_name);
inline void glAssertRecentCall() { glAssertRecentCallImpl(__FILE__, __LINE__, __FUNCTION__); }
#define glsafe(cmd) do { cmd; glAssertRecentCallImpl(__FILE__, __LINE__, __FUNCTION__); } while (false)
#define glcheck() do { glAssertRecentCallImpl(__FILE__, __LINE__, __FUNCTION__); } while (false)
#else // HAS_GLSAFE
inline void glAssertRecentCall() { }
#define glsafe(cmd) cmd
#define glcheck()
#endif // HAS_GLSAFE
class GcodeViewer;

class paramsStack
{
    GcodeViewer * handls;
    ViewType btype ;
    QVector3D eye_b;
    QVector3D up_b;
    QVector3D front_b ;
    unsigned int pushd_flag ;
    std::array<unsigned int, 2> layer_z_range_saved ;
    std::vector<std::array<float, 3>>  Extrusion_Role_Colors_saved;
    double v_angle;
    bool travelVisable;
    int w;
    int h;
public:
    paramsStack(GcodeViewer *hdls);
    ~paramsStack();
};

class boolLock
{
    bool* b_v;
public:
     boolLock(bool &v);
     ~boolLock();
};

class IgnoreEvent :public QObject
{
public:
    IgnoreEvent(QObject* obj)
    {
        if(this->thread() != obj->thread()){
            qDebug() << __FUNCTION__ << this->thread() << "!=" << obj->thread();
            this->moveToThread(obj->thread());  //  fix @2023-05-08 by ChunLian
        }
        m_obj = obj;
        m_obj->installEventFilter(this);
        //AkUtil::TDebug("-------------filtter install--------------");
    }
    ~IgnoreEvent()
    {
        m_obj->removeEventFilter(this);
        //QTime dieTime= QTime::currentTime().addMSecs(100);
        //while (QTime::currentTime() < dieTime)
        //   QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        //AkUtil::TDebug("-------------filtter remove--------------");
    }
    bool eventFilter(QObject *obj, QEvent *event)
    {
        //AkUtil::TDebug("-------------filtter--------------");
        event->ignore();
        return true;
        return QObject::eventFilter(obj, event);
    }
private:
    QObject* m_obj;
};


class GcodeViewer : public QOpenGLWidget, protected /*QOpenGLExtraFunctions*/QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
protected:
    virtual void initializeGL() Q_DECL_OVERRIDE;
    virtual void paintGL() Q_DECL_OVERRIDE;
    virtual void resizeGL(int w, int h) Q_DECL_OVERRIDE;

    //void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
    //void keyReleaseEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent* event)  Q_DECL_OVERRIDE;
    void focusOutEvent ( QFocusEvent * event ) Q_DECL_OVERRIDE;
    void focusInEvent ( QFocusEvent * event ) Q_DECL_OVERRIDE;

public:

    void getSceneParam();
    void setView(const ViewType& type);
    void setPerformance(bool setValue);
    void offerContext();

    void setSceneParams(const SceneParam& param);
    void writeToGPUBUffer(CHMeshShowObjPtr mesh, RenderMeshPtr renderMesh);
    void writeToGPUBUffer(CHMeshShowObjPtr mesh, QOpenGLBuffer& outvbo, QOpenGLBuffer& outnorbo,
        QOpenGLVertexArrayObject& outvao, GLuint& outibo, int& fnum, bool normalFlip = false);
    void writeToGPUBUffer(CHCurveShowObjPtr curvebody, RenderCurvePtr rendercurve);
    void paintMeshVbo(QOpenGLShaderProgram& shaderProgram, CHMeshShowObjPtr mesh, RenderMeshPtr rm, QMatrix4x4& pvMatrix);
    void paintCurveVbo(QOpenGLShaderProgram& shaderProgram, CHCurveShowObjPtr curve, RenderCurvePtr rc, QMatrix4x4& pvMatrix);
    void paintPointVbo(QOpenGLShaderProgram& shaderProgram, CHPointShowObjPtr point, RenderPointSpherePtr rps, QMatrix4x4& pvMatrix);
    void setRoleColor(unsigned int index,std::array<float, 3> setColor);
    void setTravelColor(unsigned int index,std::array<float, 3> setColor);
    void setZlapColor(std::array<float, 3> setColor);
    bool isGcodeWithAI();
    //void showProgressDlg();

public slots:
    void exportOffRender(const QString& savePath,bool isAiMode,bool cancel);

signals:
    void setValue(int);
    void notify_refresh_render_path(bool,bool);

private:

    
    
        
    std::unordered_map<std::string, int> m_shader;
    QOpenGLShaderProgram shaderProgram1;//why can't push QOpenGLShaderProgram in std::vector
    QOpenGLShaderProgram shaderProgram2;
    QOpenGLShaderProgram shaderProgram_or; //off_render shader
    QOpenGLShaderProgram shaderProgram3;//3D View
    QOpenGLShaderProgram shaderProgram_tr;//t render with a gs
    QOpenGLShaderProgram shaderProgram_point;
    //QOpenGLShaderProgram shaderProgram_old; // old 110 render
    Scene3D* m_scene3d  = nullptr;
    RenderDataPtr m_renderData = nullptr;
    SceneParam m_sceneParam;
    QImage offImage;
#if ENABLE_SPLITTED_VERTEX_BUFFER
    using IBufferType = unsigned short;
#endif // ENABLE_SPLITTED_VERTEX_BUFFER
    using Color = std::array<float, 3>;
    using VertexBuffer = std::vector<float>;
#if ENABLE_SPLITTED_VERTEX_BUFFER
    using MultiVertexBuffer = std::vector<VertexBuffer>;
    using IndexBuffer = std::vector<IBufferType>;
#else
    using IndexBuffer = std::vector<unsigned int>;
#endif // ENABLE_SPLITTED_VERTEX_BUFFER
    using MultiIndexBuffer = std::vector<IndexBuffer>;
    std::vector<Color> Extrusion_Role_Colors;
    static const std::vector<Color> Range_Colors;
    std::vector<Color> Travel_Colors;
    Color Zlap_Colors;
    static const std::vector<Color> Options_Colors;
    static const Color              Wipe_Color;
    enum class EOptionsColors : unsigned char
    {
        Retractions,
        Unretractions,
        ToolChanges,
        ColorChanges,
        PausePrints,
        CustomGCodes
    };
    struct VBuffer
    {
        enum class EFormat : unsigned char
        {
            // vertex format: 3 floats -> position.x|position.y|position.z
            Position ,
            // vertex format: 4 floats -> position.x|position.y|position.z|normal.x
            PositionNormal1,
            // vertex format: 6 floats -> position.x|position.y|position.z|normal.x|normal.y|normal.z
            PositionNormal3,

            PositionMoveStruct
        };

        EFormat format{ EFormat::Position };
#if ENABLE_SPLITTED_VERTEX_BUFFER
        // vbos id
        std::vector<unsigned int> vbos;
        // sizes of the buffers, in bytes, used in export to obj
        std::vector<size_t> sizes;
#else
        // vbo id
        unsigned int id{ 0 };
#endif // ENABLE_SPLITTED_VERTEX_BUFFER
        // count of vertices, updated after data are sent to gpu
        size_t count{ 0 };

        size_t data_size_bytes() const { return count * vertex_size_bytes(); }
#if ENABLE_SPLITTED_VERTEX_BUFFER
        // We set 65536 as max count of vertices inside a vertex buffer to allow
        // to use unsigned short in place of unsigned int for indices in the index buffer, to save memory
        size_t max_size_bytes() const { return 65536 * vertex_size_bytes(); }
#endif // ENABLE_SPLITTED_VERTEX_BUFFER

        size_t vertex_size_floats() const { return position_size_floats() + normal_size_floats() + owner_size_floats(); }
        size_t vertex_size_bytes() const { return vertex_size_floats() * sizeof(float); }
        size_t owner_size_floats() const {
            switch (format)
            {
            case EFormat::Position:
            case EFormat::PositionNormal3: { return 0; }
            case EFormat::PositionNormal1: { return 0; }
            case EFormat::PositionMoveStruct: {return 0;}
            default: { return 0; }
            }
        } //dir
        size_t position_offset_floats() const { return 0; }
        size_t position_offset_size() const { return position_offset_floats() * sizeof(float); }
        size_t position_size_floats() const {
            switch (format)
            {
            case EFormat::Position:
            case EFormat::PositionNormal3: { return 3; }
            case EFormat::PositionNormal1: { return 4; }
            case EFormat::PositionMoveStruct: {return 20;}
            default: { return 0; }
            }
        }
        size_t position_size_bytes() const { return position_size_floats() * sizeof(float); }

        size_t normal_offset_floats() const {
            switch (format)
            {
            case EFormat::Position:
            case EFormat::PositionNormal1: { return 0; }
            case EFormat::PositionNormal3: { return 3; }
            default: { return 0; }
            }
        }
        size_t normal_offset_size() const { return normal_offset_floats() * sizeof(float); }
        size_t normal_size_floats() const {
            switch (format)
            {
            default:
            case EFormat::Position:
            case EFormat::PositionNormal1: { return 0; }
            case EFormat::PositionNormal3: { return 3; }
            }
        }
        size_t normal_size_bytes() const { return normal_size_floats() * sizeof(float); }
        bool is_ankermove_type() const {return (format == EFormat::PositionMoveStruct);}
        void reset();
    };
    // ibo buffer containing indices data (for lines/triangles) used to render a specific toolpath type
public:
    friend class paramsStack;
    struct IBuffer
    {
#if ENABLE_SPLITTED_VERTEX_BUFFER
        // id of the associated vertex buffer
        unsigned int vbo{ 0 };
        // ibo id
        unsigned int ibo{ 0 };
#else
        // ibo id
        unsigned int id{ 0 };
#endif // ENABLE_SPLITTED_VERTEX_BUFFER
        // count of indices, updated after data are sent to gpu
        size_t count{ 0 };

        void reset();
    };

    // Used to identify different toolpath sub-types inside a IBuffer
    struct Path
    {
        struct Endpoint
        {
            // index of the buffer in the multibuffer vector
            // the buffer type may change:
            // it is the vertex buffer while extracting vertices data,
            // the index buffer while extracting indices data
            unsigned int b_id{ 0 };
            // index into the buffer
            size_t i_id{ 0 };
            // move id
            size_t s_id{ 0 };
            Vec3f position{ Vec3f::Zero() };
        };

#if ENABLE_SPLITTED_VERTEX_BUFFER
        struct Sub_Path
        {
            Endpoint first;
            Endpoint last;

            bool contains(size_t s_id) const {
                return first.s_id <= s_id && s_id <= last.s_id;
            }
        };
#endif // ENABLE_SPLITTED_VERTEX_BUFFER

        EMoveType type{ EMoveType::Noop };
        ExtrusionRole role{ erNone };
#if !ENABLE_SPLITTED_VERTEX_BUFFER
        Endpoint first;
        Endpoint last;
#endif // !ENABLE_SPLITTED_VERTEX_BUFFER
        float delta_extruder{ 0.0f };
        float height{ 0.0f };
        float width{ 0.0f };
        float feedrate{ 0.0f };
        float fan_speed{ 0.0f };
        float volumetric_rate{ 0.0f };
        unsigned char extruder_id{ 0 };
        unsigned char cp_color_id{ 0 };
#if ENABLE_SPLITTED_VERTEX_BUFFER
        std::vector<Sub_Path> sub_paths;
#endif // ENABLE_SPLITTED_VERTEX_BUFFER
        float layer_time{ 0.0f };
        float elapsed_time{ 0.0f };
        float extruder_temp{ 0.0f };

        bool matches(const GCodeProcessor::MoveVertex& move) const;
#if ENABLE_SPLITTED_VERTEX_BUFFER
        size_t vertices_count() const {
            return sub_paths.empty() ? 0 : sub_paths.back().last.s_id - sub_paths.front().first.s_id + 1;
        }
        bool contains(size_t s_id) const {
            return sub_paths.empty() ? false : sub_paths.front().first.s_id <= s_id && s_id <= sub_paths.back().last.s_id;
        }
        int get_id_of_sub_path_containing(size_t s_id) const {
            if (sub_paths.empty())
                return -1;
            else {
                for (int i = 0; i < static_cast<int>(sub_paths.size()); ++i) {
                    if (sub_paths[i].contains(s_id))
                        return i;
                }
                return -1;
            }
        }
        void add_sub_path(const GCodeProcessor::MoveVertex& move, unsigned int b_id, size_t i_id, size_t s_id) {
            Endpoint endpoint = { b_id, i_id, s_id, move.position };
            sub_paths.push_back({ endpoint , endpoint });
        }
#else
        size_t vertices_count() const { return last.s_id - first.s_id + 1; }
        bool contains(size_t id) const { return first.s_id <= id && id <= last.s_id; }
#endif // ENABLE_SPLITTED_VERTEX_BUFFER
    };

    struct RenderPath
    {
#if ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
        // Index of the parent tbuffer
        unsigned char               tbuffer_id;
#endif // ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
        // Render path property
        Color                       color;
        //is render cap ;add by friva.xie  for change cap  when render cap
        float                        render_cap;
        // Index of the buffer in TBuffer::indices
        unsigned int                ibuffer_id;
        // Render path content
        // Index of the path in TBuffer::paths
        unsigned int                path_id;
        std::vector<unsigned int>   sizes;
        std::vector<size_t>         offsets; // use size_t because we need an unsigned integer whose size matches pointer's size (used in the call glMultiDrawElements())

#if ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
        bool contains(size_t offset) const {
            for (size_t i = 0; i < offsets.size(); ++i) {
                if (offsets[i] <= offset && offset <= offsets[i] + static_cast<size_t>(sizes[i] * sizeof(IBufferType)))
                    return true;
            }
            return false;
        }
#endif // ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
    };

    struct RenderPathPropertyLower {
        bool operator() (const RenderPath& l, const RenderPath& r) const {
#if ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
            if (l.tbuffer_id < r.tbuffer_id)
                return true;
#endif // ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
            for (int i = 0; i < 3; ++i) {
                if (l.color[i] < r.color[i])
                    return true;
                else if (l.color[i] > r.color[i])
                    return false;
            }
            return l.ibuffer_id < r.ibuffer_id;
        }
    };

    struct RenderPathPropertyEqual {
        bool operator() (const RenderPath& l, const RenderPath& r) const {
#if ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
            return l.tbuffer_id == r.tbuffer_id && l.ibuffer_id == r.ibuffer_id && l.color == r.color;
#else
            return l.color == r.color && l.ibuffer_id == r.ibuffer_id;
#endif // ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
        }
    };

    struct TBuffer
    {
        enum class ERenderPrimitiveType : unsigned char
        {
            Point,
            Line,
            Triangle,
            AnkerMove
        };

        unsigned int t_vao ;

        ERenderPrimitiveType render_primitive_type;
        VBuffer vertices;
        std::vector<IBuffer> indices;

        std::string shader;
        std::vector<Path> paths;
        // std::set seems to perform significantly better, at least on Windows.
//        std::unordered_set<RenderPath, RenderPathPropertyHash, RenderPathPropertyEqual> render_paths;
        std::set<RenderPath, RenderPathPropertyLower> render_paths;
        bool visible{ false };

        void reset();

        // b_id index of buffer contained in this->indices
        // i_id index of first index contained in this->indices[b_id]
        // s_id index of first vertex contained in this->vertices
        void add_path(const GCodeProcessor::MoveVertex& move, unsigned int b_id, size_t i_id, size_t s_id);

#if ENABLE_SPLITTED_VERTEX_BUFFER
        unsigned int max_vertices_per_segment() const {
            switch (render_primitive_type)
            {
            case ERenderPrimitiveType::Point: { return 1; }
            case ERenderPrimitiveType::Line: { return 2; }
            case ERenderPrimitiveType::Triangle: { return 8; }
            case ERenderPrimitiveType::AnkerMove: { return 2;}
            default: { return 0; }
            }
        }

        size_t max_vertices_per_segment_size_floats() const { return vertices.vertex_size_floats() * static_cast<size_t>(max_vertices_per_segment()); }
        size_t max_vertices_per_segment_size_bytes() const { return max_vertices_per_segment_size_floats() * sizeof(float); }
#endif // ENABLE_SPLITTED_VERTEX_BUFFER
        unsigned int indices_per_segment() const {
            switch (render_primitive_type)
            {
            case ERenderPrimitiveType::Point: { return 1; }
            case ERenderPrimitiveType::Line: { return 2; }
            case ERenderPrimitiveType::AnkerMove: {return 2;}
#if ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
            case ERenderPrimitiveType::Triangle: { return 30; } // 3 indices x 10 triangles
#else
            case ERenderPrimitiveType::Triangle: { return 42; } // 3 indices x 14 triangles
#endif // ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
            default: { return 0; }
            }
        }
#if ENABLE_SPLITTED_VERTEX_BUFFER
        size_t indices_per_segment_size_bytes() const { return static_cast<size_t>(indices_per_segment() * sizeof(IBufferType)); }
#endif // ENABLE_SPLITTED_VERTEX_BUFFER
#if ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
        unsigned int max_indices_per_segment() const {
            switch (render_primitive_type)
            {
            case ERenderPrimitiveType::Point: { return 1; }
            case ERenderPrimitiveType::Line: { return 2; }
            case ERenderPrimitiveType::AnkerMove: { return 2; }
            case ERenderPrimitiveType::Triangle: { return 36; } // 3 indices x 12 triangles
            default: { return 0; }
            }
        }
        size_t max_indices_per_segment_size_bytes() const { return max_indices_per_segment() * sizeof(IBufferType); }
#endif // ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS

#if ENABLE_SPLITTED_VERTEX_BUFFER
        bool has_data() const {
            return !vertices.vbos.empty() && vertices.vbos.front() != 0 && !indices.empty() && indices.front().ibo != 0;
        }
#else
        unsigned int start_segment_vertex_offset() const { return 0; }
        unsigned int end_segment_vertex_offset() const {
            switch (render_primitive_type)
            {
            case ERenderPrimitiveType::Point: { return 0; }
            case ERenderPrimitiveType::Line: { return 1; }
            case ERenderPrimitiveType::Triangle: { return 36; } // 1st vertex of 13th triangle
            default: { return 0; }
            }
        }

        bool has_data() const { return vertices.id != 0 && !indices.empty() && indices.front().id != 0; }
#endif // ENABLE_SPLITTED_VERTEX_BUFFER
    };

    class Layers
    {
    public:
        struct Endpoints
        {
            size_t first{ 0 };
            size_t last{ 0 };

#if ENABLE_SPLITTED_VERTEX_BUFFER
            bool operator == (const Endpoints& other) const {
                return first == other.first && last == other.last;
            }
#endif // ENABLE_SPLITTED_VERTEX_BUFFER
        };

    private:
        std::vector<double> m_zs;
        std::vector<std::array<double, erCount>> m_filament_cost; //add for filament
        std::vector<Endpoints> m_endpoints;

    public:
        void append(double z, Endpoints endpoints, std::array<double, erCount> filamentCost) {
            m_zs.emplace_back(z);
            m_endpoints.emplace_back(endpoints);
            m_filament_cost.emplace_back(filamentCost);
        }

        void reset() {
            m_zs = std::vector<double>();
            m_endpoints = std::vector<Endpoints>();
            m_filament_cost = std::vector<std::array<double, erCount>>();
        }

        size_t size() const { return m_zs.size(); }
        bool empty() const { return m_zs.empty(); }
        const std::vector<double>& get_zs() const { return m_zs; }
        const std::vector<Endpoints>& get_endpoints() const { return m_endpoints; }
        std::vector<Endpoints>& get_endpoints() { return m_endpoints; }
        std::vector<std::array<double, erCount>>& get_filamentcost() { return m_filament_cost; }
        const std::vector<std::array<double, erCount>>& get_filamentcost() const { return m_filament_cost; }
        double get_z_at(unsigned int id) const { return (id < m_zs.size()) ? m_zs[id] : 0.0; }
        Endpoints get_endpoints_at(unsigned int id) const { return (id < m_endpoints.size()) ? m_endpoints[id] : Endpoints(); }

#if ENABLE_SPLITTED_VERTEX_BUFFER
        bool operator != (const Layers& other) const {
            if (m_zs != other.m_zs)
                return true;
            if (!(m_endpoints == other.m_endpoints))
                return true;

            return false;
        }
#endif // ENABLE_SPLITTED_VERTEX_BUFFER
    };

    struct Extrusions
    {
        struct Range
        {
            float min;
            float max;
            unsigned int count;

            Range() { reset(); }

            void update_from(const float value) {
                if (value != max && value != min)
                    ++count;
                min = std::min(min, value);
                max = std::max(max, value);
            }
            void reset() { min = FLT_MAX; max = -FLT_MAX; count = 0; }

            float step_size(bool log = false) const;
            Color get_color_at(float value, bool log = false) const;
        };

        struct Ranges
        {
            // Color mapping by layer height.
            Range height;
            // Color mapping by extrusion width.
            Range width;
            // Color mapping by feedrate.
            Range feedrate;
            // Color mapping by fan speed.
            Range fan_speed;
            // Color mapping by volumetric extrusion rate.
            Range volumetric_rate;
            // Color mapping by fan speed.
            Range extruder_temp;
            // Color mapping by layer time.
            Range layer_duration;
            // Color mapping by time.
            Range elapsed_time;



            void reset() {
                height.reset();
                width.reset();
                feedrate.reset();
                fan_speed.reset();
                //volumetric_rate.reset();
                extruder_temp.reset();
                layer_duration.reset();
                elapsed_time.reset();
            }
        };

        unsigned int role_visibility_flags{ 0 };
        Ranges ranges;

        void reset_role_visibility_flags() {
            role_visibility_flags = 0;
            for (unsigned int i = 0; i < erCount; ++i) {
                role_visibility_flags |= 1 << i;
            }
        }

        void reset_ranges() { ranges.reset(); }
    };

    //bounding box struct;
    struct boundingBox{
        Vec3f min{ Vec3f::Zero()};
        Vec3f max{ Vec3f::Zero()};
        bool isDefine{false};
    };


    struct GLModelInitializationData
    {
        std::vector<Vec3f> positions;
        std::vector<Vec3f> normals;
        std::vector<Vec3i32> triangles;
    };

    class GLModel: protected /*QOpenGLExtraFunctions*/QOpenGLFunctions_3_3_Core
    {
        unsigned int m_vbo_id{ 0 };
        unsigned int m_ibo_id{ 0 };

        unsigned int m_vao_id{ 0 };

        size_t m_indices_count{ 0 };
        boundingBox model_bounding_box;
    public:
        virtual ~GLModel() { reset(); }
        void reset();
        void init_from(const GLModelInitializationData& data);
        const boundingBox& get_bounding_box() const { return model_bounding_box; }
        void render(QOpenGLShaderProgram& shaderProgram);
    private:
        void send_to_gpu(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
    };

    struct SequentialView
    {
        //TODO:
        class Marker: protected /*QOpenGLExtraFunctions*/QOpenGLFunctions_3_3_Core
        {

            Vec3f m_world_position;
            Transform3f m_world_transform;
            float m_z_offset{ 0.5f };
            std::array<float, 4> m_color{ 1.0f, 1.0f, 1.0f, 0.5f };
            bool m_visible{ true };

        public:
            void init();

            const boundingBox& get_bounding_box() const { return m_model.get_bounding_box(); }
            void set_world_position(const Vec3f& position);
            void set_color(const std::array<float, 4>& color) { m_color = color; }

            bool is_visible() const { return m_visible; }
            void set_visible(bool visible) { m_visible = visible; }

            void render(QOpenGLShaderProgram& shaderProgramold,QMatrix4x4 vm, QMatrix4x4 pm);
            GLModel m_model;
        };

        struct Endpoints
        {
            size_t first{ 0 };
            size_t last{ 0 };
        };

        bool skip_invisible_moves{ false };
        Endpoints endpoints;
        Endpoints current;
        Endpoints last_current;
        Vec3f current_position{ Vec3f::Zero() };
        Marker marker;
    };

    enum  colorType : unsigned int
    {
        LineType = 0,
        Speed,
        Trapezoid,
        Flow,
        Count
    };

    enum class EViewType : unsigned char
    {
        FeatureType,
        Height,
        Width,
        Feedrate,
        FanSpeed,
        LayerTime,
        LayerTimeLog,
        Chronology,
        VolumetricRate,
        Tool,
        Filament,
        ColorPrint,
        ExtruderTemp,
        Count
    };

#if ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
    // used to render the toolpath caps of the current sequential range
    // (i.e. when sliding on the horizontal slider)
    struct SequentialRangeCap
    {
        TBuffer* buffer{ nullptr };
        unsigned int ibo{ 0 };
        unsigned int vbo{ 0 };

        unsigned int vao{ 0 };

        Color color;

        ~SequentialRangeCap();
        bool is_renderable() const { return buffer != nullptr; }
        void reset();
        size_t indices_count() const { return 6; }
    };
#endif // ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS

#if ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
    std::array<SequentialRangeCap, 2> m_sequential_range_caps;
#endif // ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS

    size_t m_moves_count{ 0 };

private:
    EViewType m_view_type{ EViewType::FeatureType };
    colorType m_colortype{ colorType::LineType };
    std::vector<Color> m_tool_colors;
    std::vector<Color> m_filament_colors;
    PrintEstimatedTimeStatistics m_time_statistics;
    PrintEstimatedTimeStatistics::ETimeMode m_time_estimate_mode{ PrintEstimatedTimeStatistics::ETimeMode::Normal };

    bool is_visible(ExtrusionRole role) const {
        return role < erCount && (m_extrusions.role_visibility_flags& (1 << role)) != 0;
    }
    bool is_visible(const Path& path) const { return is_visible(path.role); }
    bool m_save_pic;
    bool m_isNotOffScreen = true;
    QOpenGLFramebufferObject* m_fbo = nullptr;
    std::string m_gcode_path;
    QString m_gcode_name;//only name  example : /a/b.gcode  = b
    ProgressDialog* mPdlg = nullptr;

    //void refresh_render_paths_with_makeCurrent(bool keep_sequential_current_first, bool keep_sequential_current_last);
    std::vector<unsigned int>  genLoopVec();
    QString  genSavePath(double layer);
    bool matchGcodeName();
    static GLModelInitializationData stilized_arrow(int resolution, float tip_radius, float tip_height, float stem_radius, float stem_height);
    bool isTRender = false;
    QString originalGcodePath;
    QString originalStlName;
    QString originalGcodeName;
    QString akpicSave;
    
    QMutex  m_scene3d_init_mutex;
    QWaitCondition m_scene3d_inited;
public:
    GcodeViewer(QWidget* partent);

    ~GcodeViewer();
    void offPaint();
    void offRenderSingle();
    void load_toolpaths(const GCodeProcessor::Result& gcode_result);
    void reset();
    void deleteGpuData(); // aden
    void resetGpu(bool needModel = false);
    // recalculate ranges in dependence of what is visible and sets tool/print colors
    
    void refresh(const GCodeProcessor::Result& gcode_result, const std::vector<std::string>& str_tool_colors,bool runInOtherThread = false);
    void set_toolpath_move_type_visible(EMoveType type, bool visible);
    void refresh_render_paths(bool keep_sequential_current_first, bool keep_sequential_current_last);
    bool setGcodePath(const std::string& gcodePath);
    void setOriginalGcodePath(const QString& gcodePath);
    void setOriginalStlName(const QString& oStlName);
    void setOriginalGcodeName(const QString& oGcodeName);
    std::string getGcodePath();

    void update_sequential_view_current(unsigned int& first, unsigned int& last);
    void render_toolpaths(QMatrix4x4 , QMatrix4x4);
    void horizonsliderMove(unsigned int value1, unsigned int value2);
    void verticalsliderMove(int value1, int value2);
    void set_save_pic(bool is_save);
    void off_render(QString savePath,bool isAiMode = true);
    void off_render_single(const QImage&);
    void off_render_single_org(const QImage&);
    QImage render_singe_iamge();
    void set_look_at(std::array<float, 3> position, std::array<float, 3> lookat, std::array<float, 3> lookup,bool isaiview = true);

    std::vector<float> get_roi_info(int w, int h);
    std::vector<TBuffer> m_buffers{ static_cast<size_t>(EMoveType::Extrude) };
    Layers m_layers;
    std::array<unsigned int, 2> m_layers_z_range;
    std::vector<unsigned char> m_extruder_ids;
    Extrusions m_extrusions;
    std::vector<Anker::ExtrusionRole> m_roles;
    SequentialView m_sequential_view;
    std::vector<float> times;
    std::vector<float> percents;
    //
    GCodeProcessor::Result gcode_result;
    QImage grabFramebufferInternal(QOpenGLFramebufferObject* fbo);

    
    void setRoleVisible(ExtrusionRole role, bool isVisible);
    void getTimeAndPercent();
    void setColorType(colorType vt){this->m_colortype = vt;}
    void setIsTRender(bool tv){
        this->isTRender = tv;
    }
    boundingBox m_boundingBox;
    std::string get_gcode_path(){
        return m_gcode_path;
    }
    QString sendOriginalStlNamedFile(bool isNeedReplace);
    QString sendAcodeToPrintCtr();
    bool printMode = false;
    bool innerMode = false;
    void clearGcodeSource();
    void loadGcode();
    int waitForFileClose(QString filePath,qint64 fileSize);
    QString filterFileName(QString pathName);
    bool postGcodeHead(const QString &line,QString& reLine);
    bool waitForSceneInit();
    bool isHighPerformance = true;
    QOffscreenSurface* m_offscreenSurface = nullptr;
    bool m_orceShader1 = false;
    QOpenGLContext* cnt = nullptr; //offerender context
};



#endif // !GCODEVIEW_H

