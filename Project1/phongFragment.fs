#version 330 core
//bling-phong shading
in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;
uniform float ambient;
uniform float specular;
uniform vec3 eyePos;
uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
void main()
{
    //ambient
    vec3 ambientColor=ambient*lightColor;
    //diffuse
    vec3 normal=normalize(Normal);
    vec3 lightDir=normalize(lightPos-FragPos);
    float diffuseAngle=max(dot(normal,lightDir),0.0f);
    vec3 diffuseColor=diffuseAngle*lightColor;
    //specular
    vec3 viewDir=normalize(eyePos-FragPos);
    float specularAngle=max(dot(normal,viewDir),0.0f);
    vec3 specularColor=specular*specularAngle*lightColor;
    vec3 result=(ambientColor+diffuseColor+specularColor)*objectColor;
    FragColor = vec4(result,1.0f);
}