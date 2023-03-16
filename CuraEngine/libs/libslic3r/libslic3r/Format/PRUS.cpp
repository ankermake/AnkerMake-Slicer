#include <string.h>
#include <exception>

#include <boost/algorithm/string.hpp>
#include <boost/nowide/convert.hpp>
#include <boost/nowide/cstdio.hpp>

#include "miniz_extension.hpp"

#include <Eigen/Geometry>

#include "../libslic3r.h"
#include "../Model.hpp"

#include "PRUS.hpp"

#if 0
// Enable debugging and assert in this file.
#define DEBUG
#define _DEBUG
#undef NDEBUG
#endif

#include <assert.h>

namespace Slic3r
{

struct StlHeader
{
    char        comment[80];
    uint32_t    nTriangles;
};

static_assert(sizeof(StlHeader) == 84, "StlHeader size not correct");

// Buffered line reader to a string buffer.
class LineReader
{
public:
    LineReader(std::vector<char> &data) : m_buffer(data), m_pos(0), m_len((int)data.size()) {}

    const char* next_line() {
        // Skip empty lines.
        while (m_pos < m_len && (m_buffer[m_pos] == '\r' || m_buffer[m_pos] == '\n'))
            ++ m_pos;
        if (m_pos == m_len) {
            // End of file.
            return nullptr;
        }
        // The buffer is nonempty and it does not start with end of lines. Find the first end of line.
        int end = m_pos + 1;
        while (end < m_len && m_buffer[end] != '\r' && m_buffer[end] != '\n')
            ++ end;
        char *ptr_out = m_buffer.data() + m_pos;
        m_pos = end + 1;
        m_buffer[end] = 0;
        return ptr_out;
    }

