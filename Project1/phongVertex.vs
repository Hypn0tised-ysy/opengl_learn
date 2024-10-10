#version 330 core
//bling-phong shading
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 normal;
uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float ambient;
uniform float specular;
uniform vec3 eyePos;
uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
out vec3 Normal;
out vec3 FragPos;
out vec3 color_result;
void main()
{
gl_Position=projection*view*model*vec4(aPos,1.0);
FragPos=vec3(model*vec4(aPos,1.0f));
//better calculate the inverse operation via CPU then send it to GPU
Normal = mat3(transpose(inverse(model))) * normal;
    //ambient
    vec3 ambientColor=ambient*lightColor;
    //diffuse
    vec3 normal=normalize(Normal);
    vec3 lightDir=normalize(lightPos-FragPos);
    float diffuseAngle=max(dot(normal,lightDir),0.0f);
    vec3 diffuseColor=diffuseAngle*lightColor;
    //specular
    vec3 viewDir=normalize(eyePos-FragPos);
    vec3 reflectDir=reflect(-lightDir,normal);
    float specularAngle=max(dot(reflectDir,viewDir),0.0f);
    specularAngle=pow(specularAngle,32);
    vec3 specularColor=specular*specularAngle*lightColor;
    //ultimate color
    color_result=(ambientColor+diffuseColor+specularColor)*objectColor;
}