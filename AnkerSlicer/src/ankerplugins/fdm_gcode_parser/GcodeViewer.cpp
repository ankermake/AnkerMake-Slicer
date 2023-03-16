#include "GcodeViewer.h"
#include "process_ai_picture.h"
#include "post_process_gcode.h"
#include "ui_viewwidget.h"
#include <QCoreApplication>
#include <glm/gtc/type_ptr.hpp> // value_ptr
#include <private/qzipwriter_p.h>
#include <QOpenGLFramebufferObject>
#include <QStandardPaths>
#include <QRegularExpression>
#include <QDir>
#include <QBuffer>
#include <QElapsedTimer>
#include <QMessageBox>
#include <QTimer>
#include <QFile>
#include <QtMath>

#include <QThreadPool>

#ifdef HAS_GLSAFE
void glAssertRecentCallImpl(const char* file_name, unsigned int line, const char* function_name)
{
    GLenum err = glGetError();
    if (err == GL_NO_ERROR)
        return;
    const char* sErr = 0;
    switch (err) {
    case GL_INVALID_ENUM:       sErr = "Invalid Enum";      break;
    case GL_INVALID_VALUE:      sErr = "Invalid Value";     break;
        // be aware that GL_INVALID_OPERATION is generated if glGetError is executed between the execution of glBegin and the corresponding execution of glEnd
    case GL_INVALID_OPERATION:  sErr = "Invalid Operation"; break;
    case GL_STACK_OVERFLOW:     sErr = "Stack Overflow";    break;
    case GL_STACK_UNDERFLOW:    sErr = "Stack Underflow";   break;
    case GL_OUT_OF_MEMORY:      sErr = "Out Of Memory";     break;
    default:                    sErr = "Unknown";           break;
    }
    //TODO: BOOST_LOG_TRIVIAL(error) << "OpenGL error in " << file_name << ":" << line << ", function " << function_name << "() : " << (int)err << " - " << sErr;
    assert(false);
}
#endif // HAS_GLSAFE
static EMoveType buffer_type(unsigned char id) {
    return static_cast<EMoveType>(static_cast<unsigned char>(EMoveType::Retract) + id);
}

static unsigned char buffer_id(EMoveType type) {
    return static_cast<unsigned char>(type) - static_cast<unsigned char>(EMoveType::Retract);
}

template <typename T, typename Number>
static inline T lerp(const T& a, const T& b, Number t)
{
    assert((t >= Number(-EPSILON)) && (t <= Number(1) + Number(EPSILON)));
    return (Number(1) - t) * a + t * b;
}

inline int hex_digit_to_int(const char c)
{
    return
            (c >= '0' && c <= '9') ? int(c - '0') :
                                     (c >= 'A' && c <= 'F') ? int(c - 'A') + 10 :
                                                              (c >= 'a' && c <= 'f') ? int(c - 'a') + 10 : -1;
}

static float m_round_to_nearest(float value, unsigned int decimals)
{
    float res = 0.0f;
    if (decimals == 0)
        res = std::round(value);
    else {
        char buf[64];
        sprintf(buf, "%.*g", decimals, value);
        res = std::stof(buf);
    }
    return res;
}

static std::array<float, 3> decode_color(const std::string& color) {
    static const float INV_255 = 1.0f / 255.0f;

    std::array<float, 3> ret = { 0.0f, 0.0f, 0.0f };
    const char* c = color.data() + 1;
    if (color.size() == 7 && color.front() == '#') {
        for (size_t j = 0; j < 3; ++j) {
            int digit1 = hex_digit_to_int(*c++);
            int digit2 = hex_digit_to_int(*c++);
            if (digit1 == -1 || digit2 == -1)
                break;

            ret[j] = float(digit1 * 16 + digit2) * INV_255;
        }
    }
    return ret;
}

static std::vector<std::array<float, 3>> decode_colors(const std::vector<std::string>& colors) {
    std::vector<std::array<float, 3>> output(colors.size(), { 0.0f, 0.0f, 0.0f });
    for (size_t i = 0; i < colors.size(); ++i) {
        output[i] = decode_color(colors[i]);
    }
    return output;
}


const std::vector<GcodeViewer::Color> GcodeViewer::Options_Colors{ {
        { 0.803f, 0.135f, 0.839f },   // Retractions
        { 0.287f, 0.679f, 0.810f },   // Unretractions
        { 0.758f, 0.744f, 0.389f },   // ToolChanges
        { 0.856f, 0.582f, 0.546f },   // ColorChanges
        { 0.322f, 0.942f, 0.512f },   // PausePrints
        { 0.886f, 0.825f, 0.262f }    // CustomGCodes
                                                                   } };

const std::vector<GcodeViewer::Color> GcodeViewer::Range_Colors{ {
        { 0.043f, 0.173f, 0.478f }, // bluish
        { 0.075f, 0.349f, 0.522f },
        { 0.110f, 0.533f, 0.569f },
        { 0.016f, 0.839f, 0.059f },
        { 0.667f, 0.949f, 0.000f },
        { 0.988f, 0.975f, 0.012f },
        { 0.961f, 0.808f, 0.039f },
        { 0.890f, 0.533f, 0.125f },
        { 0.820f, 0.408f, 0.188f },
        { 0.761f, 0.322f, 0.235f },
        { 0.581f, 0.149f, 0.087f }  // reddish
                                                                 } };

const GcodeViewer::Color GcodeViewer::Wipe_Color = { 1.0f, 1.0f, 0.0f };

paramsStack::paramsStack(GcodeViewer *hdls)
{
    this->handls = hdls;
    this->btype = hdls->m_scene3d->getVIew();
    this->eye_b = hdls->m_scene3d->getEye();
    this->up_b = hdls->m_scene3d->getUp();
    this->front_b = hdls->m_scene3d->getFront();
    this->pushd_flag = hdls->m_extrusions.role_visibility_flags;
    hdls->m_isNotOffScreen = false;
    this->layer_z_range_saved = hdls->m_layers_z_range;
    this->w = hdls->m_scene3d->getW();
    this->h = hdls->m_scene3d->getH();
    this->v_angle = hdls->m_scene3d->getVerticalAngle();
    this->Extrusion_Role_Colors_saved = hdls->Extrusion_Role_Colors;
    size_t id = static_cast<size_t>(buffer_id(EMoveType::Travel));
    this->travelVisable = hdls->m_buffers[id].visible;
}

paramsStack::~paramsStack()
{
    this->handls->m_layers_z_range = layer_z_range_saved;
    this->handls->m_extrusions.role_visibility_flags = pushd_flag;
    this->handls->m_isNotOffScreen = true;
    this->handls->m_scene3d->setEye(eye_b.x(), eye_b.y(), eye_b.z());
    this->handls->m_scene3d->setUp(up_b.x(), up_b.y(), up_b.z());
    this->handls->m_scene3d->setFront(front_b.x(), front_b.y(), front_b.z());
    this->handls->m_scene3d->setVerticalAngle(30.0);
    this->handls->m_scene3d->setView(btype);
    this->handls->m_scene3d->resizeWidget(w,h);
    this->handls->m_scene3d->setVerticalAngle(v_angle);
    this->handls->Extrusion_Role_Colors = std::move(Extrusion_Role_Colors_saved);
    size_t id = static_cast<size_t>(buffer_id(EMoveType::Travel));
    this->handls->m_buffers[id].visible = this->travelVisable ;
}

boolLock::boolLock(bool &v)
{
    this->b_v = &v;
    (*b_v) = !(*b_v);
}

boolLock::~boolLock()
{
    (*b_v) = !(*b_v);
}


#ifdef  FORCE_ARM
GcodeViewer::GcodeViewer(QWidget* parent) :QOpenGLWidget(parent), m_scene3d(nullptr),isTRender(false)
  #else
GcodeViewer::GcodeViewer(QWidget* parent) :QOpenGLWidget(parent), m_scene3d(nullptr),isTRender(true)
  #endif
{
    // QSurfaceFormat surfaceFormat;
    
    //#ifdef __APPLE__
    // surfaceFormat.setVersion(3, 3);
    // surfaceFormat.setProfile(QSurfaceFormat::CoreProfile);
    //#endif
    // setFormat(surfaceFormat);

    
    connect(this,&GcodeViewer::notify_refresh_render_path,this,&GcodeViewer::refresh_render_paths,Qt::QueuedConnection);

    this->setFocusPolicy(Qt::ClickFocus);
    //load Extrusion colors
    {
        this->Extrusion_Role_Colors = {
            { 0.75f, 0.75f, 0.75f },   // erNone
            { 0.3137f, 0.3882f, 0.36078f },//{ 1.00f, 0.90f, 0.30f },   // erPerimeter
            { 1.00f, 0.5647f, 0.3215f },//{ 1.00f, 0.49f, 0.22f },   // erExternalPerimeter
            { 0.75f, 0.75f, 0.75f },//{ 0.12f, 0.12f, 1.00f },   // erOverhangPerimeter
            { 0.5411f, 0.298f, 0.8470f },//{ 0.69f, 0.19f, 0.16f },   // erInternalInfill
            { 0.38f, 0.8274f, 0.4902f },//{ 0.59f, 0.33f, 0.80f },   // erSolidInfill
            { 0.94f, 0.25f, 0.25f },   // erTopSolidInfill
            { 1.00f, 0.55f, 0.41f },   // erIroning
            { 0.30f, 0.50f, 0.73f },   // erBridgeInfill
            { 0.00f, 1.00f, 0.40f },   // erThinWall
            { 1.00f, 1.00f, 1.00f },   // erGapFill
            { 1.00f, 1.00f, 1.00f },
            { 0.2784f, 0.5607f, 1.0f },//{ 0.00f, 0.53f, 0.43f },   // erSkirt
            { 0.9765f, 0.8274f, 0.337f },   // erSupportMaterial 249 211 86
            { 0.9765f, 0.8274f, 0.337f },//{ 0.00f, 0.50f, 0.00f },   // erSupportMaterialInterface
            { 0.2784f, 0.5607f, 1.0f },//{ 0.70f, 0.89f, 0.67f },   // erWipeTower
            { 0.70f, 0.70f, 0.70f },   // erMilling
            { 0.37f, 0.82f, 0.58f },   // erCustom
            { 0.00f, 0.00f, 0.00f }    // erMixed
        };

        this->Travel_Colors = {
            {0.3568f, 0.8117f, 0.8117f }, // Move { 0.219f, 0.282f, 0.609f }
            { 0.112f, 0.422f, 0.103f }, // Extrude{ 0.112f, 0.422f, 0.103f }
            { 0.505f, 0.064f, 0.028f }  // Retract{ 0.505f, 0.064f, 0.028f }
        };
    }

    // initializes non OpenGL data of TBuffers
    // OpenGL data are initialized into render().init_gl_data()
    for (size_t i = 0; i < m_buffers.size(); ++i) {
        TBuffer& buffer = m_buffers[i];
        switch (buffer_type(i))
        {
        default: { break; }
        case EMoveType::Tool_change:
        case EMoveType::Color_change:
        case EMoveType::Pause_Print:
        case EMoveType::Custom_GCode:
        case EMoveType::Retract:
        case EMoveType::Unretract: {
            buffer.render_primitive_type = TBuffer::ERenderPrimitiveType::Point;
            buffer.vertices.format = VBuffer::EFormat::Position;
            break;
        }
        case EMoveType::Wipe:
        case EMoveType::Extrude: {
            if(isTRender){
                buffer.render_primitive_type = TBuffer::ERenderPrimitiveType::AnkerMove;
                buffer.vertices.format = VBuffer::EFormat::PositionMoveStruct;
            }
            else{
                buffer.render_primitive_type = TBuffer::ERenderPrimitiveType::Triangle;
                buffer.vertices.format = VBuffer::EFormat::PositionNormal3;
            }
            break;
        }
        case EMoveType::Travel: {
            buffer.render_primitive_type = TBuffer::ERenderPrimitiveType::Line;
            buffer.vertices.format = VBuffer::EFormat::PositionNormal1;
            break;
        }
        case EMoveType::Zlap: {
            buffer.render_primitive_type = TBuffer::ERenderPrimitiveType::Point;
            buffer.vertices.format = VBuffer::EFormat::Position;
            break;
        }
        }
    }
    set_toolpath_move_type_visible(EMoveType::Extrude, true);
    m_save_pic = false;
    m_extrusions.reset_role_visibility_flags();
    m_extrusions.reset_ranges();
}