    int next_line_scanf(const char *format, ...)
    {
        const char *line = next_line();
        if (line == nullptr)
            return -1;
        int result;
        va_list arglist;
        va_start(arglist, format);
        result = vsscanf(line, format, arglist);
        va_end(arglist);
        return result;
    }

private:
    std::vector<char>  &m_buffer;
    int                 m_pos;
    int                 m_len;
};

static void extract_model_from_archive(
    // name of the model file
    const char              *name,
    // path to the archive
    const char              *path,
    // content of scene.xml
    const std::vector<char> &scene_xml_data,
    // loaded data of this STL
    std::vector<char>       &data,
    // Model, to which the newly loaded objects will be added
    Model                   *model,
    // To map multiple STLs into a single model object for multi-material prints.
    std::map<int, ModelObject*> &group_to_model_object)
{
    // Find the model entry in the XML data.
    char model_name_tag[1024];
    sprintf(model_name_tag, "<model name=\"%s\">", name);
    const char *model_xml = strstr(scene_xml_data.data(), model_name_tag);
    const char *zero_tag  = "<zero>";
    const char *zero_xml  = strstr(scene_xml_data.data(), zero_tag);
    Vec3d instance_rotation = Vec3d::Zero();
    Vec3d instance_scaling_factor = Vec3d::Ones();
    Vec3d instance_offset = Vec3d::Zero();
    bool         trafo_set    = false;
    unsigned int group_id     = (unsigned int)-1;
    unsigned int extruder_id  = (unsigned int)-1;
    ModelObject *model_object = nullptr;
    if (model_xml != nullptr) {
        model_xml += strlen(model_name_tag);
        const char *position_tag = "<position>";
        const char *position_xml = strstr(model_xml, position_tag);
        const char *rotation_tag = "<rotation>";
        const char *rotation_xml = strstr(model_xml, rotation_tag);
        const char *scale_tag    = "<scale>";
        const char *scale_xml    = strstr(model_xml, scale_tag);
        float position[3], rotation[3], scale[3], zero[3];
        if (position_xml != nullptr && rotation_xml != nullptr && scale_xml != nullptr && zero_xml != nullptr &&
            sscanf(position_xml+strlen(position_tag), 
                "[%f, %f, %f]", position, position+1, position+2) == 3 &&
            sscanf(rotation_xml+strlen(rotation_tag), 
                "[%f, %f, %f]", rotation, rotation+1, rotation+2) == 3 &&
            sscanf(scale_xml+strlen(scale_tag),
                "[%f, %f, %f]", scale, scale+1, scale+2) == 3 &&
            sscanf(zero_xml+strlen(zero_tag), 
                "[%f, %f, %f]", zero, zero+1, zero+2) == 3) {
            instance_scaling_factor = Vec3d((double)scale[0], (double)scale[1], (double)scale[2]);
            instance_rotation = Vec3d(-(double)rotation[0], -(double)rotation[1], -(double)rotation[2]);
            instance_offset = Vec3d((double)(position[0] - zero[0]), (double)(position[1] - zero[1]), (double)(position[2] - zero[2]));
            trafo_set = true;
        }
        const char *group_tag    = "<group>";
        const char *group_xml    = strstr(model_xml, group_tag);
        const char *extruder_tag = "<extruder>";
        const char *extruder_xml = strstr(model_xml, extruder_tag);
        if (group_xml != nullptr) {
            int group = atoi(group_xml + strlen(group_tag));
            if (group > 0) {
                group_id = group;
                auto it = group_to_model_object.find(group_id);
                if (it != group_to_model_object.end())
                    model_object = it->second;
            }
        }
        if (extruder_xml != nullptr) {
            int e = atoi(extruder_xml + strlen(extruder_tag));
            if (e > 0)
            extruder_id = e;
        }
    }
    if (! trafo_set)
        throw Slic3r::FileIOError(std::string("Archive ") + path + " does not contain a valid entry in scene.xml for " + name);

    // Extract the STL.
    StlHeader header;
    TriangleMesh mesh;
    bool mesh_valid = false;
    bool stl_ascii  = false;
    if (data.size() > sizeof(StlHeader)) {
        memcpy((char*)&header, data.data(), sizeof(StlHeader));
        if (strncmp(header.comment, "solid ", 6) == 0)
            stl_ascii = true;
        else {
            // Header has been extracted. Now read the faces.
            stl_file &stl = mesh.stl;
            stl.stats.type = inmemory;
            stl.stats.number_of_facets = header.nTriangles;
            stl.stats.original_num_facets = header.nTriangles;
            stl_allocate(&stl);
            if (header.nTriangles > 0 && data.size() == 50 * header.nTriangles + sizeof(StlHeader)) {
                memcpy((char*)stl.facet_start.data(), data.data() + sizeof(StlHeader), 50 * header.nTriangles);
                if (sizeof(stl_facet) > SIZEOF_STL_FACET) {
                    // The stl.facet_start is not packed tightly. Unpack the array of stl_facets.
                    unsigned char *data = (unsigned char*)stl.facet_start.data();
                    for (size_t i = header.nTriangles - 1; i > 0; -- i)
                        memmove(data + i * sizeof(stl_facet), data + i * SIZEOF_STL_FACET, SIZEOF_STL_FACET);
                }
                // All the faces have been read.
                stl_get_size(&stl);
                mesh.repair();
                if (std::abs(stl.stats.min(2)) < EPSILON)
                    stl.stats.min(2) = 0.;
                // Add a mesh to a model.
                if (mesh.facets_count() > 0)
                    mesh_valid = true;
            }
        }
    } else
        stl_ascii = true;

    if (stl_ascii) {
        // Try to parse ASCII STL.
        char                    normal_buf[3][32];
        stl_facet               facet;
        std::vector<stl_facet>  facets;
        LineReader              line_reader(data);
        std::string             solid_name;
        facet.extra[0] = facet.extra[1] = 0;
        for (;;) {
            const char *line = line_reader.next_line();
            if (line == nullptr)
                // End of file.
                break;
            if (strncmp(line, "solid", 5) == 0) {
                // Opening the "solid" block.
                if (! solid_name.empty()) {
                    // Error, solid block is already open.
                    facets.clear();
                    break;
                }
                solid_name = line + 5;
                if (solid_name.empty())
                    solid_name = "unknown";
                continue;
            }
            if (strncmp(line, "endsolid", 8) == 0) {
                // Closing the "solid" block.
                if (solid_name.empty()) {
                    // Error, no solid block is open.
                    facets.clear();
                    break;
                }
                solid_name.clear();
                continue;
            }
            // Line has to start with the word solid.
            int res_normal      = sscanf(line, " facet normal %31s %31s %31s", normal_buf[0], normal_buf[1], normal_buf[2]);
            assert(res_normal == 3);
            int res_outer_loop  = line_reader.next_line_scanf(" outer loop");
            assert(res_outer_loop == 0);
            int res_vertex1 = line_reader.next_line_scanf(" vertex %f %f %f", &facet.vertex[0](0), &facet.vertex[0](1), &facet.vertex[0](2));
            assert(res_vertex1 == 3);
            int res_vertex2 = line_reader.next_line_scanf(" vertex %f %f %f", &facet.vertex[1](0), &facet.vertex[1](1), &facet.vertex[1](2));
            assert(res_vertex2 == 3);
            int res_vertex3 = line_reader.next_line_scanf(" vertex %f %f %f", &facet.vertex[2](0), &facet.vertex[2](1), &facet.vertex[2](2));
            assert(res_vertex3 == 3);
            int res_endloop = line_reader.next_line_scanf(" endloop");
            assert(res_endloop == 0);
            int res_endfacet = line_reader.next_line_scanf(" endfacet");
            if (res_normal != 3 || res_outer_loop != 0 || res_vertex1 != 3 || res_vertex2 != 3 || res_vertex3 != 3 || res_endloop != 0 || res_endfacet != 0) {
                // perror("Something is syntactically very wrong with this ASCII STL!");
                facets.clear();
                break;
            }
            // The facet normal has been parsed as a single string as to workaround for not a numbers in the normal definition.
            if (sscanf(normal_buf[0], "%f", &facet.normal(0)) != 1 ||
                sscanf(normal_buf[1], "%f", &facet.normal(1)) != 1 ||
                sscanf(normal_buf[2], "%f", &facet.normal(2)) != 1) {
                // Normal was mangled. Maybe denormals or "not a number" were stored?
                // Just reset the normal and silently ignore it.
                facet.normal = stl_normal::Zero();
            }
            facets.emplace_back(facet);
        }
        if (! facets.empty() && solid_name.empty()) {
            stl_file &stl = mesh.stl;
            stl.stats.type = inmemory;
            stl.stats.number_of_facets = (uint32_t)facets.size();
            stl.stats.original_num_facets = (int)facets.size();
            stl_allocate(&stl);
            memcpy((void*)stl.facet_start.data(), facets.data(), facets.size() * 50);
            stl_get_size(&stl);
            mesh.repair();
            // Add a mesh to a model.
            if (mesh.facets_count() > 0)
                mesh_valid = true;
        }
    }

    if (! mesh_valid)
        throw Slic3r::FileIOError(std::string("Archive ") + path + " does not contain a valid mesh for " + name);

    // Add this mesh to the model.
    ModelVolume *volume = nullptr;
    if (model_object == nullptr) {
        // This is a first mesh of a group. Create a new object & volume.
        model_object = model->add_object(name, path, std::move(mesh));
        volume = model_object->volumes.front();
        ModelInstance *instance  = model_object->add_instance();
        instance->set_rotation(instance_rotation);
        instance->set_scaling_factor(instance_scaling_factor);
        instance->set_offset(instance_offset);
        if (group_id != (unsigned int)(-1))
            group_to_model_object[group_id] = model_object;
    } else {
        // This is not the 1st mesh of a group. Add it to the ModelObject.
        volume = model_object->add_volume(std::move(mesh));
        volume->name = name;
    }
    // Set the extruder to the volume.
    if (extruder_id != (unsigned int)-1)
        volume->config.set("extruder", int(extruder_id));
}

// Load a PrusaControl project file into a provided model.
bool load_prus(const char *path, Model *model)
{
    mz_zip_archive archive;
    mz_zip_zero_struct(&archive);

    size_t  n_models_initial = model->objects.size();
    mz_bool res              = MZ_FALSE;
    try {
        if (!open_zip_reader(&archive, path))
            throw Slic3r::FileIOError(std::string("Unable to init zip reader for ") + path);
        std::vector<char>           scene_xml_data;
        // For grouping multiple STLs into a single ModelObject for multi-material prints.
        std::map<int, ModelObject*> group_to_model_object;
        mz_uint                     num_entries = mz_zip_reader_get_num_files(&archive);
        for (mz_uint i = 0; i < num_entries; ++ i) {
            mz_zip_archive_file_stat stat;
            if (! mz_zip_reader_file_stat(&archive, i, &stat))
                continue;
            std::vector<char> buffer;
            buffer.assign((size_t)stat.m_uncomp_size, 0);
            res = mz_zip_reader_extract_file_to_mem(&archive, stat.m_filename, (char*)buffer.data(), (size_t)stat.m_uncomp_size, 0);
            if (res == MZ_FALSE)
                throw Slic3r::FileIOError(std::string("Error while extracting a file from ") + path);
            if (strcmp(stat.m_filename, "scene.xml") == 0) {
                if (! scene_xml_data.empty())
                    throw Slic3r::FileIOError(std::string("Multiple scene.xml were found in the archive.") + path);
                scene_xml_data = std::move(buffer);
            } else if (boost::iends_with(stat.m_filename, ".stl")) {
                // May throw std::exception
                extract_model_from_archive(stat.m_filename, path, scene_xml_data, buffer, model, group_to_model_object);
            }
        }
    } catch (std::exception &ex) {
        close_zip_reader(&archive);
        throw ex;
    }

    close_zip_reader(&archive);
    return model->objects.size() > n_models_initial;
}

}; // namespace Slic3r
