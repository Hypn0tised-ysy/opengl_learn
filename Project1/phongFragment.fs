#version 330 core
//bling-phong shading
in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;
uniform vec3 eyePos;
uniform vec3 lightPos;
uniform vec3 lightColor;
struct Material
{
    vec3 ambient;
    vec3 specular;
    vec3 diffuse;
    float shininess;//高光系数
};
struct Light
{
    vec3 lightPos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Material material;
uniform Light light;
void main()
{
    //ambient
    vec3 ambientColor=material.ambient*light.ambient;
    //diffuse
    vec3 normal=normalize(Normal);
    vec3 lightDir=normalize(light.lightPos-FragPos);
    float diffuseAngle=max(dot(normal,lightDir),0.0f);
    vec3 diffuseColor=diffuseAngle*light.diffuse*material.diffuse;
    //specular
    vec3 viewDir=normalize(eyePos-FragPos);
    vec3 reflectDir=reflect(-lightDir,normal);
    float specularAngle=max(dot(reflectDir,viewDir),0.0f);
    specularAngle=pow(specularAngle,material.shininess);
    vec3 specularColor=material.specular*specularAngle*light.specular;
    //ultimate color
    vec3 result=(ambientColor+diffuseColor+specularColor);
    FragColor = vec4(result,1.0f);
}