GcodeViewer::~GcodeViewer()
{
    AkUtil::TFunction("");
    if(QFile::exists(std::move(QString::fromStdString(this->m_gcode_path))))
    {
        QFile::remove(QString::fromStdString(this->m_gcode_path));
    }
    if(QFile::exists(std::move(this->akpicSave)))
    {
        QFile::remove(this->akpicSave);
    }
    this->resetGpu();
    if(m_offscreenSurface)
    {
        m_offscreenSurface->destroy();
        delete m_offscreenSurface;
        m_offscreenSurface = nullptr;
        if(cnt)
        {
            cnt->destroyed();
            delete cnt;
            cnt = nullptr;
        }
    };
    reset();
    if(m_scene3d)
    {
        delete m_scene3d;
        m_scene3d = nullptr;
    }
}
void GcodeViewer::load_toolpaths(const GCodeProcessor::Result& gcode_result)
{
    // max index buffer size, in bytes
    static const size_t IBUFFER_THRESHOLD_BYTES = 64  * 1024 * 1024;

    // format data into the buffers to be rendered as points
    auto add_vertices_as_point = [](const GCodeProcessor::MoveVertex& curr, VertexBuffer& vertices) {
        vertices.push_back(curr.position[0]);
        vertices.push_back(curr.position[1]);
        vertices.push_back(curr.position[2]);
    };
    auto add_indices_as_point = [](const GCodeProcessor::MoveVertex& curr, TBuffer& buffer,
            unsigned int ibuffer_id, IndexBuffer& indices, size_t move_id) {
        buffer.add_path(curr, ibuffer_id, indices.size(), move_id);
        indices.push_back(static_cast<IBufferType>(indices.size()));
    };

    auto add_vertices_as_ankermove = [](const GCodeProcessor::MoveVertex& prev, const GCodeProcessor::MoveVertex& curr, VertexBuffer& vertices) {
        // x component of the normal to the current segment (the normal is parallel to the XY plane)
        //float normal_x = (curr.position - prev.position).normalized()[1];

        auto add_vertex = [&vertices](const GCodeProcessor::MoveVertex& vertex) {
            // add position
            vertices.push_back(vertex.position[0]);
            vertices.push_back(vertex.position[1]);
            vertices.push_back(vertex.position[2]);

            vertices.push_back(vertex.delta_extruder);
            vertices.push_back(vertex.feedrate);
            vertices.push_back(vertex.width);
            vertices.push_back(vertex.height);

            vertices.push_back(vertex.mm3_per_mm);
            vertices.push_back(vertex.fan_speed);
            vertices.push_back(vertex.layer_duration);
            vertices.push_back(vertex.time);

            vertices.push_back(vertex.temperature);
            vertices.push_back(vertex.motor_process.accelerate_until);
            vertices.push_back(vertex.motor_process.cruise);

            vertices.push_back(vertex.motor_process.decelerate_after);
            vertices.push_back(vertex.motor_process.entry);
            vertices.push_back(vertex.motor_process.exit);

            vertices.push_back(float(vertex.type));
            vertices.push_back(float(vertex.extrusion_role));
            vertices.push_back(float(vertex.g1_line_id));
        };

        // add previous vertex
        add_vertex(prev);
        // add current vertex
        add_vertex(curr);
    };
    auto add_indices_as_ankermove = [](const GCodeProcessor::MoveVertex& prev, const GCodeProcessor::MoveVertex& curr, TBuffer& buffer,
            unsigned int ibuffer_id, IndexBuffer& indices, size_t move_id) {
        if (prev.type != curr.type || !buffer.paths.back().matches(curr)) {
            // add starting index
            indices.push_back(static_cast<unsigned int>(indices.size()));
            buffer.add_path(curr, ibuffer_id, indices.size() - 1, move_id - 1);
            buffer.paths.back().sub_paths.front().first.position = prev.position;
        }

        Path& last_path = buffer.paths.back();
        if (last_path.sub_paths.front().first.i_id != last_path.sub_paths.back().last.i_id) {
            // add previous index
            indices.push_back(static_cast<unsigned int>(indices.size()));
        }

        // add current index
        indices.push_back(static_cast<unsigned int>(indices.size()));
        last_path.sub_paths.back().last = { ibuffer_id, indices.size() - 1, move_id, curr.position };
    };

    // format data into the buffers to be rendered as lines
    auto add_vertices_as_line = [](const GCodeProcessor::MoveVertex& prev, const GCodeProcessor::MoveVertex& curr, VertexBuffer& vertices) {
        // x component of the normal to the current segment (the normal is parallel to the XY plane)
        float normal_x = (curr.position - prev.position).normalized()[1];

        auto add_vertex = [&vertices, normal_x](const GCodeProcessor::MoveVertex& vertex) {
            // add position
            vertices.push_back(vertex.position[0]);
            vertices.push_back(vertex.position[1]);
            vertices.push_back(vertex.position[2]);
            // add normal x component
            vertices.push_back(normal_x);
        };

        // add previous vertex
        add_vertex(prev);
        // add current vertex
        add_vertex(curr);
    };
    auto add_indices_as_line = [](const GCodeProcessor::MoveVertex& prev, const GCodeProcessor::MoveVertex& curr, TBuffer& buffer,
            unsigned int ibuffer_id, IndexBuffer& indices, size_t move_id) {
        if (prev.type != curr.type || !buffer.paths.back().matches(curr)) {
            // add starting index
            indices.push_back(static_cast<unsigned int>(indices.size()));
            buffer.add_path(curr, ibuffer_id, indices.size() - 1, move_id - 1);
            buffer.paths.back().sub_paths.front().first.position = prev.position;
        }

        Path& last_path = buffer.paths.back();
        if (last_path.sub_paths.front().first.i_id != last_path.sub_paths.back().last.i_id) {
            // add previous index
            indices.push_back(static_cast<unsigned int>(indices.size()));
        }

        // add current index
        indices.push_back(static_cast<unsigned int>(indices.size()));
        last_path.sub_paths.back().last = { ibuffer_id, indices.size() - 1, move_id, curr.position };
    };

    auto add_vertices_as_solid = [](const GCodeProcessor::MoveVertex& prev, const GCodeProcessor::MoveVertex& curr, TBuffer& buffer, unsigned int vbuffer_id, VertexBuffer& vertices, size_t move_id) {
        auto store_vertex = [](VertexBuffer& vertices, const Vec3f& position, const Vec3f& normal, const Vec3f& dir) {

            // append position
            vertices.push_back(position[0]);
            vertices.push_back(position[1]);
            vertices.push_back(position[2]);
            // append normal
            vertices.push_back(normal[0]);
            vertices.push_back(normal[1]);
            vertices.push_back(normal[2]);

        };

        if (prev.type != curr.type || !buffer.paths.back().matches(curr)) {
            buffer.add_path(curr, vbuffer_id, vertices.size(), move_id - 1);
            buffer.paths.back().sub_paths.back().first.position = prev.position;
        }

        Path& last_path = buffer.paths.back();

        Vec3f dir = (curr.position - prev.position).normalized();
        Vec3f right = Vec3f(dir[1], -dir[0], 0.0f).normalized();
        Vec3f left = -right;
        Vec3f up = right.cross(dir);
        Vec3f down = -up;
        float half_width = 0.5f * last_path.width;
        float half_height = 0.5f * last_path.height;
        Vec3f prev_pos = prev.position - half_height * up;
        Vec3f curr_pos = curr.position - half_height * up;
        Vec3f d_up = half_height * up;
        Vec3f d_down = -half_height * up;
        Vec3f d_right = half_width * right;
        Vec3f d_left = -half_width * right;

        // vertices 1st endpoint
        if (last_path.vertices_count() == 1 || vertices.empty()) {
            // 1st segment or restart into a new vertex buffer
            // ===============================================
            store_vertex(vertices, prev_pos + d_up, up, dir);
            store_vertex(vertices, prev_pos + d_right, right, dir);
            store_vertex(vertices, prev_pos + d_down, down, dir);
            store_vertex(vertices, prev_pos + d_left, left, dir);
        }
        else {
            // any other segment
            // =================
            store_vertex(vertices, prev_pos + d_right, right,dir);
            store_vertex(vertices, prev_pos + d_left, left,dir);
        }

        // vertices 2nd endpoint
        store_vertex(vertices, curr_pos + d_up, up,dir);
        store_vertex(vertices, curr_pos + d_right, right,dir);
        store_vertex(vertices, curr_pos + d_down, down,dir);
        store_vertex(vertices, curr_pos + d_left, left,dir);

        last_path.sub_paths.back().last = { vbuffer_id, vertices.size(), move_id, curr.position };
    };

#if ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
    auto add_indices_as_solid = [&](const GCodeProcessor::MoveVertex& prev, const GCodeProcessor::MoveVertex& curr, const GCodeProcessor::MoveVertex* next,
            TBuffer& buffer, size_t& vbuffer_size, unsigned int ibuffer_id, IndexBuffer& indices, size_t move_id) {
#else
    auto add_indices_as_solid = [](const GCodeProcessor::MoveVertex& prev, const GCodeProcessor::MoveVertex& curr, TBuffer& buffer,
            size_t& vbuffer_size, unsigned int ibuffer_id, IndexBuffer& indices, size_t move_id) {
#endif // ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
        static Vec3f prev_dir;
        static Vec3f prev_up;
        static float sq_prev_length;
        auto store_triangle = [](IndexBuffer& indices, IBufferType i1, IBufferType i2, IBufferType i3) {
            indices.push_back(i1);
            indices.push_back(i2);
            indices.push_back(i3);
        };
        auto append_dummy_cap = [store_triangle](IndexBuffer& indices, IBufferType id) {
            store_triangle(indices, id, id, id);
            store_triangle(indices, id, id, id);
        };
#if ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
        auto convert_vertices_offset = [](size_t vbuffer_size, const std::array<int, 8>& v_offsets) {
            std::array<IBufferType, 8> ret = {
                static_cast<IBufferType>(static_cast<int>(vbuffer_size) + v_offsets[0]),
                static_cast<IBufferType>(static_cast<int>(vbuffer_size) + v_offsets[1]),
                static_cast<IBufferType>(static_cast<int>(vbuffer_size) + v_offsets[2]),
                static_cast<IBufferType>(static_cast<int>(vbuffer_size) + v_offsets[3]),
                static_cast<IBufferType>(static_cast<int>(vbuffer_size) + v_offsets[4]),
                static_cast<IBufferType>(static_cast<int>(vbuffer_size) + v_offsets[5]),
                static_cast<IBufferType>(static_cast<int>(vbuffer_size) + v_offsets[6]),
                static_cast<IBufferType>(static_cast<int>(vbuffer_size) + v_offsets[7])
            };
            return ret;
        };
        auto append_starting_cap_triangles = [&](IndexBuffer& indices, const std::array<IBufferType, 8>& v_offsets) {
            store_triangle(indices, v_offsets[0], v_offsets[2], v_offsets[1]);
            store_triangle(indices, v_offsets[0], v_offsets[3], v_offsets[2]);
        };
        auto append_stem_triangles = [&](IndexBuffer& indices, const std::array<IBufferType, 8>& v_offsets) {
            store_triangle(indices, v_offsets[0], v_offsets[1], v_offsets[4]);
            store_triangle(indices, v_offsets[1], v_offsets[5], v_offsets[4]);
            store_triangle(indices, v_offsets[1], v_offsets[2], v_offsets[5]);
            store_triangle(indices, v_offsets[2], v_offsets[6], v_offsets[5]);
            store_triangle(indices, v_offsets[2], v_offsets[3], v_offsets[6]);
            store_triangle(indices, v_offsets[3], v_offsets[7], v_offsets[6]);
            store_triangle(indices, v_offsets[3], v_offsets[0], v_offsets[7]);
            store_triangle(indices, v_offsets[0], v_offsets[4], v_offsets[7]);
        };
        auto append_ending_cap_triangles = [&](IndexBuffer& indices, const std::array<IBufferType, 8>& v_offsets) {
            store_triangle(indices, v_offsets[4], v_offsets[6], v_offsets[7]);
            store_triangle(indices, v_offsets[4], v_offsets[5], v_offsets[6]);
        };
#else
        auto append_stem_triangles = [&](IndexBuffer& indices, size_t vbuffer_size, const std::array<int, 8>& v_offsets) {
            std::array<IBufferType, 8> v_ids;
            for (size_t i = 0; i < v_ids.size(); ++i) {
                v_ids[i] = static_cast<IBufferType>(static_cast<int>(vbuffer_size) + v_offsets[i]);
            }

            // triangles starting cap
            store_triangle(indices, v_ids[0], v_ids[2], v_ids[1]);
            store_triangle(indices, v_ids[0], v_ids[3], v_ids[2]);

            // triangles sides
            store_triangle(indices, v_ids[0], v_ids[1], v_ids[4]);
            store_triangle(indices, v_ids[1], v_ids[5], v_ids[4]);
            store_triangle(indices, v_ids[1], v_ids[2], v_ids[5]);
            store_triangle(indices, v_ids[2], v_ids[6], v_ids[5]);
            store_triangle(indices, v_ids[2], v_ids[3], v_ids[6]);
            store_triangle(indices, v_ids[3], v_ids[7], v_ids[6]);
            store_triangle(indices, v_ids[3], v_ids[0], v_ids[7]);
            store_triangle(indices, v_ids[0], v_ids[4], v_ids[7]);

            // triangles ending cap
            store_triangle(indices, v_ids[4], v_ids[6], v_ids[7]);
            store_triangle(indices, v_ids[4], v_ids[5], v_ids[6]);
        };
#endif // ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS

        if (prev.type != curr.type || !buffer.paths.back().matches(curr)) {
            buffer.add_path(curr, ibuffer_id, indices.size(), move_id - 1);
            buffer.paths.back().sub_paths.back().first.position = prev.position;
        }

        Path& last_path = buffer.paths.back();

        Vec3f dir = (curr.position - prev.position).normalized();
        Vec3f right = Vec3f(dir[1], -dir[0], 0.0f).normalized();
        Vec3f up = right.cross(dir);
        float sq_length = (curr.position - prev.position).squaredNorm();

#if ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
        const std::array<IBufferType, 8> first_seg_v_offsets = convert_vertices_offset(vbuffer_size, { 0, 1, 2, 3, 4, 5, 6, 7 });
        const std::array<IBufferType, 8> non_first_seg_v_offsets = convert_vertices_offset(vbuffer_size, { -4, 0, -2, 1, 2, 3, 4, 5 });
#endif // ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS

        if (last_path.vertices_count() == 1 || vbuffer_size == 0) {
            // 1st segment or restart into a new vertex buffer
            // ===============================================
#if ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
            if (last_path.vertices_count() == 1)
                // starting cap triangles
                append_starting_cap_triangles(indices, first_seg_v_offsets);
#endif // ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
            // dummy triangles outer corner cap
            append_dummy_cap(indices, vbuffer_size);

            // stem triangles
#if ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
            append_stem_triangles(indices, first_seg_v_offsets);
#else
            append_stem_triangles(indices, vbuffer_size, { 0, 1, 2, 3, 4, 5, 6, 7 });
#endif // ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS

            vbuffer_size += 8;
        }
        else {
            // any other segment
            // =================
            float displacement = 0.0f;
            float cos_dir = prev_dir.dot(dir);
            if (cos_dir > -0.9998477f) {
                // if the angle between adjacent segments is smaller than 179 degrees
                Vec3f med_dir = (prev_dir + dir).normalized();
                float half_width = 0.5f * last_path.width;
                displacement = half_width * ::tan(::acos(std::clamp(dir.dot(med_dir), -1.0f, 1.0f)));
            }

            float sq_displacement = sqr(displacement);
            bool can_displace = displacement > 0.0f && sq_displacement < sq_prev_length&& sq_displacement < sq_length;

            bool is_right_turn = prev_up.dot(prev_dir.cross(dir)) <= 0.0f;
            // whether the angle between adjacent segments is greater than 45 degrees
            bool is_sharp = cos_dir < 0.7071068f;

            bool right_displaced = false;
            bool left_displaced = false;

            if (!is_sharp && can_displace) {
                if (is_right_turn)
                    left_displaced = true;
                else
                    right_displaced = true;
            }

            // triangles outer corner cap
            if (is_right_turn) {
                if (left_displaced)
                    // dummy triangles
                    append_dummy_cap(indices, vbuffer_size);
                else {
                    store_triangle(indices, vbuffer_size - 4, vbuffer_size + 1, vbuffer_size - 1);
                    store_triangle(indices, vbuffer_size + 1, vbuffer_size - 2, vbuffer_size - 1);
                }
            }
            else {
                if (right_displaced)
                    // dummy triangles
                    append_dummy_cap(indices, vbuffer_size);
                else {
                    store_triangle(indices, vbuffer_size - 4, vbuffer_size - 3, vbuffer_size + 0);
                    store_triangle(indices, vbuffer_size - 3, vbuffer_size - 2, vbuffer_size + 0);
                }
            }

            // stem triangles
#if ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
            append_stem_triangles(indices, non_first_seg_v_offsets);
#else
            append_stem_triangles(indices, vbuffer_size, { -4, 0, -2, 1, 2, 3, 4, 5 });
#endif // ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS

            vbuffer_size += 6;
        }

#if ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
        if (next != nullptr && (curr.type != next->type || !last_path.matches(*next)))
            // ending cap triangles
            append_ending_cap_triangles(indices, non_first_seg_v_offsets);
#endif // ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS

        last_path.sub_paths.back().last = { ibuffer_id, indices.size() - 1, move_id, curr.position };
        prev_dir = dir;
        prev_up = up;
        sq_prev_length = sq_length;
    };


    m_moves_count = gcode_result.moves.size();
    if (m_moves_count == 0)
        return;

    std::vector<MultiVertexBuffer> vertices(m_buffers.size());
    std::vector<MultiIndexBuffer> indices(m_buffers.size());
    std::vector<float> options_zs;

    auto getBoundingbox = [this](Vec3f curPos)
    {
        if (this->m_boundingBox.isDefine)
        {
            this->m_boundingBox.min[0] = this->m_boundingBox.min[0] < curPos[0] ? this->m_boundingBox.min[0] : curPos[0];
            this->m_boundingBox.min[1] = this->m_boundingBox.min[1] < curPos[1] ? this->m_boundingBox.min[1] : curPos[1];
            this->m_boundingBox.min[2] = this->m_boundingBox.min[2] < curPos[2] ? this->m_boundingBox.min[2] : curPos[2];
            this->m_boundingBox.max[0] = this->m_boundingBox.max[0] > curPos[0] ? this->m_boundingBox.max[0] : curPos[0];
            this->m_boundingBox.max[1] = this->m_boundingBox.max[1] > curPos[1] ? this->m_boundingBox.max[1] : curPos[1];
            this->m_boundingBox.max[2] = this->m_boundingBox.max[2] > curPos[2] ? this->m_boundingBox.max[2] : curPos[2];

        }
        else {
            this->m_boundingBox.min = curPos;
            this->m_boundingBox.max = curPos;
            this->m_boundingBox.isDefine = true;
        }
    };


    //--add vertice
    for (size_t i = 0; i < m_moves_count; ++i) {
        const GCodeProcessor::MoveVertex& curr = gcode_result.moves[i];
        if (curr.type == EMoveType::Extrude && curr.width != 0.0f && curr.height != 0.0f)
        {
            getBoundingbox(curr.position);
        }

        // skip first vertex
        if (i == 0)
            continue;

        const GCodeProcessor::MoveVertex& prev = gcode_result.moves[i - 1];


        unsigned char id = buffer_id(curr.type);
        TBuffer& t_buffer = m_buffers[id];
        MultiVertexBuffer& v_multibuffer = vertices[id];

        // ensure there is at least one vertex buffer
        if (v_multibuffer.empty())
            v_multibuffer.push_back(VertexBuffer());

        // if adding the vertices for the current segment exceeds the threshold size of the current vertex buffer
        // add another vertex buffer
        if (v_multibuffer.back().size() * sizeof(float) > t_buffer.vertices.max_size_bytes() - t_buffer.max_vertices_per_segment_size_bytes()) {
            v_multibuffer.push_back(VertexBuffer());
            if (t_buffer.render_primitive_type == TBuffer::ERenderPrimitiveType::Triangle) {
                Path& last_path = t_buffer.paths.back();
                if (prev.type == curr.type && last_path.matches(curr))
                    last_path.add_sub_path(prev, static_cast<unsigned int>(v_multibuffer.size()) - 1, 0, i - 1);
            }
        }

        VertexBuffer& v_buffer = v_multibuffer.back();

        switch (t_buffer.render_primitive_type)
        {
        case TBuffer::ERenderPrimitiveType::Point: { add_vertices_as_point(curr, v_buffer); break; }
        case TBuffer::ERenderPrimitiveType::Line: { add_vertices_as_line(prev, curr, v_buffer); break; }
        case TBuffer::ERenderPrimitiveType::Triangle: { add_vertices_as_solid(prev, curr, t_buffer, static_cast<unsigned int>(v_multibuffer.size()) - 1, v_buffer, i); break; }
        case TBuffer::ERenderPrimitiveType::AnkerMove:{add_vertices_as_ankermove(prev, curr, v_buffer);
            break;}
        }

        // collect options zs for later use
        if (curr.type == EMoveType::Pause_Print || curr.type == EMoveType::Custom_GCode) {
            const float* const last_z = options_zs.empty() ? nullptr : &options_zs.back();
            if (last_z == nullptr || curr.position[2] < *last_z - EPSILON || *last_z + EPSILON < curr.position[2])
                options_zs.emplace_back(curr.position[2]);
        }
    }

    // smooth toolpaths corners for the given TBuffer using triangles
    auto smooth_triangle_toolpaths_corners = [&gcode_result](const TBuffer& t_buffer, MultiVertexBuffer& v_multibuffer) {
        auto extract_position_at = [](const VertexBuffer& vertices, size_t offset) {
            return Vec3f(vertices[offset + 0], vertices[offset + 1], vertices[offset + 2]);
        };
        auto update_position_at = [](VertexBuffer& vertices, size_t offset, const Vec3f& position) {
            vertices[offset + 0] = position[0];
            vertices[offset + 1] = position[1];
            vertices[offset + 2] = position[2];
        };
        auto match_right_vertices = [&](const Path::Sub_Path& prev_sub_path, const Path::Sub_Path& next_sub_path,
                size_t curr_s_id, size_t vertex_size_floats, const Vec3f& displacement_vec) {
            if (&prev_sub_path == &next_sub_path) { // previous and next segment are both contained into to the same vertex buffer
                VertexBuffer& vbuffer = v_multibuffer[prev_sub_path.first.b_id];
                // offset into the vertex buffer of the next segment 1st vertex
                size_t next_1st_offset = (prev_sub_path.last.s_id - curr_s_id) * 6 * vertex_size_floats;
                // offset into the vertex buffer of the right vertex of the previous segment
                size_t prev_right_offset = prev_sub_path.last.i_id - next_1st_offset - 3 * vertex_size_floats;
                // new position of the right vertices
                Vec3f shared_vertex = extract_position_at(vbuffer, prev_right_offset) + displacement_vec;
                // update previous segment
                update_position_at(vbuffer, prev_right_offset, shared_vertex);
                // offset into the vertex buffer of the right vertex of the next segment
                size_t next_right_offset = next_sub_path.last.i_id - next_1st_offset;
                // update next segment
                update_position_at(vbuffer, next_right_offset, shared_vertex);
            }
            else { // previous and next segment are contained into different vertex buffers
                VertexBuffer& prev_vbuffer = v_multibuffer[prev_sub_path.first.b_id];
                VertexBuffer& next_vbuffer = v_multibuffer[next_sub_path.first.b_id];
                // offset into the previous vertex buffer of the right vertex of the previous segment
                size_t prev_right_offset = prev_sub_path.last.i_id - 3 * vertex_size_floats;
                // new position of the right vertices
                Vec3f shared_vertex = extract_position_at(prev_vbuffer, prev_right_offset) + displacement_vec;
                // update previous segment
                update_position_at(prev_vbuffer, prev_right_offset, shared_vertex);
                // offset into the next vertex buffer of the right vertex of the next segment
                size_t next_right_offset = next_sub_path.first.i_id + 1 * vertex_size_floats;
                // update next segment
                update_position_at(next_vbuffer, next_right_offset, shared_vertex);
            }
        };
        auto match_left_vertices = [&](const Path::Sub_Path& prev_sub_path, const Path::Sub_Path& next_sub_path,
                size_t curr_s_id, size_t vertex_size_floats, const Vec3f& displacement_vec) {
            if (&prev_sub_path == &next_sub_path) { // previous and next segment are both contained into to the same vertex buffer
                VertexBuffer& vbuffer = v_multibuffer[prev_sub_path.first.b_id];
                // offset into the vertex buffer of the next segment 1st vertex
                size_t next_1st_offset = (prev_sub_path.last.s_id - curr_s_id) * 6 * vertex_size_floats;
                // offset into the vertex buffer of the left vertex of the previous segment
                size_t prev_left_offset = prev_sub_path.last.i_id - next_1st_offset - 1 * vertex_size_floats;
                // new position of the left vertices
                Vec3f shared_vertex = extract_position_at(vbuffer, prev_left_offset) + displacement_vec;
                // update previous segment
                update_position_at(vbuffer, prev_left_offset, shared_vertex);
                // offset into the vertex buffer of the left vertex of the next segment
                size_t next_left_offset = next_sub_path.last.i_id - next_1st_offset + 1 * vertex_size_floats;
                // update next segment
                update_position_at(vbuffer, next_left_offset, shared_vertex);
            }
            else { // previous and next segment are contained into different vertex buffers
                VertexBuffer& prev_vbuffer = v_multibuffer[prev_sub_path.first.b_id];
                VertexBuffer& next_vbuffer = v_multibuffer[next_sub_path.first.b_id];
                // offset into the previous vertex buffer of the left vertex of the previous segment
                size_t prev_left_offset = prev_sub_path.last.i_id - 1 * vertex_size_floats;
                // new position of the left vertices
                Vec3f shared_vertex = extract_position_at(prev_vbuffer, prev_left_offset) + displacement_vec;
                // update previous segment
                update_position_at(prev_vbuffer, prev_left_offset, shared_vertex);
                // offset into the next vertex buffer of the left vertex of the next segment
                size_t next_left_offset = next_sub_path.first.i_id + 3 * vertex_size_floats;
                // update next segment
                update_position_at(next_vbuffer, next_left_offset, shared_vertex);
            }
        };

        size_t vertex_size_floats = t_buffer.vertices.vertex_size_floats();
        for (const Path& path : t_buffer.paths) {
            // the two segments of the path sharing the current vertex may belong
            // to two different vertex buffers
            size_t prev_sub_path_id = 0;
            size_t next_sub_path_id = 0;
            size_t path_vertices_count = path.vertices_count();
            float half_width = 0.5f * path.width;
            for (size_t j = 1; j < path_vertices_count - 1; ++j) {
                size_t curr_s_id = path.sub_paths.front().first.s_id + j;
                const Vec3f& prev = gcode_result.moves[curr_s_id - 1].position;
                const Vec3f& curr = gcode_result.moves[curr_s_id].position;
                const Vec3f& next = gcode_result.moves[curr_s_id + 1].position;

                // select the subpaths which contains the previous/next segments
                if (!path.sub_paths[prev_sub_path_id].contains(curr_s_id))
                    ++prev_sub_path_id;
                if (!path.sub_paths[next_sub_path_id].contains(curr_s_id + 1))
                    ++next_sub_path_id;
                const Path::Sub_Path& prev_sub_path = path.sub_paths[prev_sub_path_id];
                const Path::Sub_Path& next_sub_path = path.sub_paths[next_sub_path_id];

                Vec3f prev_dir = (curr - prev).normalized();
                Vec3f prev_right = Vec3f(prev_dir[1], -prev_dir[0], 0.0f).normalized();
                Vec3f prev_up = prev_right.cross(prev_dir);

                Vec3f next_dir = (next - curr).normalized();

                bool is_right_turn = prev_up.dot(prev_dir.cross(next_dir)) <= 0.0f;
                float cos_dir = prev_dir.dot(next_dir);
                // whether the angle between adjacent segments is greater than 45 degrees
                bool is_sharp = cos_dir < 0.7071068f;

                float displacement = 0.0f;
                if (cos_dir > -0.9998477f) {
                    // if the angle between adjacent segments is smaller than 179 degrees
                    Vec3f med_dir = (prev_dir + next_dir).normalized();
                    displacement = half_width * ::tan(::acos(std::clamp(next_dir.dot(med_dir), -1.0f, 1.0f)));
                }

                float sq_prev_length = (curr - prev).squaredNorm();
                float sq_next_length = (next - curr).squaredNorm();
                float sq_displacement = sqr(displacement);
                bool can_displace = displacement > 0.0f && sq_displacement < sq_prev_length&& sq_displacement < sq_next_length;

                if (can_displace) {
                    // displacement to apply to the vertices to match
                    Vec3f displacement_vec = displacement * prev_dir;
                    // matches inner corner vertices
                    if (is_right_turn)
                        match_right_vertices(prev_sub_path, next_sub_path, curr_s_id, vertex_size_floats, -displacement_vec);
                    else
                        match_left_vertices(prev_sub_path, next_sub_path, curr_s_id, vertex_size_floats, -displacement_vec);

                    if (!is_sharp) {
                        // matches outer corner vertices
                        if (is_right_turn)
                            match_left_vertices(prev_sub_path, next_sub_path, curr_s_id, vertex_size_floats, displacement_vec);
                        else
                            match_right_vertices(prev_sub_path, next_sub_path, curr_s_id, vertex_size_floats, displacement_vec);
                    }
                }
            }
        }
    };

    // smooth toolpaths corners for TBuffers using triangles
    if(!isTRender)
    {
        for (size_t i = 0; i < m_buffers.size(); ++i) {
            const TBuffer& t_buffer = m_buffers[i];
            if (t_buffer.render_primitive_type == TBuffer::ERenderPrimitiveType::Triangle) {
                smooth_triangle_toolpaths_corners(t_buffer, vertices[i]);
            }
        }
    }
    for (MultiVertexBuffer& v_multibuffer : vertices) {
        for (VertexBuffer& v_buffer : v_multibuffer) {
            v_buffer.shrink_to_fit();
        }
    }

    // move the wipe toolpaths half height up to render them on proper position
    MultiVertexBuffer& wipe_vertices = vertices[buffer_id(EMoveType::Wipe)];
    for (VertexBuffer& v_buffer : wipe_vertices) {
        for (size_t i = 2; i < v_buffer.size(); i += 3) {
            v_buffer[i] += 0.5f * GCodeProcessor::Wipe_Height;
        }
    }

    int numTasks = m_buffers.size() * 2;
    int lastNum = 0;
    for (size_t i = 0; i < m_buffers.size(); ++i) {
        TBuffer& t_buffer = m_buffers[i];

        glsafe(glGenVertexArrays(1, &t_buffer.t_vao));
        glsafe(glBindVertexArray(t_buffer.t_vao));
        
        int showIntNum = (i * 100 / numTasks) * 0.65 + 30 ;
        if(lastNum != showIntNum)
        {
            emit setValue(showIntNum);
            lastNum = showIntNum;

        }
        const MultiVertexBuffer& v_multibuffer = vertices[i];
        for (const VertexBuffer& v_buffer : v_multibuffer) {
            size_t size_elements = v_buffer.size();
            size_t size_bytes = size_elements * sizeof(float);
            size_t vertices_count = size_elements / t_buffer.vertices.vertex_size_floats();
            t_buffer.vertices.count += vertices_count;

#if ENABLE_GCODE_VIEWER_STATISTICS
            m_statistics.total_vertices_gpu_size += static_cast<int64_t>(size_bytes);
            m_statistics.max_vbuffer_gpu_size = std::max(m_statistics.max_vbuffer_gpu_size, static_cast<int64_t>(size_bytes));
            ++m_statistics.vbuffers_count;
#endif // ENABLE_GCODE_VIEWER_STATISTICS

            GLuint id = 0;
            glsafe(glGenBuffers(1, &id));
            t_buffer.vertices.vbos.push_back(static_cast<unsigned int>(id));
            t_buffer.vertices.sizes.push_back(size_bytes);
            glsafe(glBindBuffer(GL_ARRAY_BUFFER, id));
            glsafe(glBufferData(GL_ARRAY_BUFFER, size_bytes, v_buffer.data(), GL_STATIC_DRAW));
            glsafe(glBindBuffer(GL_ARRAY_BUFFER, 0));
        }

        glsafe(glBindVertexArray(0));

    }

    // dismiss vertices data, no more needed
    // vertices_cpu = std::move(vertices);
    std::vector<MultiVertexBuffer>().swap(vertices);

    // toolpaths data -> extract indices from result
    // paths may have been filled while extracting vertices,
    // so reset them, they will be filled again while extracting indices
    for (TBuffer& buffer : m_buffers) {
        buffer.paths.clear();
        std::vector<Path>().swap(buffer.paths);
        //buffer.paths.clear();
    }

    // variable used to keep track of the current vertex buffers index and size
    using CurrVertexBuffer = std::pair<unsigned int, size_t>;
    std::vector<CurrVertexBuffer> curr_vertex_buffers(m_buffers.size(), { 0, 0 });

    // variable used to keep track of the vertex buffers ids
    using VboIndexList = std::vector<unsigned int>;
    std::vector<VboIndexList> vbo_indices(m_buffers.size());

    //add index
    for (size_t i = 0; i < m_moves_count; ++i) {
        const GCodeProcessor::MoveVertex& curr = gcode_result.moves[i];

        // skip first vertex
        if (i == 0)
            continue;

        const GCodeProcessor::MoveVertex& prev = gcode_result.moves[i - 1];
#if ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
        const GCodeProcessor::MoveVertex* next = nullptr;
        if (i < m_moves_count - 1)
            next = &gcode_result.moves[i + 1];
#endif // ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS

        unsigned char id = buffer_id(curr.type);
        TBuffer& t_buffer = m_buffers[id];
        MultiIndexBuffer& i_multibuffer = indices[id];
        CurrVertexBuffer& curr_vertex_buffer = curr_vertex_buffers[id];
        VboIndexList& vbo_index_list = vbo_indices[id];

        // ensure there is at least one index buffer
        if (i_multibuffer.empty()) {
            i_multibuffer.push_back(IndexBuffer());
            vbo_index_list.push_back(t_buffer.vertices.vbos[curr_vertex_buffer.first]);
        }

        // if adding the indices for the current segment exceeds the threshold size of the current index buffer
        // create another index buffer
#if ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
        if (i_multibuffer.back().size() * sizeof(IBufferType) >= IBUFFER_THRESHOLD_BYTES - t_buffer.max_indices_per_segment_size_bytes()) {
#else
        if (i_multibuffer.back().size() * sizeof(IBufferType) >= IBUFFER_THRESHOLD_BYTES - t_buffer.indices_per_segment_size_bytes()) {
#endif // ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
            i_multibuffer.push_back(IndexBuffer());
            vbo_index_list.push_back(t_buffer.vertices.vbos[curr_vertex_buffer.first]);
            if (t_buffer.render_primitive_type != TBuffer::ERenderPrimitiveType::Point) {
                Path& last_path = t_buffer.paths.back();
                last_path.add_sub_path(prev, static_cast<unsigned int>(i_multibuffer.size()) - 1, 0, i - 1);
            }
        }

        // if adding the vertices for the current segment exceeds the threshold size of the current vertex buffer
        // create another index buffer
        if (curr_vertex_buffer.second * t_buffer.vertices.vertex_size_bytes() > t_buffer.vertices.max_size_bytes() - t_buffer.max_vertices_per_segment_size_bytes()) {
            i_multibuffer.push_back(IndexBuffer());

            ++curr_vertex_buffer.first;
            curr_vertex_buffer.second = 0;
            vbo_index_list.push_back(t_buffer.vertices.vbos[curr_vertex_buffer.first]);

            if (t_buffer.render_primitive_type != TBuffer::ERenderPrimitiveType::Point) {
                Path& last_path = t_buffer.paths.back();
                last_path.add_sub_path(prev, static_cast<unsigned int>(i_multibuffer.size()) - 1, 0, i - 1);
            }
        }

        IndexBuffer& i_buffer = i_multibuffer.back();

        switch (t_buffer.render_primitive_type)
        {
        case TBuffer::ERenderPrimitiveType::Point: {
            add_indices_as_point(curr, t_buffer, static_cast<unsigned int>(i_multibuffer.size()) - 1, i_buffer, i);
            curr_vertex_buffer.second += t_buffer.max_vertices_per_segment();
            break;
        }
        case TBuffer::ERenderPrimitiveType::Line: {
            add_indices_as_line(prev, curr, t_buffer, static_cast<unsigned int>(i_multibuffer.size()) - 1, i_buffer, i);
            curr_vertex_buffer.second += t_buffer.max_vertices_per_segment();
            break;
        }
        case TBuffer::ERenderPrimitiveType::AnkerMove: {
            add_indices_as_ankermove(prev, curr, t_buffer, static_cast<unsigned int>(i_multibuffer.size()) - 1, i_buffer, i);
            curr_vertex_buffer.second += t_buffer.max_vertices_per_segment();
            break;
        }
        case TBuffer::ERenderPrimitiveType::Triangle: {
#if ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
            add_indices_as_solid(prev, curr, next, t_buffer, curr_vertex_buffer.second, static_cast<unsigned int>(i_multibuffer.size()) - 1, i_buffer, i);
#else
            add_indices_as_solid(prev, curr, t_buffer, curr_vertex_buffer.second, static_cast<unsigned int>(i_multibuffer.size()) - 1, i_buffer, i);
#endif // ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
            break;
        }
        }
    }

    for (MultiIndexBuffer& i_multibuffer : indices) {
        for (IndexBuffer& i_buffer : i_multibuffer) {
            i_buffer.shrink_to_fit();
        }
    }

    //TODO: send data to meshO
    for (size_t i = 0; i < m_buffers.size(); ++i) {
        //progress.setValue(2*i);
        int showIntNum = (i * 100 / numTasks) * 0.65 + 30 + 32.5;
        if(lastNum != showIntNum)
        {
            emit setValue(showIntNum);
            lastNum = showIntNum;
        }
        TBuffer& t_buffer = m_buffers[i];

        glsafe(glBindVertexArray(t_buffer.t_vao));

        const MultiIndexBuffer& i_multibuffer = indices[i];
        for (const IndexBuffer& i_buffer : i_multibuffer) {
            size_t size_elements = i_buffer.size();
            size_t size_bytes = size_elements * sizeof(IBufferType);

            // stores index buffer informations into TBuffer
            t_buffer.indices.push_back(IBuffer());
            IBuffer& ibuf = t_buffer.indices.back();
            ibuf.count = size_elements;
            ibuf.vbo = vbo_indices[i][t_buffer.indices.size() - 1];

#if ENABLE_GCODE_VIEWER_STATISTICS
            m_statistics.total_indices_gpu_size += static_cast<int64_t>(size_bytes);
            m_statistics.max_ibuffer_gpu_size = std::max(m_statistics.max_ibuffer_gpu_size, static_cast<int64_t>(size_bytes));
            ++m_statistics.ibuffers_count;
#endif // ENABLE_GCODE_VIEWER_STATISTICS

            glsafe(glGenBuffers(1, &ibuf.ibo));
            glsafe(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuf.ibo));
            glsafe(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_bytes, i_buffer.data(), GL_STATIC_DRAW));
            glsafe(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        }

        glsafe(glBindVertexArray(0));

    }

    // dismiss indices data, no more needed
    std::vector<MultiIndexBuffer>().swap(indices);

    // layers zs / roles / extruder ids -> extract from result
    size_t last_travel_s_id = 0;
    for (size_t i = 0; i < m_moves_count; ++i) {
        const GCodeProcessor::MoveVertex& move = gcode_result.moves[i];
        if (move.type == EMoveType::Extrude) {
            // layers zs
            const double* const last_z = m_layers.empty() ? nullptr : &m_layers.get_zs().back();
            double z = static_cast<double>(move.position[2]);
            if (last_z == nullptr || z < *last_z - EPSILON || *last_z + EPSILON < z)
                m_layers.append(z, { last_travel_s_id, i }, { 0 });//new layer
            else
                m_layers.get_endpoints().back().last = i;
            //cal filament
            m_layers.get_filamentcost().back()[move.extrusion_role] += move.delta_extruder;
            // extruder ids
            m_extruder_ids.emplace_back(move.extruder_id);
            // roles
            if (i > 0)
                m_roles.emplace_back(move.extrusion_role);


        }
        else if (move.type == EMoveType::Travel) {
            if (i - last_travel_s_id > 1 && !m_layers.empty())
                m_layers.get_endpoints().back().last = i;

            last_travel_s_id = i;
        }
    }

    // roles -> remove duplicates
    std::sort(m_roles.begin(), m_roles.end());
    m_roles.erase(std::unique(m_roles.begin(), m_roles.end()), m_roles.end());
    m_roles.shrink_to_fit();

    // extruder ids -> remove duplicates
    std::sort(m_extruder_ids.begin(), m_extruder_ids.end());
    m_extruder_ids.erase(std::unique(m_extruder_ids.begin(), m_extruder_ids.end()), m_extruder_ids.end());
    m_extruder_ids.shrink_to_fit();

    // set layers z range
    if (!m_layers.empty())
        m_layers_z_range = { 0, static_cast<unsigned int>(m_layers.size() - 1) };

    // change color of paths whose layer contains option points
    if (!options_zs.empty()) {
        TBuffer& extrude_buffer = m_buffers[buffer_id(EMoveType::Extrude)];
        for (Path& path : extrude_buffer.paths) {
            float z = path.sub_paths.front().first.position[2];
            if (std::find_if(options_zs.begin(), options_zs.end(), [z](float f) { return f - EPSILON <= z && z <= f + EPSILON; }) != options_zs.end())
                path.cp_color_id = 255 - path.cp_color_id;
        }
    }


    m_time_statistics = gcode_result.time_statistics;

    if (m_time_estimate_mode != PrintEstimatedTimeStatistics::ETimeMode::Normal) {
        float time = m_time_statistics.modes[static_cast<size_t>(m_time_estimate_mode)].time;
        if (time == 0.0f ||
                short_time(get_time_dhms(time)) == short_time(get_time_dhms(m_time_statistics.modes[static_cast<size_t>(PrintEstimatedTimeStatistics::ETimeMode::Normal)].time)))
            m_time_estimate_mode = PrintEstimatedTimeStatistics::ETimeMode::Normal;
    }

}


bool GcodeViewer::Path::matches(const GCodeProcessor::MoveVertex & move) const
{
#if ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE
    auto matches_percent = [](float value1, float value2, float max_percent) {
        return std::abs(value2 - value1) / value1 <= max_percent;
    };
#endif // ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE

    switch (move.type)
    {
    case EMoveType::Tool_change:
    case EMoveType::Color_change:
    case EMoveType::Pause_Print:
    case EMoveType::Custom_GCode:
    case EMoveType::Retract:
    case EMoveType::Unretract:
    case EMoveType::Extrude: {
        // use rounding to reduce the number of generated paths
#if ENABLE_SPLITTED_VERTEX_BUFFER
#if ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE
        return type == move.type && extruder_id == move.extruder_id && cp_color_id == move.cp_color_id && role == move.extrusion_role &&
                move.position[2] <= sub_paths.front().first.position[2] && feedrate == move.feedrate && fan_speed == move.fan_speed &&
                layer_time == move.layer_duration && elapsed_time == move.time && extruder_temp == move.temperature &&
                height == m_round_to_nearest(move.height, 2) && width == m_round_to_nearest(move.width, 2) &&
                matches_percent(volumetric_rate, move.volumetric_rate(), 0.05f);
#else
        return type == move.type && move.position[2] <= sub_paths.front().position[2] && role == move.extrusion_role && height == m_round_to_nearest(move.height, 2) &&
                width == m_round_to_nearest(move.width, 2) && feedrate == move.feedrate && fan_speed == move.fan_speed &&
                layer_time == move.layer_duration && elapsed_time == move.time && extruder_temp == move.temperature &&
                volumetric_rate == m_round_to_nearest(move.volumetric_rate(), 2) && extruder_id == move.extruder_id &&
                cp_color_id == move.cp_color_id;
#endif // ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE
#else
#if ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE
        return type == move.type && extruder_id == move.extruder_id && cp_color_id == move.cp_color_id && role == move.extrusion_role &&
                move.position[2] <= first.position[2] && feedrate == move.feedrate && fan_speed == move.fan_speed &&
                layer_time == move.layer_duration && elapsed_time == move.time && extruder_temp == move.temperature &&
                height == m_round_to_nearest(move.height, 2) && width == m_round_to_nearest(move.width, 2) &&
                matches_percent(volumetric_rate, move.volumetric_rate(), 0.05f);
#else
        return type == move.type && move.position[2] <= first.position[2] && role == move.extrusion_role && height == m_round_to_nearest(move.height, 2) &&
                width == m_round_to_nearest(move.width, 2) && feedrate == move.feedrate && fan_speed == move.fan_speed &&
                layer_time == move.layer_duration && elapsed_time == move.time && extruder_temp == move.temperature &&
                volumetric_rate == m_round_to_nearest(move.volumetric_rate(), 2) && extruder_id == move.extruder_id &&
                cp_color_id == move.cp_color_id;
#endif // ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE
#endif // ENABLE_SPLITTED_VERTEX_BUFFER
    }
    case EMoveType::Travel: {
        return type == move.type && feedrate == move.feedrate && extruder_id == move.extruder_id && cp_color_id == move.cp_color_id;
    }
    default: { return false; }
    }
}



void GcodeViewer::resetGpu(bool needModel)
{
    if (cnt)
    {
        cnt->makeCurrent(m_offscreenSurface);
    }
    else
    {
        makeCurrent();
    }

    if(!needModel)
    {
        this->m_sequential_view.marker.m_model.reset();
    }
    for (TBuffer& buffer : m_buffers) {
        //
        for (IBuffer& ibuffer : buffer.indices) {
            if (ibuffer.ibo > 0) {
                glsafe(glDeleteBuffers(1, &ibuffer.ibo));
            }
        }

        if (!buffer.vertices.vbos.empty()) {
            glsafe(glDeleteBuffers(static_cast<GLsizei>(buffer.vertices.vbos.size()), static_cast<const GLuint*>(buffer.vertices.vbos.data())));

        }

        buffer.reset();
    }

    std::array<SequentialRangeCap, 2>* sequential_range_caps = const_cast<std::array<SequentialRangeCap, 2>*>(&m_sequential_range_caps);
    
    if ((*sequential_range_caps)[0].ibo > 0)
    {
        glsafe(glDeleteBuffers(1, &(*sequential_range_caps)[0].ibo));
    }
    (*sequential_range_caps)[0].reset();
    if ((*sequential_range_caps)[1].ibo > 0)
    {
        glsafe(glDeleteBuffers(1, &(*sequential_range_caps)[0].ibo));
    }
    (*sequential_range_caps)[1].reset();


    if (cnt)
    {
        cnt->doneCurrent();
    }
    else
    {
        doneCurrent();
    }
}

void GcodeViewer::reset()
{
    m_moves_count = 0;
    for (TBuffer& buffer : m_buffers) {
        buffer.reset();
    }

    //set_toolpath_move_type_visible(EMoveType::Extrude, false);  maybe need
    m_time_statistics.reset();
    m_layers.reset();
    this->times.clear();
    this->m_boundingBox = boundingBox{};
}

void GcodeViewer::GLModel::reset()
{
    // release gpu memory
    if (m_ibo_id > 0) {
        glsafe(glDeleteBuffers(1, &m_ibo_id));
        m_ibo_id = 0;
    }
    if (m_vbo_id > 0) {
        glsafe(glDeleteBuffers(1, &m_vbo_id));
        m_vbo_id = 0;
    }

}

void GcodeViewer::GLModel::render(QOpenGLShaderProgram& shaderProgram)
{
    if (m_vbo_id == 0 || m_ibo_id == 0)
        return;

    glsafe(glBindVertexArray(m_vao_id));

    glsafe(glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id));
    int attr = -1;
    attr = shaderProgram.attributeLocation("aPos");
    shaderProgram.setAttributeBuffer(attr, GL_FLOAT, 0, 3, 6 * sizeof(float));
    shaderProgram.enableAttributeArray(attr);
    attr = shaderProgram.attributeLocation("aNormal");
    shaderProgram.setAttributeBuffer(attr, GL_FLOAT, 3 * sizeof(float), 3, 6 * sizeof(float));
    shaderProgram.enableAttributeArray(attr);
    //    glsafe(glVertexPointer(3, GL_FLOAT, 6 * sizeof(float), (const void*)0));
    //    glsafe(glNormalPointer(GL_FLOAT, 6 * sizeof(float), (const void*)(3 * sizeof(float))));

    //    glsafe(glEnableClientState(GL_VERTEX_ARRAY));
    //    glsafe(glEnableClientState(GL_NORMAL_ARRAY));

    glsafe(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo_id));
    glsafe(glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices_count), GL_UNSIGNED_INT, (const void*)0));
    glsafe(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    //    glsafe(glDisableClientState(GL_NORMAL_ARRAY));
    //    glsafe(glDisableClientState(GL_VERTEX_ARRAY));

    glsafe(glBindBuffer(GL_ARRAY_BUFFER, 0));

    glsafe(glBindVertexArray(0));


}

