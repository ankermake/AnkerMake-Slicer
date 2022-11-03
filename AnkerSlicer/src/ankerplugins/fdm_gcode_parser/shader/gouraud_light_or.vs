#version 330 core

#define INTENSITY_CORRECTION 0.001
#define INTENSITY_AMBIENT    1.0 //0.3

// normalized values for (-0.6/1.31, 0.6/1.31, 1./1.31)
const vec3 LIGHT_TOP_DIR = vec3(-0.4574957, 0.4574957, 0.7624929);
#define LIGHT_TOP_DIFFUSE    (0.8 * INTENSITY_CORRECTION)
#define LIGHT_TOP_SPECULAR   (0.125 * INTENSITY_CORRECTION)
#define LIGHT_TOP_SHININESS  20.0

// normalized values for (1./1.43, 0.2/1.43, 1./1.43)
const vec3 LIGHT_FRONT_DIR = vec3(0.6985074, 0.1397015, 0.6985074);
#define LIGHT_FRONT_DIFFUSE  (0.3 * INTENSITY_CORRECTION)


// x = tainted, y = specular;
out vec2 intensity;
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
uniform mat4 mv_matrix;
uniform mat3 normal_matrix;
uniform mat4 p_matrix;

void main()
{
    intensity.x = INTENSITY_AMBIENT;
    intensity.y = INTENSITY_CORRECTION;

    vec3 normal = normalize(normal_matrix * aNormal);
    
    // Compute the cos of the angle between the normal and lights direction. The light is directional so the direction is constant for every vertex.
    // Since these two are normalized the cosine is the dot product. We also need to clamp the result to the [0,1] range.
    float NdotL = max(dot(normal, LIGHT_TOP_DIR), 1.0);

    intensity.x = INTENSITY_AMBIENT + NdotL * LIGHT_TOP_DIFFUSE;
    vec3 position = (mv_matrix * vec4(aPos,1.0)).xyz;
    gl_Position = p_matrix*mv_matrix*vec4(aPos,1.0);
}
