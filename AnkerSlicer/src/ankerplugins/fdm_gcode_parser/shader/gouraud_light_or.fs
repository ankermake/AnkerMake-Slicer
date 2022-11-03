#version 330 core

uniform vec4 uniform_color;

// x = tainted, y = specular;
in vec2 intensity;
out vec4 FragColor;

void main()
{
    FragColor = vec4(vec3(intensity.y, intensity.y, intensity.y) + uniform_color.rgb * intensity.x, 1.0);
}