void GcodeViewer::GLModel::send_to_gpu(const std::vector<float>&vertices, const std::vector<unsigned int>&indices)
{
    // vertex data -> send to gpu

    glsafe(glGenVertexArrays(1, &m_vao_id));
    glsafe(glBindVertexArray(m_vao_id));

    glsafe(glGenBuffers(1, &m_vbo_id));
    glsafe(glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id));
    glsafe(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW));
    glsafe(glBindBuffer(GL_ARRAY_BUFFER, 0));

    // indices data -> send to gpu
    glsafe(glGenBuffers(1, &m_ibo_id));
    glsafe(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo_id));
    glsafe(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW));
    glsafe(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));


    glsafe(glBindVertexArray(0));

}

void GcodeViewer::GLModel::init_from(const GLModelInitializationData & data)
{
    assert(!data.positions.empty() && !data.triangles.empty());
    assert(data.positions.size() == data.normals.size());
    this->initializeOpenGLFunctions();
    auto getBoundingbox = [this](Vec3f curPos)
    {
        if (this->model_bounding_box.isDefine)
        {
            this->model_bounding_box.min[0] = this->model_bounding_box.min[0] < curPos[0] ? this->model_bounding_box.min[0] : curPos[0];
            this->model_bounding_box.min[1] = this->model_bounding_box.min[1] < curPos[1] ? this->model_bounding_box.min[1] : curPos[1];
            this->model_bounding_box.min[2] = this->model_bounding_box.min[2] < curPos[2] ? this->model_bounding_box.min[2] : curPos[2];
            this->model_bounding_box.max[0] = this->model_bounding_box.max[0] > curPos[0] ? this->model_bounding_box.max[0] : curPos[0];
            this->model_bounding_box.max[1] = this->model_bounding_box.max[1] > curPos[1] ? this->model_bounding_box.max[1] : curPos[1];
            this->model_bounding_box.max[2] = this->model_bounding_box.max[2] > curPos[2] ? this->model_bounding_box.max[2] : curPos[2];

        }
        else {
            this->model_bounding_box.min = curPos;
            this->model_bounding_box.max = curPos;
            this->model_bounding_box.isDefine = true;
        }
    };
    std::vector<float> vertices(6 * data.positions.size());
    // vertices/normals data
    for (size_t i = 0; i < data.positions.size(); ++i) {
        size_t offset = i * 6;
        memcpy(static_cast<void*>(&vertices[offset]), static_cast<const void*>(data.positions[i].data()), 3 * sizeof(float));
        memcpy(static_cast<void*>(&vertices[3 + offset]), static_cast<const void*>(data.normals[i].data()), 3 * sizeof(float));
    }

    // indices data
    std::vector<unsigned int> indices(3 * data.triangles.size());
    for (size_t i = 0; i < data.triangles.size(); ++i) {
        for (size_t j = 0; j < 3; ++j) {
            indices[i * 3 + j] = static_cast<unsigned int>(data.triangles[i][j]);
        }
    }
    m_indices_count = static_cast<unsigned int>(indices.size());

    for (size_t i = 0; i < data.positions.size(); ++i) {
        getBoundingbox(data.positions[i]);
    }
    send_to_gpu(vertices, indices);
}

GcodeViewer::GLModelInitializationData GcodeViewer::stilized_arrow(int resolution, float tip_radius, float tip_height, float stem_radius, float stem_height)
{
    auto append_vertex = [](GLModelInitializationData& data, const Vec3f& position, const Vec3f& normal) {
        data.positions.emplace_back(position);
        data.normals.emplace_back(normal);
    };

    resolution = std::max(4, resolution);

    GLModelInitializationData data;

    const float angle_step = 2.0f * M_PI / static_cast<float>(resolution);
    std::vector<float> cosines(resolution);
    std::vector<float> sines(resolution);

    for (int i = 0; i < resolution; ++i)
    {
        float angle = angle_step * static_cast<float>(i);
        cosines[i] = ::cos(angle);
        sines[i] = -::sin(angle);
    }

    const float total_height = tip_height + stem_height;

    // tip vertices/normals
    append_vertex(data, { 0.0f, 0.0f, total_height }, Vec3f::UnitZ());
    for (int i = 0; i < resolution; ++i)
    {
        append_vertex(data, { tip_radius * sines[i], tip_radius * cosines[i], stem_height }, { sines[i], cosines[i], 0.0f });
    }

    // tip triangles
    for (int i = 0; i < resolution; ++i)
    {
        int v3 = (i < resolution - 1) ? i + 2 : 1;
        data.triangles.emplace_back(0, i + 1, v3);
    }

    // tip cap outer perimeter vertices
    for (int i = 0; i < resolution; ++i)
    {
        append_vertex(data, { tip_radius * sines[i], tip_radius * cosines[i], stem_height }, -Vec3f::UnitZ());
    }

    // tip cap inner perimeter vertices
    for (int i = 0; i < resolution; ++i)
    {
        append_vertex(data, { stem_radius * sines[i], stem_radius * cosines[i], stem_height }, -Vec3f::UnitZ());
    }

    // tip cap triangles
    for (int i = 0; i < resolution; ++i)
    {
        int v2 = (i < resolution - 1) ? i + resolution + 2 : resolution + 1;
        int v3 = (i < resolution - 1) ? i + 2 * resolution + 2 : 2 * resolution + 1;
        data.triangles.emplace_back(i + resolution + 1, v3, v2);
        data.triangles.emplace_back(i + resolution + 1, i + 2 * resolution + 1, v3);
    }

    // stem bottom vertices
    for (int i = 0; i < resolution; ++i)
    {
        append_vertex(data, { stem_radius * sines[i], stem_radius * cosines[i], stem_height }, { sines[i], cosines[i], 0.0f });
    }

    // stem top vertices
    for (int i = 0; i < resolution; ++i)
    {
        append_vertex(data, { stem_radius * sines[i], stem_radius * cosines[i], 0.0f }, { sines[i], cosines[i], 0.0f });
    }

    // stem triangles
    for (int i = 0; i < resolution; ++i)
    {
        int v2 = (i < resolution - 1) ? i + 3 * resolution + 2 : 3 * resolution + 1;
        int v3 = (i < resolution - 1) ? i + 4 * resolution + 2 : 4 * resolution + 1;
        data.triangles.emplace_back(i + 3 * resolution + 1, v3, v2);
        data.triangles.emplace_back(i + 3 * resolution + 1, i + 4 * resolution + 1, v3);
    }

    // stem cap vertices
    append_vertex(data, Vec3f::Zero(), -Vec3f::UnitZ());
    for (int i = 0; i < resolution; ++i)
    {
        append_vertex(data, { stem_radius * sines[i], stem_radius * cosines[i], 0.0f }, -Vec3f::UnitZ());
    }

    // stem cap triangles
    for (int i = 0; i < resolution; ++i)
    {
        int v3 = (i < resolution - 1) ? i + 5 * resolution + 3 : 5 * resolution + 2;
        data.triangles.emplace_back(5 * resolution + 1, v3, i + 5 * resolution + 2);
    }

    return data;
}


