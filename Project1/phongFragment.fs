#version 330 core
//bling-phong shading
in vec3 Normal;
in vec3 FragPos;
in vec3 lightPosition;
out vec4 FragColor;
uniform float ambient;
uniform float specular;
uniform vec3 objectColor;
uniform vec3 lightColor;
void main()
{
    vec3 eyePos=vec3(0.0f);
    //ambient
    vec3 ambientColor=ambient*lightColor;
    //diffuse
    vec3 normal=normalize(Normal);
    vec3 lightDir=normalize(lightPosition-FragPos);
    float diffuseAngle=max(dot(normal,lightDir),0.0f);
    vec3 diffuseColor=diffuseAngle*lightColor;
    //specular
    vec3 viewDir=normalize(eyePos-FragPos);
    vec3 reflectDir=reflect(-lightDir,normal);
    float specularAngle=max(dot(reflectDir,viewDir),0.0f);
    specularAngle=pow(specularAngle,32);
    vec3 specularColor=specular*specularAngle*lightColor;
    //ultimate color
    vec3 result=(ambientColor+diffuseColor+specularColor)*objectColor;
    FragColor = vec4(result,1.0f);
}