#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat3 norTranMatrix;
uniform mat4 pvmMatrix;
uniform mat4 mMatrix;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

void main(){
  gl_Position = pvmMatrix * vec4(aPos, 1.0);
  TexCoord = aTexCoord;
  FragPos = vec3(mMatrix * vec4(aPos, 1.0));
  Normal = norTranMatrix * aNormal;
}