void GcodeViewer::SequentialView::Marker::init()
{
    m_model.init_from(stilized_arrow(16, 2.0f, 4.0f, 1.0f, 7.0f));
    this->initializeOpenGLFunctions();
}

void GcodeViewer::SequentialView::Marker::set_world_position(const Vec3f & position)
{
    m_world_position = position;
    auto assemble_transform = [](const Vec3d& translation, const Vec3d& rotation = Vec3d::Zero(), const Vec3d& scale = Vec3d::Ones(), const Vec3d& mirror = Vec3d::Ones())
    {
        Transform3d transform;
        transform = Transform3d::Identity();
        transform.translate(translation);
        transform.rotate(Eigen::AngleAxisd(rotation(2), Vec3d::UnitZ()) * Eigen::AngleAxisd(rotation(1), Vec3d::UnitY()) * Eigen::AngleAxisd(rotation(0), Vec3d::UnitX()));
        transform.scale(scale.cwiseProduct(mirror));
        return transform;
    };
    float z_size = m_model.get_bounding_box().max(2) - m_model.get_bounding_box().min(2);
    m_world_transform = (assemble_transform((position + m_z_offset * Vec3f::UnitZ()).cast<double>()) * assemble_transform(z_size * Vec3d::UnitZ(), { M_PI, 0.0, 0.0 })).cast<float>();
}

void GcodeViewer::SequentialView::Marker::render(QOpenGLShaderProgram & shaderProgram1,QMatrix4x4 vm, QMatrix4x4 pm)
{
    if (!m_visible)
        return;

    QOpenGLShaderProgram* shaderProgram = &shaderProgram1;
    if (shaderProgram == nullptr)
        return;
    //glsafe(glEnable(GL_BLEND));
    //glsafe(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    shaderProgram->bind();

    int id = -1;
    id = glGetUniformLocation(shaderProgram->programId(), "uniform_color");
    shaderProgram->setUniformValue(id, (GLfloat)(m_color[0]) , (GLfloat)(m_color[1]), (GLfloat)(m_color[2]) , (GLfloat)(m_color[3]) );
    //    if (id >= 0) {
    //        glsafe(glUniform4f(id, (GLfloat)(m_color[0]),(GLfloat)(m_color[1]),(GLfloat)(m_color[2]),(GLfloat)(m_color[3])));
    //    }
    QMatrix4x4 t(m_world_transform.data());
    shaderProgram->setUniformValue("mv_matrix", vm*t.transposed());
    shaderProgram->setUniformValue("p_matrix", pm);
    QMatrix3x3 normalM = (vm).normalMatrix();//vM.inverted().transposed().normalMatrix();
    shaderProgram->setUniformValue("normal_matrix", normalM);
    m_model.render(shaderProgram1);

    //glsafe(glPopMatrix());
    shaderProgram->release();
    //glsafe(glDisable(GL_BLEND));
}

void GcodeViewer::refresh(const GCodeProcessor::Result &gcode_result, const std::vector<std::string> &str_tool_colors , bool runInOtherThread)
{
#if ENABLE_GCODE_VIEWER_STATISTICS
    auto start_time = std::chrono::high_resolution_clock::now();
#endif // ENABLE_GCODE_VIEWER_STATISTICS
    if (m_moves_count == 0)
        return;
    //TODO:
    if (m_view_type == EViewType::Tool && !gcode_result.extruder_colors.empty())
        // update tool colors from config stored in the gcode
        m_tool_colors = decode_colors(gcode_result.extruder_colors);
    else
        // update tool colors
        m_tool_colors = decode_colors(str_tool_colors);

    //// ensure at least one (default) color is defined
    if (m_tool_colors.empty())
        m_tool_colors.push_back(decode_color("#FF8000"));
    if (m_view_type == EViewType::Filament && !gcode_result.filament_colors.empty())
        // update tool colors from config stored in the gcode
        m_filament_colors = decode_colors(gcode_result.filament_colors);
    else
        // update tool colors
        m_filament_colors = decode_colors(str_tool_colors);


    // update ranges for coloring / legend
    m_extrusions.reset_ranges();
    for (size_t i = 0; i < m_moves_count; ++i) {
        // skip first vertex
        if (i == 0)
            continue;
        const GCodeProcessor::MoveVertex& curr = gcode_result.moves[i];

        switch (curr.type)
        {
        case EMoveType::Extrude:
        {
            m_extrusions.ranges.height.update_from(m_round_to_nearest(curr.height, 2));
            m_extrusions.ranges.width.update_from(m_round_to_nearest(curr.width, 2));
            m_extrusions.ranges.fan_speed.update_from(curr.fan_speed);
            if (curr.layer_duration > 0.f)
                m_extrusions.ranges.layer_duration.update_from(curr.layer_duration);
            m_extrusions.ranges.elapsed_time.update_from(curr.time);
            m_extrusions.ranges.volumetric_rate.update_from(m_round_to_nearest(curr.volumetric_rate(), 2));
            m_extrusions.ranges.extruder_temp.update_from(curr.temperature);
            [[fallthrough]];
        }
        case EMoveType::Travel:
        {
            if (m_buffers[buffer_id(curr.type)].visible)
                m_extrusions.ranges.feedrate.update_from(curr.feedrate);

            break;
        }
        default: { break; }
        }
    }

    if(runInOtherThread)
    {
        refresh_render_paths(false, false);
    }else{
        emit notify_refresh_render_path(false, false);
    }//refresh_render_paths(false, false);
}

void GcodeViewer::refresh_render_paths(bool keep_sequential_current_first, bool keep_sequential_current_last)
{
    QString debugStr;
    if (cnt)
    {
        cnt->makeCurrent(this->m_offscreenSurface);
    }
    else
    {
        this->makeCurrent();
    }

#if ENABLE_GCODE_VIEWER_STATISTICS
    auto start_time = std::chrono::high_resolution_clock::now();
#endif // ENABLE_GCODE_VIEWER_STATISTICS

    auto extrusion_color = [this](const Path& path) {
        Color color;
        switch (m_view_type)
        {
        case EViewType::FeatureType: { color = Extrusion_Role_Colors[static_cast<unsigned int>(path.role)]; break; }
        case EViewType::Height: { color = m_extrusions.ranges.height.get_color_at(path.height); break; }
        case EViewType::Width: { color = m_extrusions.ranges.width.get_color_at(path.width); break; }
        case EViewType::Feedrate: { color = m_extrusions.ranges.feedrate.get_color_at(path.feedrate); break; }
        case EViewType::FanSpeed: { color = m_extrusions.ranges.fan_speed.get_color_at(path.fan_speed); break; }
        case EViewType::LayerTime: { color = m_extrusions.ranges.layer_duration.get_color_at(path.layer_time); break; }
        case EViewType::LayerTimeLog: { color = m_extrusions.ranges.layer_duration.get_color_at(path.layer_time, true); break; }
        case EViewType::Chronology: { color = m_extrusions.ranges.elapsed_time.get_color_at(path.elapsed_time); break; }
        case EViewType::VolumetricRate: { color = m_extrusions.ranges.volumetric_rate.get_color_at(path.volumetric_rate); break; }
        case EViewType::Tool: { color = m_tool_colors[path.extruder_id]; break; }
        case EViewType::Filament: { color = m_filament_colors[path.extruder_id]; break; }
        case EViewType::ExtruderTemp: { color = m_extrusions.ranges.extruder_temp.get_color_at(path.extruder_temp); break; }
        case EViewType::ColorPrint: {
            if (path.cp_color_id >= static_cast<unsigned char>(m_tool_colors.size())) {
                color = { 0.5f, 0.5f, 0.5f };
                //                // complementary color
                //                color = m_tool_colors[255 - path.cp_color_id];
                //                color = { 1.0f - color[0], 1.0f - color[1], 1.0f - color[2] };
            }
            else
                color = m_tool_colors[path.cp_color_id];

            break;
        }
        default: { color = { 1.0f, 1.0f, 1.0f }; break; }
        }
        return color;
    };

    auto travel_color = [this](const Path& path) {
        return (path.delta_extruder < 0.0f) ? Travel_Colors[2] /* Retract */ :
            ((path.delta_extruder > 0.0f) ? Travel_Colors[1] /* Extrude */ :
                Travel_Colors[0] /* Move */);
        };

        auto is_in_layers_range = [this](const Path& path, size_t min_id, size_t max_id) {
            auto in_layers_range = [this, min_id, max_id](size_t id) {
                return m_layers.get_endpoints_at(min_id).first <= id && id <= m_layers.get_endpoints_at(max_id).last;
            };

            return in_layers_range(path.sub_paths.front().first.s_id) || in_layers_range(path.sub_paths.back().last.s_id);
        };

        auto is_travel_in_layers_range = [this](size_t path_id, size_t min_id, size_t max_id) {
            const TBuffer& buffer = m_buffers[buffer_id(EMoveType::Travel)];
            if (path_id >= buffer.paths.size())
                return false;

            Path path = buffer.paths[path_id];
            size_t first = path_id;
            size_t last = path_id;

            // check adjacent paths
            while (first > 0 && path.sub_paths.front().first.position.isApprox(buffer.paths[first - 1].sub_paths.back().last.position)) {
                --first;
                path.sub_paths.front().first = buffer.paths[first].sub_paths.front().first;
            }
            while (last < buffer.paths.size() - 1 && path.sub_paths.back().last.position.isApprox(buffer.paths[last + 1].sub_paths.front().first.position)) {
                ++last;
                path.sub_paths.back().last = buffer.paths[last].sub_paths.back().last;
            }

            size_t min_s_id = m_layers.get_endpoints_at(min_id).first;
            size_t max_s_id = m_layers.get_endpoints_at(max_id).last;

            return (min_s_id <= path.sub_paths.front().first.s_id && path.sub_paths.front().first.s_id <= max_s_id) ||
                    (min_s_id <= path.sub_paths.back().last.s_id && path.sub_paths.back().last.s_id <= max_s_id);
        };

#if ENABLE_GCODE_VIEWER_STATISTICS
        Statistics* statistics = const_cast<Statistics*>(&m_statistics);
        statistics->render_paths_size = 0;
#endif // ENABLE_GCODE_VIEWER_STATISTICS
 //AkUtil::TDebug( "1");
        bool top_layer_only = true;//TODO: get_app_config()->get("seq_top_layer_only") == "1";

        SequentialView::Endpoints global_endpoints = { m_moves_count , 0 };
        SequentialView::Endpoints top_layer_endpoints = global_endpoints;
        SequentialView* sequential_view = const_cast<SequentialView*>(&m_sequential_view);
        if (top_layer_only || !keep_sequential_current_first) sequential_view->current.first = 0;
        if (!keep_sequential_current_last) sequential_view->current.last = m_moves_count;

        // first pass: collect visible paths and update sequential view data
#if ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
        std::vector<std::tuple<unsigned char, unsigned int, unsigned int, unsigned int>> paths;
#else
        std::vector<std::tuple<TBuffer*, unsigned int, unsigned int, unsigned int>> paths;
#endif // ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
        for (size_t b = 0; b < m_buffers.size(); ++b) {
            TBuffer& buffer = const_cast<TBuffer&>(m_buffers[b]);
            // reset render paths
            buffer.render_paths.clear();

            if (!buffer.visible)
                continue;

            for (size_t i = 0; i < buffer.paths.size(); ++i) {
                const Path& path = buffer.paths[i];
                if (path.type == EMoveType::Travel) {
                    if (!is_travel_in_layers_range(i, m_layers_z_range[0], m_layers_z_range[1]))
                        continue;
                }
                else if (!is_in_layers_range(path, m_layers_z_range[0], m_layers_z_range[1]))
                    continue;

                if (path.type == EMoveType::Extrude && !is_visible(path))
                    continue;

                // store valid path
                for (size_t j = 0; j < path.sub_paths.size(); ++j) {
#if ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
                    paths.push_back({ static_cast<unsigned char>(b), path.sub_paths[j].first.b_id, static_cast<unsigned int>(i), static_cast<unsigned int>(j) });
#else
                    paths.push_back({ &buffer, path.sub_paths[j].first.b_id, static_cast<unsigned int>(i), static_cast<unsigned int>(j) });
#endif // ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
                }

                global_endpoints.first = std::min(global_endpoints.first, path.sub_paths.front().first.s_id);
                global_endpoints.last = std::max(global_endpoints.last, path.sub_paths.back().last.s_id);

                if (top_layer_only) {
                    if (path.type == EMoveType::Travel) {
                        if (is_travel_in_layers_range(i, m_layers_z_range[1], m_layers_z_range[1])) {
                            top_layer_endpoints.first = std::min(top_layer_endpoints.first, path.sub_paths.front().first.s_id);
                            top_layer_endpoints.last = std::max(top_layer_endpoints.last, path.sub_paths.back().last.s_id);
                        }
                    }
                    else if (is_in_layers_range(path, m_layers_z_range[1], m_layers_z_range[1])) {
                        top_layer_endpoints.first = std::min(top_layer_endpoints.first, path.sub_paths.front().first.s_id);
                        top_layer_endpoints.last = std::max(top_layer_endpoints.last, path.sub_paths.back().last.s_id);
                    }
                }
            }
        }

//AkUtil::TDebug( "2");
        //fix error (all paths in m_buffers may be out of the m_layers_z_range)
        //FIXME better than this dumb stop-gap
        if (global_endpoints.first > global_endpoints.last) {
            global_endpoints = { 0, m_moves_count };
        }

        // update current sequential position
        sequential_view->current.first = !top_layer_only && keep_sequential_current_first ? std::clamp(sequential_view->current.first, global_endpoints.first, global_endpoints.last) : global_endpoints.first;
        sequential_view->current.last = keep_sequential_current_last ? std::clamp(sequential_view->current.last, global_endpoints.first, global_endpoints.last) : global_endpoints.last;

        // get the world position from gpu
        bool found = false;
        for (const TBuffer& buffer : m_buffers) {
            // searches the path containing the current position

            glsafe(glBindVertexArray(buffer.t_vao));

            for (const Path& path : buffer.paths) {
                if (path.contains(m_sequential_view.current.last)) {
                    int sub_path_id = path.get_id_of_sub_path_containing(m_sequential_view.current.last);
                    if (sub_path_id != -1) {
                        const Path::Sub_Path& sub_path = path.sub_paths[sub_path_id];
                        unsigned int offset = static_cast<unsigned int>(m_sequential_view.current.last - sub_path.first.s_id);
                        if (offset > 0) {
                            if (buffer.render_primitive_type == TBuffer::ERenderPrimitiveType::Line || buffer.render_primitive_type == TBuffer::ERenderPrimitiveType::AnkerMove)
                                offset = 2 * offset - 1;
                            else if (buffer.render_primitive_type == TBuffer::ERenderPrimitiveType::Triangle) {
                                unsigned int indices_count = buffer.indices_per_segment();
#if ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
                                offset = indices_count * (offset - 1) + (indices_count - 2);
                                if (sub_path_id == 0)
                                    offset += 6; // add 2 triangles for starting cap
#else
                                offset = indices_count * (offset - 1) + (indices_count - 6);
#endif // ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
                            }
                        }
                        offset += static_cast<unsigned int>(sub_path.first.i_id);

                        // gets the vertex index from the index buffer on gpu
                        const IBuffer& i_buffer = buffer.indices[sub_path.first.b_id];
                        unsigned int index = 0;
                        //TODO:
                        glsafe(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer.ibo));
                        glsafe(glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLintptr>(offset * sizeof(IBufferType)), static_cast<GLsizeiptr>(sizeof(IBufferType)), static_cast<void*>(&index)));
                        glsafe(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

                        //gets the position from the vertices buffer on gpu
                        //TODO:
                        glsafe(glBindBuffer(GL_ARRAY_BUFFER, i_buffer.vbo));
                        glsafe(glGetBufferSubData(GL_ARRAY_BUFFER, static_cast<GLintptr>(index * buffer.vertices.vertex_size_bytes()), static_cast<GLsizeiptr>(3 * sizeof(float)), static_cast<void*>(sequential_view->current_position.data())));
                        glsafe(glBindBuffer(GL_ARRAY_BUFFER, 0));

                        found = true;
                        break;
                    }
                }
            }

            glsafe(glBindVertexArray(0));

            if (found)
                break;
        }
QDebug(&debugStr)<<"paths.size(): " <<paths.size();
//AkUtil::TDebug( "5" + debugStr);
        // second pass: filter paths by sequential data and collect them by color
        RenderPath* render_path = nullptr;
#if ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
        for (const auto& [tbuffer_id, ibuffer_id, path_id, sub_path_id] : paths) {
            TBuffer& buffer = const_cast<TBuffer&>(m_buffers[tbuffer_id]);

            if (buffer.paths.size() <= path_id) return; // invalid data check
            const Path& path = buffer.paths[path_id];
            if (m_tool_colors.size() <= path.extruder_id) return; // invalid data check
#else
        for (const auto& [buffer, ibuffer_id, path_id, sub_path_id] : paths) {
            const Path& path = buffer->paths[path_id];
#endif // ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS

            const Path::Sub_Path& sub_path = path.sub_paths[sub_path_id];
            if (m_sequential_view.current.last <= sub_path.first.s_id || sub_path.last.s_id <= m_sequential_view.current.first)
                continue;

            Color color;
            float is_render_cap  = -1.0; //for render cap flag  update 20220518
            switch (path.type)
            {
            case EMoveType::Tool_change: { color = Options_Colors[static_cast<unsigned int>(EOptionsColors::ToolChanges)]; break; }
            case EMoveType::Color_change: { color = Options_Colors[static_cast<unsigned int>(EOptionsColors::ColorChanges)]; break; }
            case EMoveType::Pause_Print: { color = Options_Colors[static_cast<unsigned int>(EOptionsColors::PausePrints)]; break; }
            case EMoveType::Custom_GCode: { color = Options_Colors[static_cast<unsigned int>(EOptionsColors::CustomGCodes)]; break; }
            case EMoveType::Retract: { color = Options_Colors[static_cast<unsigned int>(EOptionsColors::Retractions)]; break; }
            case EMoveType::Unretract: { color = Options_Colors[static_cast<unsigned int>(EOptionsColors::Unretractions)]; break; }
            case EMoveType::Extrude: {
                if (!top_layer_only ||
                        m_sequential_view.current.last == global_endpoints.last ||
                        is_in_layers_range(path, m_layers_z_range[1], m_layers_z_range[1]))
                    color = extrusion_color(path);
                else{
                    color = { 0.65f, 0.65f, 0.65f };
                    is_render_cap = 1.0;
                }
                break;
            }
            case EMoveType::Travel: {
                if (!top_layer_only || m_sequential_view.current.last == global_endpoints.last || is_travel_in_layers_range(path_id, m_layers_z_range[1], m_layers_z_range[1]))
                    color = (m_view_type == EViewType::Feedrate || m_view_type == EViewType::Tool || m_view_type == EViewType::ColorPrint) ? extrusion_color(path) : travel_color(path);
                else
                    color = { 0.25f, 0.25f, 0.25f };

                break;
            }
            case EMoveType::Wipe: { color = Wipe_Color; break; }
            default: { color = { 0.0f, 0.0f, 0.0f }; break; }
            }

#if ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
            RenderPath key{ tbuffer_id, color, is_render_cap, static_cast<unsigned int>(ibuffer_id), path_id };
            if (render_path == nullptr || !RenderPathPropertyEqual()(*render_path, key))
                render_path = const_cast<RenderPath*>(&(*buffer.render_paths.emplace(key).first));

            unsigned int delta_1st = 0;
            if (sub_path.first.s_id < m_sequential_view.current.first && m_sequential_view.current.first <= sub_path.last.s_id)
                delta_1st = static_cast<unsigned int>(m_sequential_view.current.first - sub_path.first.s_id);
#else
            RenderPath key{ color, static_cast<unsigned int>(ibuffer_id), path_id };
            if (render_path == nullptr || !RenderPathPropertyEqual()(*render_path, key))
                render_path = const_cast<RenderPath*>(&(*buffer->render_paths.emplace(key).first));
#endif // ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS

            unsigned int size_in_indices = 0;
#if ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
            switch (buffer.render_primitive_type)
#else
            switch (buffer->render_primitive_type)
#endif // ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
            {
            case TBuffer::ERenderPrimitiveType::Point: {
#if ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
                size_in_indices = buffer.indices_per_segment();
#else
                size_in_indices = buffer->indices_per_segment();
#endif // ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
                break;
            }
            case TBuffer::ERenderPrimitiveType::AnkerMove:
            case TBuffer::ERenderPrimitiveType::Line:
            case TBuffer::ERenderPrimitiveType::Triangle: {
                unsigned int segments_count = std::min(m_sequential_view.current.last, sub_path.last.s_id) - std::max(m_sequential_view.current.first, sub_path.first.s_id);
#if ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
                size_in_indices = buffer.indices_per_segment() * segments_count;
#else
                size_in_indices = buffer->indices_per_segment() * segments_count;
#endif // ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
                break;
            }
            }

#if ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
            if (size_in_indices == 0)
                continue;

            if (buffer.render_primitive_type == TBuffer::ERenderPrimitiveType::Triangle) {
                if (sub_path_id == 0 && delta_1st == 0)
                    size_in_indices += 6; // add 2 triangles for starting cap
                if (sub_path_id == path.sub_paths.size() - 1 && path.sub_paths.back().last.s_id <= m_sequential_view.current.last)
                    size_in_indices += 6; // add 2 triangles for ending cap
                if (delta_1st > 0)
                    size_in_indices -= 6; // remove 2 triangles for corner cap
            }
#endif // ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS

            render_path->sizes.push_back(size_in_indices);

#if !ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
            unsigned int delta_1st = 0;
            if (sub_path.first.s_id < m_sequential_view.current.first && m_sequential_view.current.first <= sub_path.last.s_id)
                delta_1st = m_sequential_view.current.first - sub_path.first.s_id;
#endif // !ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS

#if ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
            if (buffer.render_primitive_type == TBuffer::ERenderPrimitiveType::Triangle) {
                delta_1st *= buffer.indices_per_segment();
                if (delta_1st > 0) {
                    delta_1st += 6; // skip 2 triangles for corner cap
                    if (sub_path_id == 0)
                        delta_1st += 6; // skip 2 triangles for starting cap
                }
            }
#else
            if (buffer->render_primitive_type == TBuffer::ERenderPrimitiveType::Triangle)
                delta_1st *= buffer->indices_per_segment();
#endif // ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS

            render_path->offsets.push_back(static_cast<size_t>((sub_path.first.i_id + delta_1st) * sizeof(IBufferType)));

#if 0
            // check sizes and offsets against index buffer size on gpu
            GLint buffer_size;
            glsafe(::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->indices[render_path->ibuffer_id].ibo));
            glsafe(::glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &buffer_size));
            glsafe(::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
            if (render_path->offsets.back() + render_path->sizes.back() * sizeof(IBufferType) > buffer_size)
                BOOST_LOG_TRIVIAL(error) << "GCodeViewer::refresh_render_paths: Invalid render path data";
#endif
        }

