#version 330 core
//bling-phong shading
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;
out vec4 FragColor;
uniform vec3 eyePos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float matrixMove;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emit;
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
    vec3 diffuse=texture(material.diffuse,TexCoord).rgb;
    vec3 ambient=texture(material.diffuse,TexCoord).rgb;
    vec3 specular=vec3(1.0f)-texture(material.specular,TexCoord).rgb;
    //emission
    float specular_value = texture(material.specular, TexCoord).r;
    vec3 emission = specular_value != 0 ? texture(material.emit,vec2(TexCoord.x,TexCoord.y+matrixMove)).rgb : vec3(0.0f);
    //ambient
    vec3 ambientColor=ambient*light.ambient;
    //diffuse
    vec3 normal=normalize(Normal);
    vec3 lightDir=normalize(light.lightPos-FragPos);
    float diffuseAngle=max(dot(normal,lightDir),0.0f);
    vec3 diffuseColor=diffuseAngle*light.diffuse*diffuse;
    //specular
    vec3 viewDir=normalize(eyePos-FragPos);
    vec3 reflectDir=reflect(-lightDir,normal);
    float specularAngle=max(dot(reflectDir,viewDir),0.0f);
    specularAngle=pow(specularAngle,material.shininess);
    vec3 specularColor=specular*specularAngle*light.specular;
    //ultimate color
    vec3 result=(ambientColor+diffuseColor+specularColor+emission);
    FragColor = vec4(result,1.0f);
}