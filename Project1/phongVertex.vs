#version 330 core
//bling-phong shading
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 normal;
uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;
out vec3 Normal;
out vec3 FragPos;
out vec3 lightPosition;
void main()
{
gl_Position=projection*view*model*vec4(aPos,1.0);
lightPosition=vec3(view*vec4(lightPos,1.0f));
FragPos=vec3(view*model*vec4(aPos,1.0f));
//better calculate the inverse operation via CPU then send it to GPU
Normal = mat3(transpose(inverse(view*model))) * normal;
}