//AkUtil::TDebug( "3");
        // set sequential data to their final value
        sequential_view->endpoints = top_layer_only ? top_layer_endpoints : global_endpoints;
        sequential_view->current.first = !top_layer_only && keep_sequential_current_first ? std::clamp(sequential_view->current.first, sequential_view->endpoints.first, sequential_view->endpoints.last) : sequential_view->endpoints.first;

#if ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
        // updates sequential range caps
        std::array<SequentialRangeCap, 2>* sequential_range_caps = const_cast<std::array<SequentialRangeCap, 2>*>(&m_sequential_range_caps);
        
        if ((*sequential_range_caps)[0].ibo > 0)
        {
            glsafe(glDeleteBuffers(1, &(*sequential_range_caps)[0].ibo));
        }
        (*sequential_range_caps)[0].reset();
        if ((*sequential_range_caps)[1].ibo > 0)
        {
            glsafe(glDeleteBuffers(1, &(*sequential_range_caps)[0].ibo));
        }
        (*sequential_range_caps)[1].reset();
//AkUtil::TDebug( "4");
        if (m_sequential_view.current.first != m_sequential_view.current.last) {
            for (const auto& [tbuffer_id, ibuffer_id, path_id, sub_path_id] : paths) {
                TBuffer& buffer = const_cast<TBuffer&>(m_buffers[tbuffer_id]);
                if (buffer.render_primitive_type != TBuffer::ERenderPrimitiveType::Triangle)
                    continue;

                const Path& path = buffer.paths[path_id];
                const Path::Sub_Path& sub_path = path.sub_paths[sub_path_id];
                if (m_sequential_view.current.last <= sub_path.first.s_id || sub_path.last.s_id <= m_sequential_view.current.first)
                    continue;

                // update cap for first endpoint of current range
                if (m_sequential_view.current.first > sub_path.first.s_id) {
                    SequentialRangeCap& cap = (*sequential_range_caps)[0];

                    const IBuffer& i_buffer = buffer.indices[ibuffer_id];
                    cap.buffer = &buffer;
                    cap.vbo = i_buffer.vbo;

                    // calculate offset into the index buffer
                    unsigned int offset = sub_path.first.i_id;
                    offset += 6; // add 2 triangles for corner cap
                    offset += static_cast<unsigned int>(m_sequential_view.current.first - sub_path.first.s_id) * buffer.indices_per_segment();
                    if (sub_path_id == 0)
                        offset += 6; // add 2 triangles for starting cap

                    // extract indices from index buffer
                    std::array<IBufferType, 6> indices{ 0, 0, 0, 0, 0, 0 };
                    //TODO:

                    glsafe(glBindVertexArray(buffer.t_vao));

                    glsafe(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer.ibo));
                    glsafe(glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLintptr>((offset + 0) * sizeof(IBufferType)), static_cast<GLsizeiptr>(sizeof(IBufferType)), static_cast<void*>(&indices[0])));
                    glsafe(glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLintptr>((offset + 7) * sizeof(IBufferType)), static_cast<GLsizeiptr>(sizeof(IBufferType)), static_cast<void*>(&indices[1])));
                    glsafe(glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLintptr>((offset + 1) * sizeof(IBufferType)), static_cast<GLsizeiptr>(sizeof(IBufferType)), static_cast<void*>(&indices[2])));
                    glsafe(glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLintptr>((offset + 13) * sizeof(IBufferType)), static_cast<GLsizeiptr>(sizeof(IBufferType)), static_cast<void*>(&indices[4])));
                    glsafe(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
                    indices[3] = indices[0];
                    indices[5] = indices[1];

                    glsafe(glBindVertexArray(0));

                    // send indices to gpu
                    // TODO:

                    glsafe(glGenVertexArrays(1, &cap.vao));
                    glsafe(glBindVertexArray(cap.vao));

                    glsafe(glGenBuffers(1, &cap.ibo));
                    glsafe(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cap.ibo));
                    glsafe(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(IBufferType), indices.data(), GL_STATIC_DRAW));
                    glsafe(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

                    glsafe(glBindVertexArray(0));

                    // extract color from render path
                    size_t offset_bytes = offset * sizeof(IBufferType);
                    for (const RenderPath& render_path : buffer.render_paths) {
                        if (render_path.ibuffer_id == ibuffer_id) {
                            for (size_t j = 0; j < render_path.offsets.size(); ++j) {
                                if (render_path.contains(offset_bytes)) {
                                    cap.color = render_path.color;
                                    break;
                                }
                            }
                        }
                    }
                }

                // update cap for last endpoint of current range
                if (m_sequential_view.current.last < sub_path.last.s_id) {
                    SequentialRangeCap& cap = (*sequential_range_caps)[1];
                    const IBuffer& i_buffer = buffer.indices[ibuffer_id];
                    cap.buffer = &buffer;
                    cap.vbo = i_buffer.vbo;

                    // calculate offset into the index buffer
                    unsigned int offset = sub_path.first.i_id;
                    offset += 6; // add 2 triangles for corner cap
                    offset += static_cast<unsigned int>(m_sequential_view.current.last - 1 - sub_path.first.s_id) * buffer.indices_per_segment();
                    if (sub_path_id == 0)
                        offset += 6; // add 2 triangles for starting cap

                    // extract indices from index buffer
                    std::array<IBufferType, 6> indices{ 0, 0, 0, 0, 0, 0 };
                    //TODO:

                    glsafe(glBindVertexArray(buffer.t_vao));

                    glsafe(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer.ibo));
                    glsafe(glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLintptr>((offset + 2) * sizeof(IBufferType)), static_cast<GLsizeiptr>(sizeof(IBufferType)), static_cast<void*>(&indices[0])));
                    glsafe(glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLintptr>((offset + 4) * sizeof(IBufferType)), static_cast<GLsizeiptr>(sizeof(IBufferType)), static_cast<void*>(&indices[1])));
                    glsafe(glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLintptr>((offset + 10) * sizeof(IBufferType)), static_cast<GLsizeiptr>(sizeof(IBufferType)), static_cast<void*>(&indices[2])));
                    glsafe(glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLintptr>((offset + 16) * sizeof(IBufferType)), static_cast<GLsizeiptr>(sizeof(IBufferType)), static_cast<void*>(&indices[5])));
                    glsafe(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
                    indices[3] = indices[0];
                    indices[4] = indices[2];

                    glsafe(glBindVertexArray(0));

                    // send indices to gpu
                    //TODO:

                    glsafe(glGenVertexArrays(1, &cap.vao));
                    glsafe(glBindVertexArray(cap.vao));

                    glsafe(glGenBuffers(1, &cap.ibo));
                    glsafe(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cap.ibo));
                    glsafe(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(IBufferType), indices.data(), GL_STATIC_DRAW));
                    glsafe(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

                    glsafe(glBindVertexArray(0));

                    // extract color from render path
                    size_t offset_bytes = offset * sizeof(IBufferType);
                    for (const RenderPath& render_path : buffer.render_paths) {
                        if (render_path.ibuffer_id == ibuffer_id) {
                            for (size_t j = 0; j < render_path.offsets.size(); ++j) {
                                if (render_path.contains(offset_bytes)) {
                                    cap.color = render_path.color;
                                    break;
                                }
                            }
                        }
                    }
                }

                if ((*sequential_range_caps)[0].is_renderable() && (*sequential_range_caps)[1].is_renderable())
                    break;
            }
        }
#endif // ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS


#if ENABLE_GCODE_VIEWER_STATISTICS
        for (const TBuffer& buffer : m_buffers) {
            statistics->render_paths_size += SLIC3R_STDUNORDEREDSET_MEMSIZE(buffer.render_paths, RenderPath);
            for (const RenderPath& path : buffer.render_paths) {
                statistics->render_paths_size += SLIC3R_STDVEC_MEMSIZE(path.sizes, unsigned int);
                statistics->render_paths_size += SLIC3R_STDVEC_MEMSIZE(path.offsets, size_t);
            }
        }
        statistics->refresh_paths_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_time).count();
#endif // ENABLE_GCODE_VIEWER_STATISTICS
    }

    void GcodeViewer::TBuffer::reset()
    {
    // release gpu memory
    vertices.reset();
    for (IBuffer& buffer : indices) {
    buffer.reset();
}

    // release cpu memory
    indices.clear();
    paths.clear();
    vector<IBuffer>().swap(indices);
    vector<Path>().swap(paths);
    render_paths.clear();
}

    void GcodeViewer::TBuffer::add_path(const GCodeProcessor::MoveVertex & move, unsigned int b_id, size_t i_id, size_t s_id)
    {
    Path::Endpoint endpoint = { b_id, i_id, s_id, move.position };
    // use rounding to reduce the number of generated paths
#if ENABLE_SPLITTED_VERTEX_BUFFER
#if ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE
    paths.push_back({ move.type, move.extrusion_role, move.delta_extruder,
                      m_round_to_nearest(move.height, 2), m_round_to_nearest(move.width, 2), move.feedrate, move.fan_speed,
                      move.volumetric_rate(), move.extruder_id, move.cp_color_id, { { endpoint, endpoint } }, move.layer_duration, move.time, move.temperature });
#else
    paths.push_back({ move.type, move.extrusion_role, move.delta_extruder,
                      m_round_to_nearest(move.height, 2), m_round_to_nearest(move.width, 2), move.feedrate, move.fan_speed,
                      m_round_to_nearest(move.volumetric_rate(), 2), move.extruder_id, move.cp_color_id, { { endpoint, endpoint } }, move.layer_duration, move.time, move.temperature });
#endif // ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE
#else
#if ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE
    paths.push_back({ move.type, move.extrusion_role, endpoint, endpoint, move.delta_extruder,
                      m_round_to_nearest(move.height, 2), m_round_to_nearest(move.width, 2), move.feedrate, move.fan_speed,
                      move.volumetric_rate(), move.extruder_id, move.cp_color_id, move.layer_duration, move.time, move.temperature });
#else
    paths.push_back({ move.type, move.extrusion_role, endpoint, endpoint, move.delta_extruder,
                      m_round_to_nearest(move.height, 2), m_round_to_nearest(move.width, 2), move.feedrate, move.fan_speed,
                      m_round_to_nearest(move.volumetric_rate(), 2), move.extruder_id, move.cp_color_id, move.layer_duration, move.time, move.temperature });
#endif // ENABLE_TOOLPATHS_WIDTH_HEIGHT_FROM_GCODE
#endif // ENABLE_SPLITTED_VERTEX_BUFFER

}

    void GcodeViewer::IBuffer::reset()
    {
#if ENABLE_SPLITTED_VERTEX_BUFFER
    // release gpu memory
    if (ibo > 0) {
    //glsafe(glDeleteBuffers(1, &ibo));
    ibo = 0;
}
#else
    // release gpu memory
    if (id > 0) {
    glsafe(::glDeleteBuffers(1, &id));
    id = 0;
}
#endif // ENABLE_SPLITTED_VERTEX_BUFFER

#if ENABLE_SPLITTED_VERTEX_BUFFER
    vbo = 0;
#endif // ENABLE_SPLITTED_VERTEX_BUFFER
    count = 0;
}

    void GcodeViewer::VBuffer::reset()
    {
    // release gpu memory
    if (!vbos.empty()) {
    //glsafe(glDeleteBuffers(static_cast<GLsizei>(vbos.size()), static_cast<const GLuint*>(vbos.data())));
    vbos.clear();
}
    sizes.clear();
    count = 0;
}

    float GcodeViewer::Extrusions::Range::step_size(bool log) const
    {
    if (log)
    {
    float min_range = min;
    if (min_range == 0)
    min_range = 0.001f;
    return (std::log(max / min_range) / (static_cast<float>(Range_Colors.size()) - 1.0f));
}
    else
    return (max - min) / (static_cast<float>(Range_Colors.size()) - 1.0f);
}

    GcodeViewer::Color GcodeViewer::Extrusions::Range::get_color_at(float value, bool log) const
    {
    // Input value scaled to the colors range
    const float step = step_size(log);
    float global_t;
    if (log)
    {
    float min_range = min;
    if (min_range == 0)
    min_range = 0.001f;
    global_t = (step != 0.0f) ? std::max(0.0f, std::log(value / min_range)) / step : 0.0f; // lower limit of 0.0f
}
    else
    global_t = (step != 0.0f) ? std::max(0.0f, value - min) / step : 0.0f; // lower limit of 0.0f

    const size_t color_max_idx = Range_Colors.size() - 1;

    // Compute the two colors just below (low) and above (high) the input value
    const size_t color_low_idx = std::clamp<size_t>(static_cast<size_t>(global_t), 0, color_max_idx);
    const size_t color_high_idx = std::clamp<size_t>(color_low_idx + 1, 0, color_max_idx);

    // Compute how far the value is between the low and high colors so that they can be interpolated
    const float local_t = std::clamp(global_t - static_cast<float>(color_low_idx), 0.0f, 1.0f);

    // Interpolate between the low and high colors to find exactly which color the input value should get
    Color ret;
    for (unsigned int i = 0; i < 3; ++i) {
        ret[i] = lerp(Range_Colors[color_low_idx][i], Range_Colors[color_high_idx][i], local_t);
    }
    return ret;
}

#if ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
GcodeViewer::SequentialRangeCap::~SequentialRangeCap() {
    if (ibo > 0)
        //TODO: glsafe(::glDeleteBuffers(1, &ibo));
        return;
}

void GcodeViewer::SequentialRangeCap::reset() {
    if (ibo > 0)
        //TODO: glsafe(glDeleteBuffers(1, &ibo));
        ;

    buffer = nullptr;
    ibo = 0;
    vbo = 0;
    color = { 0.0f, 0.0f, 0.0f };
}
#endif

void GcodeViewer::set_toolpath_move_type_visible(EMoveType type, bool visible)
{
    size_t id = static_cast<size_t>(buffer_id(type));
    if (id < m_buffers.size())
        m_buffers[id].visible = visible;
}



void GcodeViewer::offerContext()
{
    
    AkUtil::TFunction("");
    m_offscreenSurface = new QOffscreenSurface();
    m_offscreenSurface->setFormat(format());//this->context()->format());
    m_offscreenSurface->create();
    if (!cnt)
    {
        cnt = new QOpenGLContext(this);
        cnt->setFormat(format());

        if (cnt->create())
        {
            cnt->makeCurrent(m_offscreenSurface);

        }
        else
        {
            delete cnt;
            cnt = nullptr;
        }
    }

    //this->makeCurrent();
    this->initializeOpenGLFunctions();
    m_scene3d = new Scene3D(this, m_sceneParam);
    for (size_t i = 0; i < m_buffers.size(); ++i) {
        TBuffer& buffer = const_cast<TBuffer&>(m_buffers[i]);
        switch (buffer_type(i))
        {
        default: { break; }
        case EMoveType::Tool_change:
        case EMoveType::Color_change:
        case EMoveType::Pause_Print:
        case EMoveType::Custom_GCode:
        case EMoveType::Retract:
        case EMoveType::Unretract: {
            buffer.shader = "options_120";
            break;
        }
        case EMoveType::Wipe:
        case EMoveType::Extrude: {
            buffer.shader = "gouraud_light";
            break;
        }
        case EMoveType::Travel: {
            buffer.shader = "toolpaths_lines";
            break;
        }
        }
    }

    bool success;
    success = shaderProgram_tr.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/gouraud_light_tr.vs");
    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << shaderProgram_tr.log();
        return;
    }
    
    if(isHighPerformance){
        success = shaderProgram_tr.addShaderFromSourceFile(QOpenGLShader::Geometry, ":/shaders/gouraud_light_tr.gs");
        if (!success) {
            qDebug() << "shaderProgram addShaderFromSourceFile failed!" << shaderProgram_tr.log();
            return;
        }
    }else{
        success = shaderProgram_tr.addShaderFromSourceFile(QOpenGLShader::Geometry, ":/shaders/gouraud_light_tr_perfermance.gs");
        if (!success) {
            qDebug() << "shaderProgram addShaderFromSourceFile failed!" << shaderProgram_tr.log();
            return;
        }
    }
    success = shaderProgram_tr.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/gouraud_light_tr.fs");
    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << shaderProgram_tr.log();
        return;
    }

    success = shaderProgram_tr.link();
    if (!success) {
        qDebug() << "shaderProgram link failed!" << shaderProgram_tr.log();
    }

    this->load_toolpaths(this->gcode_result);
    //    if (this->m_layers.get_zs().size() < 1)
    //    {
    //        QMessageBox msgBox;
    //        msgBox.setText(tr("gcode file error : invalid gcode context"));
    //        msgBox.exec();
    //        return;
    //    }

    std::vector<std::string> colors;
    colors.push_back("#808080");
    this->refresh(gcode_result, colors);

    glsafe(glEnable(GL_DEPTH_TEST));
    this->doneCurrent();
}


void GcodeViewer::initializeGL()
{
    AkUtil::TFunction("");
    AkUtil::TDebug("in initializeGL" );
    qDebug() << " flow_range: " <<this->gcode_result.flow_range ;
    //grabKeyboard();
    this->initializeOpenGLFunctions();


    // initializes opengl data of TBuffers
    for (size_t i = 0; i < m_buffers.size(); ++i) {
        TBuffer& buffer = const_cast<TBuffer&>(m_buffers[i]);
        switch (buffer_type(i))
        {
        default: { break; }
        case EMoveType::Tool_change:
        case EMoveType::Color_change:
        case EMoveType::Pause_Print:
        case EMoveType::Custom_GCode:
        case EMoveType::Retract:
        case EMoveType::Unretract: {
            buffer.shader = "options_120";
            break;
        }
        case EMoveType::Wipe:
        case EMoveType::Extrude: {
            buffer.shader = "gouraud_light";
            break;
        }
        case EMoveType::Travel: {
            buffer.shader = "toolpaths_lines";
            break;
        }
        case EMoveType::Zlap: {
            buffer.shader = "toolpaths_point";
            break;
        }
        }
    }


    bool success = shaderProgram1.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/gouraud_light.vs");
    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << shaderProgram1.log();
        AkUtil::TDebug( QString("shaderProgram1 addShaderFromSourceFile failed!"));
        return;
    }
    //success = shaderProgram1.addShaderFromSourceFile()
    //    success = shaderProgram1.addShaderFromSourceFile(QOpenGLShader::Geometry, ":/shaders/gouraud_light.gs");
    //    if (!success) {
    //        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << shaderProgram1.log();
    //        return;
    //    }

    success = shaderProgram1.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/gouraud_light.fs");
    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << shaderProgram1.log();
        AkUtil::TDebug( QString("shaderProgram1 addShaderFromSourceFile failed!"));
        return;
    }

    success = shaderProgram1.link();
    if (!success) {
        qDebug() << "shaderProgram link failed!" << shaderProgram1.log();
        AkUtil::TDebug( QString("shaderProgram link failed!"));
    }

    success = shaderProgram_tr.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/gouraud_light_tr.vs");
    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << shaderProgram_tr.log();
        return;
    }
    
    if(isHighPerformance){
        success = shaderProgram_tr.addShaderFromSourceFile(QOpenGLShader::Geometry, ":/shaders/gouraud_light_tr.gs");
        if (!success) {
            qDebug() << "shaderProgram addShaderFromSourceFile failed!" << shaderProgram_tr.log();
            return;
        }
    }else{
        success = shaderProgram_tr.addShaderFromSourceFile(QOpenGLShader::Geometry, ":/shaders/gouraud_light_tr_perfermance.gs");
        if (!success) {
            qDebug() << "shaderProgram addShaderFromSourceFile failed!" << shaderProgram_tr.log();
            return;
        }
    }
    success = shaderProgram_tr.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/gouraud_light_tr.fs");
    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << shaderProgram_tr.log();
        return;
    }

    success = shaderProgram_tr.link();
    if (!success) {
        qDebug() << "shaderProgram link failed!" << shaderProgram_tr.log();
    }

    this->m_shader.insert(std::pair<std::string, int>("gouraud_light", shaderProgram1.programId()));

    success = shaderProgram2.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/toolpaths_lines.vs");
    if (!success) {
        qDebug() << "shaderProgram2 addShaderFromSourceFile failed!" << shaderProgram2.log();
        AkUtil::TDebug( QString("shaderProgram2 addShaderFromSourceFile failed!"));
        return;
    }

    success = shaderProgram2.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/toolpaths_lines.fs");
    if (!success) {
        qDebug() << "shaderProgram2 addShaderFromSourceFile failed!" << shaderProgram2.log();
        AkUtil::TDebug( QString("shaderProgram2 addShaderFromSourceFile failed!"));
        return;
    }

    success = shaderProgram2.link();
    if (!success) {
        qDebug() << "shaderProgram link failed!" << shaderProgram2.log();
        AkUtil::TDebug( QString("shaderProgram link failed!"));
    }

    this->m_shader.insert(std::pair<std::string, int>("toolpaths_lines", shaderProgram2.programId()));

    success = shaderProgram_or.addShaderFromSourceFile(QOpenGLShader::Vertex,":/shaders/gouraud_light_or.vs");
    if (!success) {
        qDebug() << "shaderProgramor addShaderFromSourceFile failed!" << shaderProgram_or.log();
        return;
    }

    success = shaderProgram_or.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/gouraud_light_or.fs");
    if (!success) {
        qDebug() << "shaderProgram2 addShaderFromSourceFile failed!" << shaderProgram_or.log();
        return;
    }

    success = shaderProgram_or.link();
    if (!success) {
        qDebug() << " shaderProgram_or link failed!" << shaderProgram_or.log();
    }

    //TODO!!!Add By Aden Hu
    success = shaderProgram3.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/scene_view.vert");
    if (!success) {
        qDebug() << "shaderProgram3 addShaderFromSourceFile failed!" << shaderProgram3.log();
        return;
    }

    const_cast<SequentialView*>(&m_sequential_view)->marker.init();
    success = shaderProgram3.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/scene_view.frag");
    if (!success) {
        qDebug() << "shaderProgram3 addShaderFromSourceFile failed!" << shaderProgram3.log();
        return;
    }

    success = shaderProgram3.link();
    if (!success) {
        qDebug() << "shaderProgram3 link failed!" << shaderProgram3.log();
    }


    success = shaderProgram_point.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/options_120.vs");
    if (!success) {
        qDebug() << "shaderProgram_point addShaderFromSourceFile failed!" << shaderProgram_point.log();
        AkUtil::TDebug( QString("shaderProgram_point addShaderFromSourceFile failed!"));
        return;
    }

    success = shaderProgram_point.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/options_120.fs");
    if (!success) {
        qDebug() << "shaderProgram_point addShaderFromSourceFile failed!" << shaderProgram_point.log();
        AkUtil::TDebug( QString("shaderProgram_point addShaderFromSourceFile failed!"));
        return;
    }

    success = shaderProgram_point.link();
    if (!success) {
        qDebug() << "shaderProgram link failed!" << shaderProgram_point.log();
        AkUtil::TDebug( QString("shaderProgram link failed!"));
    }


    glsafe(glEnable(GL_DEPTH_TEST));
    glsafe(glEnable(GL_CULL_FACE));
