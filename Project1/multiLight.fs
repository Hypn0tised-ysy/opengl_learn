#version 330 core
//bling-phong shading
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;
out vec4 FragColor;
uniform vec3 eyePos;
uniform vec3 lightColor;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;//高光系数
};
struct FlashLight//
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    //for attenuation
    float constant;
    float linear;
    float quadratic;
    //falsh light
    float cutOff;//cos(phi)
    float outerCutOff;//cos(phi+gamma)
    vec3 lightPos;
    vec3 spotDir;//看向的方向,与viewer的front方向相同
};
struct DirLight//
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight//点光源
{
    vec3 lightPos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    //for attenuation
    float constant;
    float linear;
    float quadratic;
};
#define NR_POINT_LIGHTS 4//
uniform Material material;
uniform FlashLight flashLight;
uniform DirLight dirLight;
uniform PointLight pointLight[NR_POINT_LIGHTS];
vec3 CalDirLight(vec3 normal,vec3 viewDir,DirLight dirLight);
vec3 CalPointLight(vec3 normal,vec3 viewDir,PointLight pointLight);
vec3 CalFlashLight(vec3 normal, vec3 viewDir,FlashLight flashLight);
void main()
{
    vec3 viewDir=normalize(eyePos-FragPos);
    vec3 normal=normalize(Normal);
    vec3 result=vec3(0.0f,0.0f,0.0f);
    for(int i=0;i<NR_POINT_LIGHTS;i++)
    {
        result+=CalPointLight(normal,viewDir,pointLight[i]);
    }
    result+=CalDirLight(normal,viewDir,dirLight);
    result+=CalFlashLight(normal,viewDir,flashLight);
    FragColor = vec4(result,1.0f);
}
vec3 CalDirLight(vec3 normal,vec3 viewDir,DirLight dirLight)
{
    vec3 diffuse=texture(material.diffuse,TexCoord).rgb;
    vec3 ambient=texture(material.diffuse,TexCoord).rgb;
    vec3 specular=texture(material.specular,TexCoord).rgb;
    vec3 lightDir=-normalize(dirLight.direction);
    //ambient
    vec3 ambientColor=ambient*dirLight.ambient;
    //diffuse
    float diffuseAngle=max(dot(normal,lightDir),0.0f);
    vec3 diffuseColor=diffuseAngle*dirLight.diffuse*diffuse;
    //specular
    vec3 reflectDir=reflect(-lightDir,normal);
    float specularAngle=max(dot(reflectDir,viewDir),0.0f);
    specularAngle=pow(specularAngle,material.shininess);
    vec3 specularColor=specular*specularAngle*dirLight.specular;
    //ultimate color
    vec3 result=(ambientColor+diffuseColor+specularColor);
    return result;
}
vec3 CalPointLight(vec3 normal,vec3 viewDir,PointLight pointLight)
{
    vec3 diffuse=texture(material.diffuse,TexCoord).rgb;
    vec3 ambient=texture(material.diffuse,TexCoord).rgb;
    vec3 specular=texture(material.specular,TexCoord).rgb;
    float distance=length(FragPos-pointLight.lightPos);
    float attenuation=1/(pointLight.constant+distance*pointLight.linear+pointLight.quadratic*distance*distance);
    //ambient
    vec3 ambientColor=ambient*pointLight.ambient;
    //diffuse
    vec3 lightDir=normalize(pointLight.lightPos-FragPos);
    float diffuseAngle=max(dot(normal,lightDir),0.0f);
    vec3 diffuseColor=diffuseAngle*pointLight.diffuse*diffuse;
    //specular
    vec3 reflectDir=reflect(-lightDir,normal);
    float specularAngle=max(dot(reflectDir,viewDir),0.0f);
    specularAngle=pow(specularAngle,material.shininess);
    vec3 specularColor=specular*specularAngle*pointLight.specular;
    //ultimate color
    vec3 result=attenuation*(ambientColor+diffuseColor+specularColor);
    return result;
}
vec3 CalFlashLight(vec3 normal, vec3 viewDir,FlashLight flashLight)
{
    vec3 diffuse=texture(material.diffuse,TexCoord).rgb;
    vec3 ambient=texture(material.diffuse,TexCoord).rgb;
    vec3 specular=texture(material.specular,TexCoord).rgb;
    //
    float distance=length(FragPos-flashLight.lightPos);
    float attenuation=1/(flashLight.constant+distance*flashLight.linear+flashLight.quadratic*distance*distance);
    //
    vec3 lightDir=normalize(flashLight.lightPos-FragPos);

    float theta     = dot(-normalize(flashLight.spotDir), lightDir);
    float epsilon   = flashLight.cutOff - flashLight.outerCutOff;
    float intensity = clamp((theta - flashLight.outerCutOff) / epsilon, 0.0, 1.0);
    //ambient
    vec3 ambientColor=ambient*flashLight.ambient;
    //diffuse

    float diffuseAngle=max(dot(normal,lightDir),0.0f);
    vec3 diffuseColor=diffuseAngle*flashLight.diffuse*diffuse;
    diffuseColor*=intensity;
    //specular

    vec3 reflectDir=reflect(-lightDir,normal);
    float specularAngle=max(dot(reflectDir,viewDir),0.0f);
    specularAngle=pow(specularAngle,material.shininess);
    vec3 specularColor=specular*specularAngle*flashLight.specular;
    specularColor*=intensity;
    //ultimate color
    vec3 result=attenuation*(ambientColor+diffuseColor+specularColor);
    return result;
}