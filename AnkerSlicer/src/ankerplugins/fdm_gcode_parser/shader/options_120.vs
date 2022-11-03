#version 330 core
layout (location = 0) in vec3 aPos;

#define INTENSITY_CORRECTION 0.6
//->0.001

// normalized values for (-0.6/1.31, 0.6/1.31, 1./1.31)
const vec3 LIGHT_TOP_DIR = vec3(-0.4574957, 0.4574957, 0.7624929);
#define LIGHT_TOP_DIFFUSE    (0.8 * INTENSITY_CORRECTION)
#define LIGHT_TOP_SPECULAR   (0.45 * INTENSITY_CORRECTION)
#define LIGHT_TOP_SHININESS  3.0

// normalized values for (1./1.43, 0.2/1.43, 1./1.43)
const vec3 LIGHT_FRONT_DIR = vec3(0.6985074, 0.1397015, 0.6985074);
#define LIGHT_FRONT_DIFFUSE  (0.3 * INTENSITY_CORRECTION)

#define INTENSITY_AMBIENT    0.3 //0.3
//->0.6

// x = tainted, y = specular;
out vec2 intensity;

//out VS_OUT {
//    vec2 intensity;
//} vs_out;

uniform mat4 mv_matrix;
uniform mat3 normal_matrix;
uniform mat4 p_matrix;
const float radius = 1.2;

void main()
{
    gl_Position = p_matrix*mv_matrix*vec4(aPos,1.0);
    //gl_PointSize = 50.0;
    gl_PointSize = 500.0 * p_matrix[1][1] * radius / gl_Position.w;
}