#ifdef FORCE_ARM
    glsafe(glCullFace(GL_BACK));
#else
    glsafe(glCullFace(GL_FRONT));
#endif

        if(!printMode && !innerMode)
        {
            mPdlg = new ProgressDialog(nullptr);
            IgnoreEvent ign(this);
            mPdlg->setText(tr("Load G-Code"));
            mPdlg->setCancelVisible(false);
            QObject::connect(this,SIGNAL(setValue(int)),mPdlg,SLOT(setValue(int)));
            mPdlg->exec();
        }

}

//void GcodeViewer::showProgressDlg()
//{
//    ProgressDialog progressDlg(nullptr);
//    progressDlg.setText(tr("gen ai pic"));
//    progressDlg.setCancelVisible(false);
//    connect( this,SIGNAL(setValue(int)),&progressDlg,SLOT(setValue(int)));
//    progressDlg.exec();
//}


void GcodeViewer::exportOffRender(const QString& savePath,bool isAiMode,bool cancel)
{
    this->off_render(savePath,isAiMode);
}

void GcodeViewer::resizeGL(int w, int h)
{
    //Add by Aden Hu
    if (m_scene3d == NULL)
        return;
    m_scene3d->resizeWidget(w, h);
    glViewport(0, 0, w, h);
}

void GcodeViewer::getSceneParam()
{

}

void GcodeViewer::setView(const ViewType & type)
{
    if (m_scene3d)
    {
        m_scene3d->setView(type);
        update();
    }
}

void GcodeViewer::setPerformance(bool setValue)
{
    this->isHighPerformance = setValue;
}

void GcodeViewer::setRoleColor(unsigned int index,std::array<float, 3> setColor)
{
    this->Extrusion_Role_Colors[index] = setColor;
}


void GcodeViewer::setTravelColor(unsigned int index,std::array<float, 3> setColor)
{
    this->Travel_Colors[index] = setColor;
}

void GcodeViewer::loadGcode()
{
    if (cnt)
    {
        cnt->makeCurrent(m_offscreenSurface);
    }
    else {
        this->makeCurrent();
    }
    this->load_toolpaths(this->gcode_result);
    std::vector<std::string> colors;
    colors.push_back("#808080");
    this->refresh(gcode_result, colors, true);
    if(!this->printMode)
    {
        offImage = this->render_singe_iamge();
        if(m_fbo)
        {
            m_fbo->release();
            delete m_fbo;
            m_fbo = nullptr;
        }
        QThreadPool::globalInstance()->start([=](){
            
            off_render_single(offImage);
            emit setValue(100);
        });
    }else{
        emit setValue(100);
    }

    if (cnt)
    {
        cnt->doneCurrent();
    }
    else {
        this->doneCurrent();
    }

}

void GcodeViewer::setSceneParams(const SceneParam & param)
{
    m_sceneParam = param;
    if (m_scene3d)
    {
        m_scene3d->setSceneParams(param);
        update();
    }else
    {
        //scene_part
        this->makeCurrent();
        
        m_scene3d_init_mutex.lock();
        AkUtil::TDebug("m_scene3d_init_mutex.lock() in setSceneParams");
        m_scene3d = new Scene3D(this, m_sceneParam);
        m_renderData = RenderDataPtr(new RenderData);
        m_renderData->m_printMachineBoxPtr->create(m_sceneParam.logoMesh,
                                                   m_sceneParam.m_printMachineBox.m_length, m_sceneParam.m_printMachineBox.m_width, m_sceneParam.m_printMachineBox.m_height,
                                                   m_sceneParam.m_printMachineBox.m_color);
        m_renderData->m_printMachineBoxPtr->setColor(m_sceneParam.m_printMachineBox.m_color);
        m_renderData->setShaderProgram(&shaderProgram3);
        m_renderData->bindPrintMachineBoxRenderData();
        for (auto meshIt = m_renderData->m_allmeshes.begin(); meshIt != m_renderData->m_allmeshes.end(); meshIt++)
        {
            writeToGPUBUffer(meshIt->first, meshIt->second);
        }

        for (auto curveIt = m_renderData->m_allcurves.begin(); curveIt != m_renderData->m_allcurves.end(); curveIt++)
        {
            writeToGPUBUffer(curveIt->first, curveIt->second);
        }
        Vec3f boxCenter = (m_boundingBox.max + m_boundingBox.min) / 2.0;
        m_scene3d->setRefTranPoint(boxCenter.x(), boxCenter.y(), boxCenter.z());
        this->load_toolpaths(this->gcode_result);
        std::vector<std::string> colors;
        colors.push_back("#808080");
        this->refresh(gcode_result, colors);
        //update 20221103: whatever printMode or innerMode ,there is no need to emit the setValue 100 signal

        
        m_scene3d_inited.wakeAll();
        AkUtil::TDebug("waked!");
        m_scene3d_init_mutex.unlock();
        AkUtil::TDebug("m_scene3d_init_mutex.unlock() in setSceneParams");
        this->doneCurrent();
    }
}


void GcodeViewer::writeToGPUBUffer(CHMeshShowObjPtr mesh, RenderMeshPtr renderMesh)
{
    writeToGPUBUffer(mesh, renderMesh->m_vbo, renderMesh->m_norbo, renderMesh->m_vao, renderMesh->m_nIBOId, renderMesh->m_fNum);
}

void GcodeViewer::writeToGPUBUffer(CHMeshShowObjPtr mesh, QOpenGLBuffer & outvbo, QOpenGLBuffer & outnorbo,
                                   QOpenGLVertexArrayObject & outvao, GLuint & outibo, int& fnum, bool normalFlip)
{
    if(mesh == nullptr)
    {
        return;
    }
    int verNum = mesh->m_vertices.size();
    float* vertices = reinterpret_cast<float*>(mesh->m_vertices.data());
    float* nors = reinterpret_cast<float*>(mesh->m_nors.data());
    fnum = mesh->m_trians.size();
    int* tri = reinterpret_cast<int*>(mesh->m_trians.data());

    QOpenGLVertexArrayObject::Binder vaoBind(&outvao);//RAII
    outvbo.create();
    outvbo.bind();
    outvbo.allocate(vertices, sizeof(GLfloat) * verNum * 3);
    int attr = -1;
    attr = m_renderData->m_shaderProgramPtr->attributeLocation("aPos");
    m_renderData->m_shaderProgramPtr->setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(GLfloat) * 3);
    m_renderData->m_shaderProgramPtr->enableAttributeArray(attr);
    outvbo.release();

    outnorbo.create();
    outnorbo.bind();
    outnorbo.allocate(nors, sizeof(GLfloat) * verNum * 3);
    attr = m_renderData->m_shaderProgramPtr->attributeLocation("aNormal");
    m_renderData->m_shaderProgramPtr->setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(GLfloat) * 3);
    m_renderData->m_shaderProgramPtr->enableAttributeArray(attr);
    if (normalFlip)
    {
        float* nordata = (float*)outnorbo.map(QOpenGLBuffer::Access::ReadWrite);
        for (int i = 0; i < verNum * 3; i++)
        {
            nordata[i] = -nordata[i];
        }
        outnorbo.unmap();
    }
    outnorbo.release();

    
    glGenBuffers(1, &outibo);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, outibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * fnum * 3, tri, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GcodeViewer::writeToGPUBUffer(CHCurveShowObjPtr curvebody, RenderCurvePtr rendercurve)
{
    if(curvebody == nullptr)
    {
        return;
    }
    std::vector<QVector3D>& points = curvebody->getShowPoints();
    rendercurve->m_Num = (int)points.size();
    GLfloat* vertices = new GLfloat[rendercurve->m_Num * 3];
    for (int i = 0; i < rendercurve->m_Num; i++)
    {
        vertices[3 * i] = points[i].x();
        vertices[3 * i + 1] = points[i].y();
        vertices[3 * i + 2] = points[i].z();
    }

    QOpenGLVertexArrayObject::Binder vaoBind(&(rendercurve->m_vao));
    rendercurve->m_vbo.create();
    rendercurve->m_vbo.bind();
    rendercurve->m_vbo.allocate(vertices, sizeof(GLfloat) * rendercurve->m_Num * 3);
    
    rendercurve->m_vbo.release();

    delete vertices;
}

void GcodeViewer::paintMeshVbo(QOpenGLShaderProgram & shaderProgram, CHMeshShowObjPtr mesh, RenderMeshPtr rm, QMatrix4x4 & pvMatrix)
{
    
    QColor color = mesh->getColor();
    shaderProgram.setUniformValue("objColor", QVector3D((float)color.red() / 255.0,
                                                        (float)color.green() / 255.0, (float)color.blue() / 255.0));

    
    QMatrix4x4 mMatrix = mesh->getTransform();
    shaderProgram.setUniformValue("mMatrix", mMatrix);
    shaderProgram.setUniformValue("pvmMatrix", pvMatrix * mMatrix);
    shaderProgram.setUniformValue("norTranMatrix", mMatrix.normalMatrix());

    QOpenGLVertexArrayObject::Binder vaoBind(&(rm->m_vao));
    //glDrawArrays(GL_TRIANGLES, 0, 6);

    shaderProgram.setUniformValue("calLight", mesh->getCalLight());
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rm->m_nIBOId);
    glDrawElements(GL_TRIANGLES, rm->m_fNum * 3, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void GcodeViewer::paintCurveVbo(QOpenGLShaderProgram & shaderProgram, CHCurveShowObjPtr curve, RenderCurvePtr rc, QMatrix4x4 & pvMatrix)
{
    if(curve == nullptr || rc == nullptr)
    {
        return;
    }
    
    QColor color = /*m_sceneParam.m_printMachineBox.m_color;*/curve->getColor();
    shaderProgram.setUniformValue("objColor", QVector3D((float)color.red() / 255.0,
                                                        (float)color.green() / 255.0, (float)color.blue() / 255.0));

    
    QMatrix4x4 mMatrix = curve->getTransform();
    shaderProgram.setUniformValue("mMatrix", mMatrix);
    shaderProgram.setUniformValue("pvmMatrix", pvMatrix * mMatrix);
    shaderProgram.setUniformValue("norTranMatrix", mMatrix.normalMatrix());

    //glLineWidth(curve->getLineWide());
    QOpenGLVertexArrayObject::Binder vaoBind(&(rc->m_vao));
    
    rc->m_vbo.bind();
    int attr = -1;
    attr = m_renderData->m_shaderProgramPtr->attributeLocation("aPos");
    m_renderData->m_shaderProgramPtr->setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(GLfloat) * 3);
    m_renderData->m_shaderProgramPtr->enableAttributeArray(attr);
    glsafe(glDrawArrays(GL_LINE_STRIP, 0, rc->m_Num));
    rc->m_vbo.release();
}

