#version 330

out vec3 eye_normal;

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aNormal;
uniform mat4 mv_matrix;
uniform mat3 normal_matrix;
uniform mat4 p_matrix;
vec3 world_normal()
{
    // the world normal is always parallel to the world XY plane
    // the x component is stored into gl_Vertex.w
    float x = aPos.w;
    float y = sqrt(1.0 - x * x);
    return vec3(x, y, 0.0);
}

void main()
{
    vec4 world_position = vec4(aPos.xyz, 1.0);
    gl_Position = p_matrix*mv_matrix* world_position;
    eye_normal = normal_matrix * world_normal();    
}
