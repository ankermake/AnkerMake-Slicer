#version 330 core

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 objColor;
uniform vec3 lightColor[10];
uniform vec3 lightPos[10];
uniform int lightNum;
uniform vec3 viewPos;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform bool useTexture;
uniform bool calLight;//是否计算光照
uniform bool twoSideRender;//是否双面渲染

out vec4 FragColor;

float calSumWeight(vec3 lightposition)
{
    float ambientStrength = 0.2/lightNum;
 
    vec3 norm = normalize(Normal);
    if(twoSideRender && !gl_FrontFacing)//双面
    {
       norm = -norm;
     }
    vec3 lightDir = normalize(lightposition - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
 
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    return ambientStrength + diff + specularStrength * spec;
}

void main(void)
{
    if(!calLight)
    {
        FragColor = vec4(objColor, 1.0);
        return;
     }

    vec3 allLight = calSumWeight(lightPos[0])*lightColor[0];
    for(int i=1;i<lightNum;i++)
    {
        allLight = allLight + calSumWeight(lightPos[i])*lightColor[i];
     }
    if(useTexture)
    {
        vec3 result = allLight * vec3(texture(ourTexture1, TexCoord));
        FragColor = vec4(result, 1.0);    
    }
    else
    {
        vec3 result = allLight * objColor;
        FragColor = vec4(result, 1.0);
    }
}