void GcodeViewer::paintPointVbo(QOpenGLShaderProgram & shaderProgram, CHPointShowObjPtr point, RenderPointSpherePtr rps, QMatrix4x4 & pvMatrix)
{
    shaderProgram.setUniformValue("calLight", point->getCalLight());

    
    QColor color = point->getColor();
    if (point->getStatus() == canPicked || point->getStatus() == selected)
    {
        color = canPickedColor;
    }
    shaderProgram.setUniformValue("objColor", QVector3D((float)color.red() / 255.0,
                                                        (float)color.green() / 255.0, (float)color.blue() / 255.0));

    
    QMatrix4x4 mMatrix;
    QVector3D newCoord = m_scene3d->pRot(point->getTransform(), point->m_coord);
    mMatrix.translate(newCoord);
    if (true)
    {
        QMatrix4x4 sMatrix;
        sMatrix.scale(QVector3D::dotProduct(newCoord - m_scene3d->getEye(), m_scene3d->getFront()) * tan(m_scene3d->getVerticalAngle() / 360.0 * CH_PI)
                      / height() * 2 * point->m_size);
        mMatrix = mMatrix * sMatrix;
    }
    shaderProgram.setUniformValue("mMatrix", mMatrix);
    shaderProgram.setUniformValue("pvmMatrix", pvMatrix * mMatrix);
    shaderProgram.setUniformValue("norTranMatrix", mMatrix.normalMatrix());

    QOpenGLVertexArrayObject::Binder vaoBind(&(rps->m_vao));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rps->m_nIBOId);
    glDrawElements(GL_TRIANGLES, rps->m_fNum * 3, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void GcodeViewer::paintGL()
{
    if (m_scene3d == NULL)
        return;
    if (width() <= 0 || height() <= 0)
        return;
    //glClearColor(234 / 255.0, 234 / 255.0, 234 / 255.0, 0.0);
    if(m_isNotOffScreen)
    {
        glClearColor(31.0 / 255.0, 32.0 / 255.0, 34.0 / 255.0,0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }else{
        glClearColor(255, 255, 255, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    //Add by Aden Hu
    //apply
    QMatrix4x4 pMatrix;
    m_scene3d->getCurrentProjMat(pMatrix);
    QMatrix4x4 vMatrix;
    m_scene3d->getCurrentViewMat(vMatrix);
    if (m_isNotOffScreen)
    {
        m_renderData->m_shaderProgramPtr->bind();
        QMatrix4x4 pvMatrix = pMatrix * vMatrix;
        m_renderData->m_shaderProgramPtr->setUniformValue("useTexture", false);
        
        QVector3D lightColor(1.0f, 1.0f, 1.0f);
        QVector3D lightPos(0.0f, 0.0f, 200.0f);
        m_renderData->m_shaderProgramPtr->setUniformValue("viewPos", m_scene3d->getEye());

        
        QVector3D lightscolor[10];
        QVector3D lightsPos[10];
        lightscolor[0] = QVector3D(1.0f, 1.0f, 1.0f);
        lightsPos[0] = m_scene3d->getEye();
        lightscolor[1] = QVector3D(1.0f, 1.0f, 1.0f);
        lightsPos[1] = QVector3D(0, -300, 200);
        m_renderData->m_shaderProgramPtr->setUniformValueArray("lightColor", lightscolor, 10);
        m_renderData->m_shaderProgramPtr->setUniformValueArray("lightPos", lightsPos, 10);
        m_renderData->m_shaderProgramPtr->setUniformValue("lightNum", 1);



        for (auto mit = m_renderData->m_allmeshes.begin(); mit != m_renderData->m_allmeshes.end(); mit++)
        {
            paintMeshVbo(*m_renderData->m_shaderProgramPtr, mit->first, mit->second, pvMatrix);
        }

        m_renderData->m_shaderProgramPtr->setUniformValue("calLight", false);
#ifdef __APPLE__
        for (auto cit = m_renderData->m_allcurves.begin(); cit != m_renderData->m_allcurves.end(); cit++)
        {
            paintCurveVbo(*m_renderData->m_shaderProgramPtr, cit->first, cit->second, pvMatrix);
        }
        
#else
        for (auto cit = m_renderData->m_allcurves.begin(); cit != m_renderData->m_allcurves.end(); cit++)
        {
            if(cit->first->getColor() == QColor(125, 0, 0))
            {
                //qDebug() << "X Axis.";
            }
            paintCurveVbo(*m_renderData->m_shaderProgramPtr, cit->first, cit->second, pvMatrix);
        }
        
#endif
        //glLineWidth(1);
        m_renderData->m_shaderProgramPtr->release();
    }

    const GLfloat* view_matrix = vMatrix.data();
    //apply projection
    GLfloat* projection_matrix = pMatrix.data();
    glsafe(glEnable(GL_DEPTH_TEST));
//    glsafe(glDepthMask(GL_TRUE));
//    glsafe(glDisable(GL_BLEND));
    render_toolpaths(vMatrix, pMatrix);
    SequentialView* sequential_view = const_cast<SequentialView*>(&m_sequential_view);

    if (sequential_view->current.last != sequential_view->endpoints.last && sequential_view->endpoints.first != this->m_moves_count) {
        sequential_view->marker.set_world_position(sequential_view->current_position);
        sequential_view->marker.render(this->shaderProgram1,vMatrix, pMatrix);
    }
     //glDepthMask( GL_FALSE );
}

#include<QDateTime>
void GcodeViewer::render_toolpaths(QMatrix4x4 vM, QMatrix4x4 pM)
{
    //    QElapsedTimer timer;
    auto render_as_points = [this](const TBuffer& buffer, unsigned int ibuffer_id, QOpenGLShaderProgram& shader){
        for (const RenderPath& path : buffer.render_paths) {
            if (path.ibuffer_id == ibuffer_id) {
                //shader.setUniformValue();
                glsafe(glMultiDrawElements(GL_POINTS, (const GLsizei*)path.sizes.data(), GL_UNSIGNED_SHORT, (const void* const*)path.offsets.data(), (GLsizei)path.sizes.size()));
            }
        }
    };

    auto render_as_triangles = [this](const TBuffer& buffer, unsigned int ibuffer_id, QOpenGLShaderProgram& shader) {
        for (const RenderPath& path : buffer.render_paths) {
            if (path.ibuffer_id == ibuffer_id) {
                int id = -1;
                id = glGetUniformLocation(shader.programId(), "uniform_color");
                if (id >= 0) {
                    glsafe(glUniform4fv(id, 1, static_cast<const GLfloat*>(path.color.data())));
                }
                glsafe(glMultiDrawElements(GL_TRIANGLES, (const GLsizei*)path.sizes.data(), GL_UNSIGNED_SHORT, (const void* const*)path.offsets.data(), (GLsizei)path.sizes.size()));
            }
        }
    };

    std::array<float, 4> light_intensity = { 0.25f, 0.70f, 0.75f, 0.75f };
    auto render_as_lines = [this, light_intensity](const TBuffer& buffer, unsigned int ibuffer_id, QOpenGLShaderProgram& shader) {
        //shader.set_uniform("light_intensity", light_intensity);
        int lid = -1;
        lid = glGetUniformLocation(shader.programId(), "light_intensity");
        if (lid >= 0) {
            glsafe(glUniform4fv(lid, 1, static_cast<const GLfloat*>(light_intensity.data())));
        }
        for (const RenderPath& path : buffer.render_paths) {
            if (path.ibuffer_id == ibuffer_id) {
                //set_uniform_color(path.color, shader);
                int id = -1;
                id = glGetUniformLocation(shader.programId(), "uniform_color");
                if (id >= 0) {
                    glsafe(glUniform4fv(id, 1, static_cast<const GLfloat*>(path.color.data())));
                }
                glsafe(glMultiDrawElements(GL_LINES, (const GLsizei*)path.sizes.data(), GL_UNSIGNED_SHORT, (const void* const*)path.offsets.data(), (GLsizei)path.sizes.size()));
            }
        }
    };

    auto render_as_ankermove = [this](const TBuffer& buffer, unsigned int ibuffer_id, QOpenGLShaderProgram& shader) {
        //shader.set_uniform("light_intensity", light_intensity)
        for (const RenderPath& path : buffer.render_paths) {
            if (path.ibuffer_id == ibuffer_id) {
                //set_uniform_color(path.color, shader);
                shader.setUniformValue("uniform_color",path.color[0],path.color[1],path.color[2],path.render_cap);
                glsafe(glMultiDrawElements(GL_LINES, (const GLsizei*)path.sizes.data(), GL_UNSIGNED_SHORT, (const void* const*)path.offsets.data(), (GLsizei)path.sizes.size()));
            }
        }
    };

    unsigned char begin_id = buffer_id(EMoveType::Retract);
    unsigned char end_id = buffer_id(EMoveType::Count);
    for (unsigned char i = begin_id; i < end_id; ++i) {
        const TBuffer& buffer = m_buffers[i];
        if (!buffer.visible || !buffer.has_data())
            continue;
        //         timer.start();
        QOpenGLShaderProgram* shaderProgram = nullptr;//= &this->shaderProgram1;
        if (buffer.shader == "gouraud_light")
        {
            if (m_isNotOffScreen || m_orceShader1)
            {
                if(this->isTRender){
                    shaderProgram = &this->shaderProgram_tr;
                }
                else{
                    shaderProgram = &this->shaderProgram1;
                }
            }else{
                shaderProgram = &this->shaderProgram_or;
            }
        }
        if (buffer.shader == "toolpaths_lines")
        {
            shaderProgram = &this->shaderProgram2;
        }
        if (buffer.shader == "toolpaths_point"){
            shaderProgram = &shaderProgram_point;
        }

        if (shaderProgram == nullptr)
        {
            return;
        }
        shaderProgram->bind();

        glsafe(glBindVertexArray(buffer.t_vao));

        shaderProgram->setUniformValue("mv_matrix", vM);
        shaderProgram->setUniformValue("p_matrix", pM);
        QMatrix3x3 normalM = vM.normalMatrix();//vM.inverted().transposed().normalMatrix();
        shaderProgram->setUniformValue("normal_matrix", normalM);

        for (size_t j = 0; j < buffer.indices.size(); ++j) {
            const IBuffer& i_buffer = buffer.indices[j];

            glsafe(glBindBuffer(GL_ARRAY_BUFFER, i_buffer.vbo));
            //glsafe(glVertexPointer(buffer.vertices.position_size_floats(), GL_FLOAT, buffer.vertices.vertex_size_bytes(), (const void*)buffer.vertices.position_offset_size()));
            //buffer.vertices.position_size_floats()   ->  size
            //buffer.vertices.vertex_size_bytes()  ->  stride
            //(const void*)buffer.vertices.position_offset_size() -> pointer
            int attr = -1;
            attr = shaderProgram->attributeLocation("aPos");
            shaderProgram->setAttributeBuffer(attr, GL_FLOAT, 0, 3, buffer.vertices.vertex_size_bytes());
            shaderProgram->enableAttributeArray(attr);
            //glsafe(glVertexAttribPointer(0, buffer.vertices.position_size_floats(), GL_FLOAT, GL_FALSE, buffer.vertices.vertex_size_bytes(), (const void*)buffer.vertices.position_offset_size()));
            bool has_normals = buffer.vertices.normal_size_floats() > 0;
            if (has_normals) {
                int attr = -1;
                attr = shaderProgram->attributeLocation("aNormal");
                shaderProgram->setAttributeBuffer(attr, GL_FLOAT, buffer.vertices.normal_offset_size(), 3, buffer.vertices.vertex_size_bytes());
                shaderProgram->enableAttributeArray(attr);
            }
            bool has_ankermoveinfo = buffer.vertices.is_ankermove_type();
            if (has_ankermoveinfo) {
                //freedrate
                switch(m_colortype)
                {
                case Flow:
                    shaderProgram->setUniformValue("colorRange", this->gcode_result.flow_range[0],this->gcode_result.flow_range[1]);
                    //qDebug() << "this->gcode_result.flow_range[0]" <<this->gcode_result.flow_range[0];
                    break;
                default:
                    shaderProgram->setUniformValue("colorRange", this->gcode_result.freedrate_range[0],this->gcode_result.freedrate_range[1]);
                    break;

                }

                shaderProgram->setUniformValue("renderType",(int)this->m_colortype);

                int attr = -1;
                attr = shaderProgram->attributeLocation("ankerInfo_1");
                shaderProgram->setAttributeBuffer(attr, GL_FLOAT, 3 * sizeof(float), 4, buffer.vertices.vertex_size_bytes());
                shaderProgram->enableAttributeArray(attr);

                attr = shaderProgram->attributeLocation("ankerInfo_2");// mm3_per_mm fan_speed layer_duration time

                shaderProgram->setAttributeBuffer(2, GL_FLOAT, 7 * sizeof(float), 4, buffer.vertices.vertex_size_bytes());
                shaderProgram->enableAttributeArray(2);

                attr = shaderProgram->attributeLocation("ankerInfo_3");// temperature motor_process.accelerate_until  motor_process.cruise
                shaderProgram->setAttributeBuffer(3, GL_FLOAT, 11 * sizeof(float), 3, buffer.vertices.vertex_size_bytes());
                shaderProgram->enableAttributeArray(3);

                attr = shaderProgram->attributeLocation("ankerInfo_4");// motor_process.decelerate_after motor_process.entry  motor_process.exit
                shaderProgram->setAttributeBuffer(4, GL_FLOAT, 14 * sizeof(float), 3, buffer.vertices.vertex_size_bytes());
                shaderProgram->enableAttributeArray(4);

                attr = shaderProgram->attributeLocation("ankerInfo_5");// type extrusion_role  g1_line_id
                shaderProgram->setAttributeBuffer(5, GL_FLOAT, 17 * sizeof(float), 3, buffer.vertices.vertex_size_bytes());
                shaderProgram->enableAttributeArray(5);


            }

            glsafe(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer.ibo));
            switch (buffer.render_primitive_type)
            {
            case TBuffer::ERenderPrimitiveType::Point: {
                glsafe(glEnable(GL_PROGRAM_POINT_SIZE));

                render_as_points(buffer, static_cast<unsigned int>(j), *shaderProgram);
                break;
            }
            case TBuffer::ERenderPrimitiveType::Line: {
                render_as_lines(buffer, static_cast<unsigned int>(j), *shaderProgram);
                break;
            }
            case TBuffer::ERenderPrimitiveType::Triangle: {
                render_as_triangles(buffer, static_cast<unsigned int>(j), *shaderProgram);
                break;
            case TBuffer::ERenderPrimitiveType::AnkerMove: {
                    render_as_ankermove(buffer, static_cast<unsigned int>(j), *shaderProgram);
                    break;
                }
                }
            }

            glsafe(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

            //            if (has_normals)
            //                glsafe(glDisableClientState(GL_NORMAL_ARRAY));

            //            glsafe(glDisableClientState(GL_VERTEX_ARRAY));
            glsafe(glBindBuffer(GL_ARRAY_BUFFER, 0));
        }

        glsafe(glBindVertexArray(0));

        shaderProgram->release();
        //         qDebug() << "bind release main  :"<<timer.elapsed() <<"  ms";
    }

#if ENABLE_REDUCED_TOOLPATHS_SEGMENT_CAPS
    auto render_sequential_range_cap = [this](const SequentialRangeCap& cap) {

        shaderProgram1.bind();

        glsafe(glBindVertexArray(cap.vao));

        glsafe(glBindBuffer(GL_ARRAY_BUFFER, cap.vbo));
        int attr = -1;
        attr = shaderProgram1.attributeLocation("aPos");
        shaderProgram1.setAttributeBuffer(attr, GL_FLOAT, 0, cap.buffer->vertices.position_size_floats(), cap.buffer->vertices.vertex_size_bytes());
        shaderProgram1.enableAttributeArray(attr);
        //glsafe(glVertexPointer(cap.buffer->vertices.position_size_floats(), GL_FLOAT, cap.buffer->vertices.vertex_size_bytes(), (const void*)cap.buffer->vertices.position_offset_size()));
        //glsafe(glEnableClientState(GL_VERTEX_ARRAY));
        bool has_normals = cap.buffer->vertices.normal_size_floats() > 0;
        if (has_normals) {
            int attr = -1;
            attr = shaderProgram1.attributeLocation("aNormal");
            shaderProgram1.setAttributeBuffer(attr, GL_FLOAT, cap.buffer->vertices.normal_offset_size(), 3, cap.buffer->vertices.vertex_size_bytes());
            shaderProgram1.enableAttributeArray(attr);
            //glsafe(glNormalPointer(GL_FLOAT, cap.buffer->vertices.vertex_size_bytes(), (const void*)cap.buffer->vertices.normal_offset_size()));
            //glsafe(glEnableClientState(GL_NORMAL_ARRAY));
        }

        //set_uniform_color(cap.color, *shader);
        int id = -1;
        id = glGetUniformLocation(shaderProgram1.programId(), "uniform_color");
        if (id >= 0) {
            glsafe(glUniform4fv(id, 1, static_cast<const GLfloat*>(cap.color.data())));
        }

        glsafe(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cap.ibo));
        glsafe(glDrawElements(GL_TRIANGLES, (GLsizei)cap.indices_count(), GL_UNSIGNED_SHORT, nullptr));
        glsafe(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

#if ENABLE_GCODE_VIEWER_STATISTICS
        ++const_cast<Statistics*>(&m_statistics)->gl_triangles_calls_count;
#endif // ENABLE_GCODE_VIEWER_STATISTICS

        //        if (has_normals)
        //            glsafe(glDisableClientState(GL_NORMAL_ARRAY));

        //        glsafe(glDisableClientState(GL_VERTEX_ARRAY));
        glsafe(glBindBuffer(GL_ARRAY_BUFFER, 0));

        glsafe(glBindVertexArray(0));

        shaderProgram1.release();

    };

    for (unsigned int i = 0; i < 2; ++i) {
        if (m_sequential_range_caps[i].is_renderable())
            render_sequential_range_cap(m_sequential_range_caps[i]);
    }
#endif
}

void GcodeViewer::horizonsliderMove(unsigned int value1, unsigned int value2)
{
    //qDebug() << tr("HorizonSlider: ") << value;
    //unsigned int  layerfisrt = static_cast<unsigned int>(m_layers.get_endpoints_at(value).first);
    this->update_sequential_view_current(value1, value2);
    this->update();
}

void GcodeViewer::verticalsliderMove(int value1, int value2)
{
    //qDebug() << tr("VerticalSlider: ") << value;
    //refreshtic
    m_layers_z_range = std::array<unsigned int, 2>{static_cast<unsigned int>(value1 - 1), static_cast<unsigned int>(value2 - 1) };
    refresh_render_paths(true, false);
    //this->update();
}

void GcodeViewer::focusInEvent( QFocusEvent * event )
{

    return QOpenGLWidget::focusInEvent(event);
}

void GcodeViewer::focusOutEvent( QFocusEvent * event )
{
     return QOpenGLWidget::focusOutEvent(event);
}
void GcodeViewer::mouseMoveEvent(QMouseEvent * event)
{
    //AkUtil::TDebug("------------- GcodeViewer::mouseMoveEvent--------------");
    if (m_scene3d == NULL)
        return;
    QPoint qPoint = event->pos();
    if(event->buttons() & Qt::LeftButton && m_scene3d->getIsMousePress())
    {
        m_scene3d->xyRotate(qPoint.x(), qPoint.y());
    }
    else if(event->buttons() & Qt::MiddleButton && m_scene3d->getIsMousePress())
    {
        m_scene3d->xyTranslate(qPoint.x(), qPoint.y());
    }
    else if (event->buttons() & Qt::RightButton && m_scene3d->getIsMousePress())
    {
        m_scene3d->xyTranslate(qPoint.x(), qPoint.y());
    }
    //    QPoint qPoint = event->pos();
    //    if (event->buttons() & Qt::MiddleButton && m_scene3d->getIsMousePress())
    //    {
    //        if (m_scene3d->getIsControlKeyDown())
    //        {
    //            m_scene3d->xyTranslate(qPoint.x(), qPoint.y());
    //        }
    //        else
    //        {
    //            m_scene3d->xyRotate(qPoint.x(), qPoint.y());
    //        }
    //    }
    //    if (event->buttons() & Qt::RightButton && m_scene3d->getIsMousePress())
    //    {
    //        m_scene3d->xyTranslate(qPoint.x(), qPoint.y());
    //    }
    //    if (event->buttons() & Qt::LeftButton && m_scene3d->getIsMousePress())
    //    {
    //        m_scene3d->xyRotate(qPoint.x(), qPoint.y());
    //    }
    QOpenGLWidget::update();
    event->accept();
}

void GcodeViewer::mouseDoubleClickEvent(QMouseEvent * event)
{
    //AkUtil::TDebug("------------- GcodeViewer::mouseDoubleClickEvent--------------");
    QWidget::mouseReleaseEvent(event);
}

void GcodeViewer::mousePressEvent(QMouseEvent * event)
{
    //AkUtil::TDebug("------------- GcodeViewer::mousePressEvent--------------");
    if (m_scene3d == NULL)
        return;
    m_scene3d->setMousePress(true);
    QVector3D center = m_scene3d->getMainbox().getCenterPoint();//(m_boundingBox.max + m_boundingBox.min) / 2.0;
    if (center != QVector3D(0.0, 0.0, 0.0))
    {
        m_scene3d->setRefTranPoint(center.x(), center.y(), center.z());
    }

    QOpenGLWidget::update();
    event->accept();
}

void GcodeViewer::mouseReleaseEvent(QMouseEvent * event)
{
    AkUtil::TDebug("------------- GcodeViewer::mouseReleaseEvent(--------------");
    if (m_scene3d == NULL)
        return;
    m_scene3d->setMousePress(false);
    m_scene3d->setBMove(true);
    QWidget::mouseReleaseEvent(event);
    QOpenGLWidget::update();
    //event->accept();
}

void GcodeViewer::wheelEvent(QWheelEvent * event)
{
    AkUtil::TDebug("------------- GcodeViewer::wheelEvent--------------");
    //setFocus();
    //qDebug() << "gcodeViewer hasFocus ()" << this->hasFocus();
    if (m_scene3d == NULL | this->hasFocus() == false)
    {
        event->ignore();
        return;
    }
    int qwheel = event->delta();
    double zoomScale = (double)(qwheel) / 2000.0;
    m_scene3d->scale(event->x(), event->y(), zoomScale);
    QOpenGLWidget::update();
    //event->accept();
}


void GcodeViewer::update_sequential_view_current(unsigned int& first, unsigned int& last)
{
    auto is_visible = [this](unsigned int id) {
        for (const TBuffer& buffer : m_buffers) {
            if (buffer.visible) {
                for (const Path& path : buffer.paths) {
#if ENABLE_SPLITTED_VERTEX_BUFFER
                    if (path.sub_paths.front().first.s_id <= id && id <= path.sub_paths.back().last.s_id)
#else
                    if (path.first.s_id <= id && id <= path.last.s_id)
#endif // ENABLE_SPLITTED_VERTEX_BUFFER
                        return true;
                }
            }
        }
        return false;
    };

    int first_diff = static_cast<int>(first) - static_cast<int>(m_sequential_view.last_current.first);
    int last_diff = static_cast<int>(last) - static_cast<int>(m_sequential_view.last_current.last);

    unsigned int new_first = first;
    unsigned int new_last = last;

    if (m_sequential_view.skip_invisible_moves) {
        while (!is_visible(new_first)) {
            if (first_diff > 0)
                ++new_first;
            else
                --new_first;
        }

        while (!is_visible(new_last)) {
            if (last_diff > 0)
                ++new_last;
            else
                --new_last;
        }
    }

    m_sequential_view.current.first = new_first;
    m_sequential_view.current.last = new_last;
    m_sequential_view.last_current = m_sequential_view.current;

    refresh_render_paths(true, true);

    if (new_first != first || new_last != last)
    {
        first = new_first;
        last = new_last;
    }
}


void GcodeViewer::set_look_at(std::array<float, 3> position, std::array<float, 3> lookat, std::array<float, 3> lookup, bool isaiview)
{
    this->m_scene3d->setView(OTHER_VIEW_TYPE);
    this->m_scene3d->setEye(position[0], position[1], position[2]);
    //this->m_scene3d->setRefTranPoint(lookat[0],lookat[1],lookat[2]);
    this->m_scene3d->setUp(lookup[0], lookup[1], lookup[2]);
    this->m_scene3d->setFront(lookat[0], lookat[1], lookat[2]);
    if(isaiview)
    {
        this->m_scene3d->setIsAiView();
    }
}

void GcodeViewer::set_save_pic(bool is_save)
{
    this->m_save_pic = is_save;
    this->update();
}

std::vector<unsigned int>  GcodeViewer::genLoopVec()
{
    unsigned int LayerMax = this->m_layers.size() - 1;
    unsigned int realLayerNum = LayerMax > 100 ? 100 : LayerMax;
    double mulPara = LayerMax / 100.0 > 1.0 ? LayerMax / 100.0 : 1.0;
    std::vector<unsigned int> genVec;
    for (unsigned int i = 0; i < realLayerNum; i++)
    {
        unsigned int r = round(i * mulPara);
        genVec.push_back(r);
    }
    return genVec;
}

bool GcodeViewer::matchGcodeName()
{
    QRegularExpression re(".+/(.+).gcode");
    QString gcodePath = QString::fromStdString(this->m_gcode_path.c_str());
    QRegularExpressionMatch match = re.match(gcodePath);
    //qDebug() << "gcodePath" << gcodePath;
    if (match.hasMatch())
    {
        this->m_gcode_name = match.captured(1);
    }
    return match.hasMatch();
}

void GcodeViewer::setOriginalStlName(const QString& oStlName)
{
    this->originalStlName = oStlName;
}

void GcodeViewer::setOriginalGcodeName(const QString& oGcodeName)
{
    this->originalGcodeName = oGcodeName;
}

QString GcodeViewer::sendOriginalStlNamedFile(bool isNeedReplace)
{
    if(this->m_gcode_path.size() <= 1 )
    {
        return "";
    };
    QString senStr;
    if(this->originalStlName.size() > 0 && matchGcodeName())
    {
        senStr = QString::fromStdString(this->m_gcode_path).replace(m_gcode_name,originalStlName);
    }else{
        AkUtil::TDebug("originalStlName size < 0 , use default gcode name");
        senStr = QString::fromStdString(this->m_gcode_path).replace("aiGcode.gcode",originalGcodeName);
    }

    if(senStr.size() > 0 && isNeedReplace)
    {
        QString debugstr;
        QDebug(&debugstr) << "m_gcode_name gcode :" << this->m_gcode_name;
        QDebug(&debugstr) << "originalGcodeName gcode :" << this->originalGcodeName;
        QDebug(&debugstr) << "m_gcode_path gcode :" << QString::fromStdString(this->m_gcode_path);
        QDebug(&debugstr) << "sendOriginalStlNamedFile gcode :" << senStr;
        AkUtil::TDebug(debugstr);
        if(QFile::exists(QString::fromStdString(m_gcode_path)))
        {
            QFile::rename(QString::fromStdString(m_gcode_path), senStr);
            m_gcode_path = senStr.toStdString();
        }
    }
    return senStr;
}

QString GcodeViewer::sendAcodeToPrintCtr()
{
    QString re = sendOriginalStlNamedFile(false);
    qDebug()<< "re :" <<re;
    if(re.size() <= 1)
    {
        return re;
    }
    re = re.replace(".gcode",".acode");
    
    return re;
}

bool GcodeViewer::setGcodePath(const std::string & gcodePath)
{
    this->m_gcode_path = gcodePath;
    return this->matchGcodeName();
}

std::string GcodeViewer::getGcodePath()
{
    return this->m_gcode_path;
}

QString  GcodeViewer::genSavePath(unsigned int layer)
{
    auto writableLocation = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QString dirPath = writableLocation + "./AICamera";
    QDir dir(dirPath);
    if (!dir.exists()) {
        dir.mkpath(dir.absolutePath());
    }


    QString modelName = "mesh";//this->m_gcode_name;
    modelName += "_layer";
    modelName += QString::number(layer);
    modelName += ".jpg";

    return modelName;

}

std::vector<float> GcodeViewer::get_roi_info(int w, int h)
{
    std::vector<float> roi_info{-1.0,-1.0,1.0,1.0};

    QMatrix4x4 pMatrix;
    m_scene3d->getCurrentProjMat(pMatrix);
    QMatrix4x4 vMatrix;
    m_scene3d->getCurrentViewMat(vMatrix);
    auto genVertex =[](boundingBox bBox){
        std::vector<QVector4D> vertex(20,{0.0,0.0,0.0,1.0});
        vertex[0][0] = bBox.min[0];
        vertex[0][1] = bBox.min[1];
        vertex[0][2] = bBox.min[2];

        vertex[1][0] = bBox.min[0];
        vertex[1][1] = bBox.max[1];
        vertex[1][2] = bBox.min[2];

        vertex[2][0] = bBox.min[0];
        vertex[2][1] = bBox.max[1];
        vertex[2][2] = bBox.max[2];

        vertex[3][0] = bBox.min[0];
        vertex[3][1] = bBox.min[1];
        vertex[3][2] = bBox.max[2];

        vertex[4][0] = bBox.max[0];
        vertex[4][1] = bBox.min[1];
        vertex[4][2] = bBox.max[2];

        vertex[5][0] = bBox.max[0];
        vertex[5][1] = bBox.min[1];
        vertex[5][2] = bBox.min[2];

        vertex[6][0] = bBox.max[0];
        vertex[6][1] = bBox.max[1];
        vertex[6][2] = bBox.min[2];

        vertex[7][0] = bBox.max[0];
        vertex[7][1] = bBox.max[1];
        vertex[7][2] = bBox.max[2];

        vertex[8] =  (vertex[0] + vertex[1]) / 2;
        vertex[9] =  (vertex[1] + vertex[2]) / 2;
        vertex[10] =  (vertex[2] + vertex[3]) / 2;
        vertex[11] =  (vertex[3] + vertex[0]) / 2;

        vertex[12] =  (vertex[4] + vertex[5]) / 2;
        vertex[13] =  (vertex[5] + vertex[6]) / 2;
        vertex[14] =  (vertex[6] + vertex[7]) / 2;
        vertex[15] =  (vertex[7] + vertex[4]) / 2;

        vertex[16] =  (vertex[4] + vertex[0]) / 2;
        vertex[17] =  (vertex[5] + vertex[1]) / 2;
        vertex[18] =  (vertex[6] + vertex[2]) / 2;
        vertex[19] =  (vertex[7] + vertex[3]) / 2;

        return vertex;
    };

    std::vector<QVector4D> vertex = genVertex(m_boundingBox);
    for (auto it = vertex.begin();it != vertex.end() ;it++)
    {
        QVector4D coorPos = pMatrix * vMatrix * (*it);
        
        coorPos = coorPos / coorPos[3];
        
        //coorPos.normalized();
        roi_info[0] = roi_info[0] <  coorPos[0] ? coorPos[0] : roi_info[0]; //max
        roi_info[1] = roi_info[1] <  coorPos[1] ? coorPos[1] : roi_info[1];
        roi_info[2] = roi_info[2] >  coorPos[0] ? coorPos[0] : roi_info[2]; //min
        roi_info[3] = roi_info[3] >  coorPos[1] ? coorPos[1] : roi_info[3];
    }
    
    for(int i = 0;i < 4;i++)
    {
        roi_info[i] = (roi_info[i] + 1.0) / 2.0;
        roi_info[i] = roi_info[i] > 1.0 ? 1.0 : roi_info[i];
        roi_info[i] = roi_info[i] < 0.0 ? 0.0 : roi_info[i];
        if(i % 2 != 0)
        {
            //roi_info[i] = 1 - roi_info[i];
            roi_info[i] *= h;
        }else{
            roi_info[i] *= w;
        }

    }

    // to  w h p1 p2 openGL coord
    roi_info[0] = roi_info[0] - roi_info[2];
    roi_info[1] = roi_info[1] - roi_info[3];

    return roi_info;
}


//float getNearWay(float fov,float ration,float imageW,float imageH)
//{

//}


QImage GcodeViewer::render_singe_iamge()
{
    AkUtil::TFunction("");

    paramsStack pst(this);
    boolLock orc(this->m_orceShader1);
    //m_scene3d->setVerticalAngle(45.0);

    QOpenGLFramebufferObjectFormat format;
    format.setSamples(24);
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);

    
    //for (int t = ExtrusionRole::erNone;t < ExtrusionRole::erCount; (ExtrusionRole)t++)
    {
        std::array<float, 3> passColor{ 230.0 / 255.0,230.0 / 255.0,230.0 / 255.0};
        this->setRoleColor(ExtrusionRole::erExternalPerimeter,std::move(passColor));
        passColor = {{220.0 / 255.0, 220.0 / 255.0, 220.0 / 255.0}};
        this->setRoleColor(ExtrusionRole::erPerimeter,std::move(passColor));
        passColor = {{200.0 / 255.0, 200.0 / 255.0, 200.0 / 255.0}};
        this->setRoleColor(ExtrusionRole::erSolidInfill,std::move(passColor));
        passColor = {{180.0 / 255.0, 180.0 / 255.0, 180.0 / 255.0}};
        this->setRoleColor(ExtrusionRole::erSkirt,std::move(passColor));
        this->setRoleColor(ExtrusionRole::erWipeTower,std::move(passColor));
        passColor = {{160.0 / 255.0, 160.0 / 255.0, 160.0 / 255.0}};
        this->setRoleColor(ExtrusionRole::erInternalInfill,std::move(passColor));

    }
    this->refresh_render_paths(false,false);
    
    std::array<float, 3> setPosition;
    std::array<float, 3> viewCenter;
    std::array<float, 3> camLookUp;
    std::array<float, 3> boundcenter;
    float boundbox_height = this->m_boundingBox.max.z() - this->m_boundingBox.min.z();
    float boundbox_width = this->m_boundingBox.max.x() - this->m_boundingBox.min.x();
    float halfDepth = this->m_boundingBox.max.y() - this->m_boundingBox.min.y();
    float length = (m_boundingBox.max - m_boundingBox.min).norm();

    
    float degrees = 15.0f;
    float radians = qDegreesToRadians(degrees);
    //qDebug() <<"(1.0 / qTan(radians))" <<(1.0 / qTan(radians));
    //float nearWay = (1.0 / qTan(radians)) * 0.5 * boundbox_height;// 1/tan(fov) * h/2
    //float nearWay_w = boundbox_width / (16.0 / 9.0  * qTan(radians));
    float nearWay = (1.0 / qTan(radians)) * 0.5 * length;       
    float nearWay_w = length / (16.0 / 9.0  * qTan(radians));   
    float nerWay_1 = (1.0 / qTan(radians)) * 0.5 * (boundbox_width * 23.0 / 9.0);      
    nearWay = qMax(nearWay, nearWay_w);
    nearWay = qMax(nearWay, nerWay_1);
    
    double dis_near_way = nearWay ;//+ halfDepth;
    float e_degrees = 15.0f;
    double y_delta = dis_near_way * qCos(qDegreesToRadians(e_degrees));
    double z_delta = dis_near_way * qSin(qDegreesToRadians(e_degrees));

    //qDebug() <<"near way" <<nearWay;
    boundcenter[0] = (this->m_boundingBox.max.x() + this->m_boundingBox.min.x()) * 0.5;
    boundcenter[1] = (this->m_boundingBox.max.y() + this->m_boundingBox.min.y()) * 0.5;
    boundcenter[2] = (this->m_boundingBox.max.z() + this->m_boundingBox.min.z()) * 0.5;

    setPosition[0] = boundcenter[0];
    setPosition[2] = boundcenter[2] + z_delta;
    setPosition[1] = boundcenter[1] - y_delta;

    QVector3D tp;
    tp.setX(setPosition[0]-boundcenter[0]);
    tp.setY(setPosition[1]-boundcenter[1]);
    tp.setZ(setPosition[2]-boundcenter[2]);
    tp.normalize();
    viewCenter[0] = -tp.x();//0.0;
    viewCenter[1] = -tp.y();//1.0;
    viewCenter[2] = -tp.z();//0.0;
    camLookUp[0] = 0.0;
    camLookUp[1] = 0.0;
    camLookUp[2] = 1.0;
    this->set_look_at(std::move(setPosition), std::move(viewCenter), std::move(camLookUp),false);

    m_scene3d->resizeWidget(5096, 2867);
    m_fbo = new QOpenGLFramebufferObject(2867, 2867, format);

    if (!m_fbo->isValid()) {
        //qDebug() << tr("OpenGlOffscreenSurface::recreateFbo() - Failed to create background FBO!");
        AkUtil::TDebug(QString("OpenGlOffscreenSurface::recreateFbo() - Failed to create background FBO!" ));
    }
    // clear framebuffer
    m_fbo->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glViewport((-5096+2867)*0.5, 0,5096, 2867);

    this->offRenderSingle();
    return m_fbo->toImage().scaled(256, 256,Qt::KeepAspectRatio,Qt::SmoothTransformation);
}

bool GcodeViewer::postGcodeHead(const QString &line,QString& reLine)
{
    reLine = line;
    QRegularExpression timeMatched("(?<=;TIME:).+(?=s)");
    QRegularExpressionMatch time_match = timeMatched.match(line);
    if(time_match.hasMatch())
    {
        this->getTimeAndPercent();
        float sum = std::accumulate(times.begin(),times.end(),0.0);
        reLine = reLine.replace(time_match.captured(),QString::number(sum));
        return true;
    }
    return false;
}

bool GcodeViewer::waitForSceneInit()
{
    
    
    if(m_scene3d == nullptr)
    {
        m_scene3d_init_mutex.lock();
        AkUtil::TDebug("wait for notify....");
        m_scene3d_inited.wait(&m_scene3d_init_mutex);
        m_scene3d_init_mutex.unlock();
    }
    return true;
}

void GcodeViewer::off_render_single(const QImage& res)
{
    AkUtil::TFunction("");
    //AkUtil::TDebug("---Time start--");
    postProcessGcode tpg;

    QRegularExpression re("(?<=;Generated).+(?=AnkerSlicer)");
    QRegularExpression reTb("(?<=;).*(?=thumbnail begin)");
    QRegularExpression reTe("(?<=;).*(?=thumbnail end)");
    bool inThumbnailLine = false;
    bool inHead = true;
    bool isWritePng = false;

    QString pngPath = QString::fromStdString(this->m_gcode_path).replace(this->m_gcode_name,"aiGcode");//QString::fromStdString(this->m_gcode_path);
    qDebug()<< "this->m_gcode_path" << QString::fromStdString(this->m_gcode_path);
    qDebug()<<"pngPath" << pngPath;
    //pngPath.replace(".gcode","withpng.gcode");
    QFile wfile(pngPath);
    if(!wfile.open(QIODevice::WriteOnly|QIODevice::Text)){
    }
    QStringList resStrList;
    //AkUtil::TDebug("---start read_file--");
    
    tpg.read_file(this->m_gcode_path,[this,&re,&res,&wfile,&reTb,&reTe,&isWritePng,&inThumbnailLine,&inHead,&resStrList](postProcessGcode& reader, const QString& line){
        resStrList<<line;
        QByteArray resstr = line.toUtf8();
        if(inHead)
        {
            //AkUtil::TDebug("---read_file head--");
            QRegularExpressionMatch match = re.match(line);
            if(match.hasMatch())
            {
                //AkUtil::TDebug("---read_file render_singe_iamge--");
//                QImage res = this->render_singe_iamge();
                //AkUtil::TDebug("---read_file render_singe_iamge scaled--");
//                QImage scale = res.scaled(256, 256,Qt::KeepAspectRatio,Qt::SmoothTransformation);

//                res.save("testpng.png","PNG");
//                scale.save("testpngscale.png","PNG");
                QByteArray arr;
                QBuffer buffer(&arr);
                buffer.open(QIODevice::WriteOnly);
                res.save(&buffer, "PNG");
                buffer.close();
                wfile.write(resstr+'\n');
                wfile.write("; thumbnail begin 256 256\n");
                int curi = 0;
                int stride = 200;
                //AkUtil::TDebug("---read_file render_singe_iamge write--");
                while(curi + stride < arr.toBase64().size())
                {
                    wfile.write("; ");
                    wfile.write(arr.toBase64().constData() + curi,stride);
                    wfile.write("\n");
                    curi += stride;
                    //qDebug() <<"teste";
                }
                wfile.write("; ");
                wfile.write(arr.toBase64().constData() + curi ,arr.toBase64().size() - curi);
                wfile.write("\n");
                //wfile.write(arr.toBase64().constData()+10,arr.toBase64().size()-10);
                wfile.write("; thumbnail end \n");
                isWritePng = true;
                return ;
            }else{
                QRegularExpressionMatch matchTb = reTb.match(line);
                QRegularExpressionMatch matchTe = reTe.match(line);
                if(matchTb.hasMatch())
                {
                    inThumbnailLine = true;
                    return ;
                }
                if(!inThumbnailLine)
                {
                    if(isWritePng){
                        inHead = false;
                    }
                   // AkUtil::TDebug("---read_file test--");
                    QString reline;
                    bool needUpdate = postGcodeHead(line,reline);
                    resstr = needUpdate ? reline.toUtf8(): resstr;
                    //AkUtil::TDebug("---read_file test 111--");
                    wfile.write(resstr+'\n');
                    return ;
                }
                if(matchTe.hasMatch())
                {
                    inThumbnailLine = false;
                    inHead = false;
                    return ;
                }
                
                return;
            }
        }

        
        wfile.write(resstr+'\n');
        return ;
    });
    wfile.close();
    QString debugstr;
    QDebug(&debugstr) << "off_render_single gcode Path:" << QString::fromStdString(this->m_gcode_path);
    AkUtil::TDebug(debugstr);
    QFile::remove(QString::fromStdString(this->m_gcode_path));
    this->m_gcode_path = pngPath.toStdString();
}

void GcodeViewer::offPaint()
{
    QOpenGLFramebufferObjectFormat format;
    format.setSamples(1);
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);

    if(m_isNotOffScreen)
    {
        glClearColor(31.0 / 255.0, 32.0 / 255.0, 34.0 / 255.0,0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }else{
        glClearColor(255, 255, 255, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    QMatrix4x4 pMatrix;
    m_scene3d->getCurrentProjMat_ssp(pMatrix);
    QMatrix4x4 vMatrix;
    m_scene3d->getCurrentViewMat(vMatrix);
    glsafe(glEnable(GL_DEPTH_TEST));
    render_toolpaths(vMatrix, pMatrix);
}

void GcodeViewer::offRenderSingle()
{
    if (m_scene3d == NULL)
        return;
    glClearColor(31.0 / 255.0, 32.0 / 255.0, 34.0 / 255.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    QMatrix4x4 pMatrix;
    m_scene3d->getCurrentProjMat_sp(pMatrix);
    QMatrix4x4 vMatrix;
    m_scene3d->getCurrentViewMat(vMatrix);
    const GLfloat* view_matrix = vMatrix.data();
    //apply projection
    GLfloat* projection_matrix = pMatrix.data();
    glsafe(glEnable(GL_DEPTH_TEST));
    render_toolpaths(vMatrix, pMatrix);
}

QString GcodeViewer::filterFileName(QString pathName)
{
    QRegularExpression re(".+/(.+).acode");
    QString QgcodePath = pathName.replace("\\","/");
    QRegularExpressionMatch match = re.match(QgcodePath);
    if (match.hasMatch()){
        return match.captured(1);
    }else{
        AkUtil::TDebug("error : filterFileName has not matched");
        return "";
    }
}
#include <QTextCodec>
void GcodeViewer::off_render(QString savePath,bool isAiMode)
{
    //AkUtil::TFunction("");
    //IgnoreEvent(this);
    // akpic is exits , don't need to render again
    QString _gPath;
    QFileInfo savePathName = QFileInfo(savePath);

    if(QFile::exists(QString(QString::fromStdString(this->m_gcode_path.c_str()))))
    {
        _gPath =  QString::fromStdString(this->m_gcode_path.c_str());
    }else{
        AkUtil::TDebug("error : file not exit");
        return ;
    }

     AkUtil::TDebug("akpicSave"+akpicSave);
    if(akpicSave.isEmpty()){

        qDebug() <<"Aimode "<<isAiMode;
        std::vector<int>  loopVec = this->gcode_result.ai_pic_Layer;
        std::string cstring;
        QRegularExpression re(".+(?=.gcode)");
        QRegularExpressionMatch match = re.match(_gPath);

#ifdef USE_EXTRA_UI
        QString matchName;
#endif
        if (match.hasMatch()) //&& !m_offscreenSurface)
        {
            akpicSave = match.captured(0);
#ifdef USE_EXTRA_UI
            matchName = match.captured(0);
#endif
            akpicSave += ".akpic";
        }
        if(isAiMode)
        {
            paramsStack pst(this);
            //m_extrusions.role_visibility_flags = 24630;
            m_extrusions.role_visibility_flags = 524287;
            
            set_toolpath_move_type_visible(EMoveType::Travel, false);
            this->m_isNotOffScreen = false;
            cstring = std::string((const char*)savePathName.fileName().toLocal8Bit().constData()); //convert UTF8 wide char
            assert(cstring.size() < 64);
            processAiPicture picWrite(cstring,std::string((const char*)akpicSave.toLocal8Bit().constData()),this->m_layers.size(), loopVec.size());
            qDebug() << "cstring"<<QString::fromStdString(cstring);
            if (cnt)
            {
                cnt->makeCurrent(m_offscreenSurface);
            }
            else {
                this->makeCurrent();
            }
            int numTasks = 100;
            int yi = 0;

            QOpenGLFramebufferObjectFormat format;
            format.setSamples(1);


            for (auto it = loopVec.begin(); it != loopVec.end(); it++)
            {
                //progressDlg.setValue(((int)(*it) * 100 / (int)this->m_layers.size()));
                QCoreApplication::processEvents();
                m_layers_z_range = std::array<unsigned int, 2>{0, static_cast<unsigned int>(*it-1)};
                refresh_render_paths(true, false);

                //TODO: change the back
                std::array<float, 3> setPosition;
                std::array<float, 3> viewCenter;
                std::array<float, 3> camLookUp;
                setPosition[0] = 292.78;
                setPosition[2] = 2.07 + this->gcode_result.layer_offset[yi][2];//(*it) == 1 ? 52.07 + this->m_layers.get_z_at(*it-1) : 2.07 + this->m_layers.get_z_at(*it-1);
                setPosition[1] = 20.57 + this->gcode_result.layer_offset[yi][1];//+ this->m_sequential_view.current_position[1];
                viewCenter[0] = -0.866;
                viewCenter[1] = 0;
                viewCenter[2] = -0.5;
                camLookUp[0] = -0.5;
                camLookUp[1] = 0.0;
                camLookUp[2] = 0.866;
                this->set_look_at(std::move(setPosition), std::move(viewCenter), std::move(camLookUp));
                std::vector<float> roi_info = this->get_roi_info(2048,1192); //OpenGL coord  w  h px py
#ifdef  FULL_SCREEN
                m_fbo = new QOpenGLFramebufferObject(2048,1192, format);
#else
                m_fbo = new QOpenGLFramebufferObject(roi_info[0], roi_info[1], format);
#endif
                m_fbo->setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
                if (!m_fbo->isValid()) {
                    //qDebug() << tr("OpenGlOffscreenSurface::recreateFbo() - Failed to create background FBO!");
                    AkUtil::TDebug(QString("OpenGlOffscreenSurface::recreateFbo() - Failed to create background FBO!" ));
                    control::MessageDialog a("ERROR",QObject::tr("Failed to create background FBO. There may be insufficient memory."), control::MessageDialog::BUTTONFLAG::OK);
                    bool re = a.exec();
                    return ;
                }
                // clear framebuffer
                m_fbo->bind();
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                //add change
#ifdef  FULL_SCREEN
                glViewport(0, 0,2048,1192);
#else
                glViewport(-roi_info[2], -roi_info[3],2048,1192);
#endif

                m_scene3d->resizeWidget(2048, 1192);
                this->offPaint();

                QImage test = m_fbo->toImage();//grabFramebufferInternal(m_fbo);

                QByteArray arr;
                QBuffer buffer(&arr);
                buffer.open(QIODevice::WriteOnly);
                test.save(&buffer, "jpg");
                buffer.close();
                imgInfo tImgInfo;
                tImgInfo.data = arr.data();
                tImgInfo.size = arr.size();
                imageHead iHead;
                iHead.percentage = *it / this->m_layers.size();
                iHead.layer_num = *it;
                std::copy(roi_info.begin(), roi_info.end(), iHead.roi_info);
                iHead.file_size = tImgInfo.size;

                std::string  saveName = this->genSavePath(*it).toStdString();
                std::copy(saveName.begin(), saveName.end(), iHead.name);
                //qDebug() << iHead.name;
                picWrite.writeImage(iHead, tImgInfo);
                m_fbo->release();
                delete m_fbo;
                m_fbo = nullptr;
                yi++;
                emit setValue(((int)(*it) * 100 / ((int)this->m_layers.size()+1)));
                //this->doneCurrent();
            }
            //progressDlg.setValue(numTasks);


            //this->update();
        }
        refresh_render_paths(true, false);
    }
    if(QFile::exists(savePath))
    {
        QFile::remove(savePath);
    }

    //tar .akpic .gcode  TODO:need to add new libtar
    if(isAiMode){
        QFileInfo fileInfo = QFileInfo(_gPath);
        QFileInfo fileInfo1 = QFileInfo(akpicSave);
        pCmd = new QProcess();
        QString cmd;
        pCmd->setWorkingDirectory(fileInfo.absoluteDir().path());
        cmd = "tar";
        QStringList args;
#ifdef __APPLE__
        args.append("--disable-copyfile");
        args.append("--no-xattrs");
#endif
        args.append("--format=ustar"); 
        args.append("-cvf");
        args.append(savePath);
        args.append(fileInfo.fileName());
        args.append(fileInfo1.fileName());
        pCmd->start(cmd,args);
        pCmd->waitForStarted();
        qDebug() << "GcodeView fileName = " << args;
        QString debugstr;
        QDebug(&debugstr) <<"tar cmd  program: "<<pCmd->program();
        QDebug(&debugstr) <<"tar cmd  arguments: "<<pCmd->arguments();
        AkUtil::TDebug(debugstr);
        connect(pCmd,&QProcess::readyReadStandardError,this,[=]()
        {
            
            QString data=pCmd->readAllStandardError();
            AkUtil::TDebug("return  cmd : "+data);
        });
        pCmd->waitForFinished();
    }else{
        
        if(QFile::exists(QString::fromStdString(this->m_gcode_path)))
        {
            QFile::copy(QString::fromStdString(this->m_gcode_path), savePath);
        }else{
            AkUtil::TDebug("error : file not exit");
        }
        AkUtil::TDebug(QString("this->m_gcode_path " )+QString::fromStdString(this->m_gcode_path.c_str()));
        AkUtil::TDebug(QString("savePath " )+savePath);
    }
    emit setValue(100);
    //TODO: change the back
}

//TODO:no need
QImage GcodeViewer::grabFramebufferInternal(QOpenGLFramebufferObject * fbo)
{
    QImage image;
    // bind framebuffer first
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo->handle());
    glReadBuffer(GL_COLOR_ATTACHMENT0);

    GLenum internalFormat = fbo->format().internalTextureFormat();
    bool hasAlpha = internalFormat == GL_RGBA || internalFormat == GL_BGRA
            || internalFormat == GL_RGBA8;
    hasAlpha = false;
    if (internalFormat == GL_BGRA) {
        image = QImage(fbo->size(), hasAlpha ? QImage::Format_ARGB32 : QImage::Format_RGB32);
        glReadPixels(0, 0, fbo->size().width(),
                     fbo->size().height(), GL_BGRA, GL_UNSIGNED_BYTE, image.bits());
    }
    else if ((internalFormat == GL_RGBA) || (internalFormat == GL_RGBA8)) {
        image = QImage(fbo->size(), hasAlpha ? QImage::Format_RGBA8888 : QImage::Format_RGBX8888);
        glReadPixels(0, 0, fbo->size().width(),
                     fbo->size().height(), GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
    }
    else {
        qDebug() << "OpenGlOffscreenSurface::grabFramebuffer() - Unsupported framebuffer format"
                 << internalFormat << "!";
    }
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo->handle());

    return (image.mirrored());
}

void GcodeViewer::setRoleVisible(ExtrusionRole role, bool isVisible)
{
    m_extrusions.role_visibility_flags = isVisible ? m_extrusions.role_visibility_flags | (1 << role) : m_extrusions.role_visibility_flags & ~(1 << role);
}

void GcodeViewer::clearGcodeSource()
{
    if(QFile::exists(std::move(QString::fromStdString(this->m_gcode_path))))
    {
        QFile::remove(QString::fromStdString(this->m_gcode_path));
    }
    this->akpicSave.clear();
    
    this->resetGpu(true);
    reset();

}

void GcodeViewer::getTimeAndPercent()
{
   if(times.size() >=  m_roles.size())
   {
       return ;
   }
    const PrintEstimatedTimeStatistics::Mode& time_mode = m_time_statistics.modes[static_cast<size_t>(m_time_estimate_mode)];
    auto role_time_and_percent = [this, time_mode](ExtrusionRole role) {
        auto it = std::find_if(time_mode.roles_times.begin(), time_mode.roles_times.end(), [role](const std::pair<ExtrusionRole, float>& item) { return role == item.first; });
        return (it != time_mode.roles_times.end()) ? std::make_pair(it->second, it->second / time_mode.time) : std::make_pair(0.0f, 0.0f);
    };
    float max_percent = 0.0f;
    if (m_view_type == EViewType::FeatureType) {
        // calculate offsets to align time/percentage data
        for (size_t i = 0; i < m_roles.size(); ++i) {
            ExtrusionRole role = m_roles[i];
            if (role < erCount) {
                auto [time, percent] = role_time_and_percent(role);
                        times.push_back(time > 0.0f ? time : 0);
                        percents.push_back(percent);
                        max_percent = std::max(max_percent, percent);
            }
        }
    }
}





