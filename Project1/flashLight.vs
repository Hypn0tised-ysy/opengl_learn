#version 330 core
//bling-phong shading
layout(location=0) in vec3 aPos;//a:attribute
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTexCoord;
uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;
void main()
{
gl_Position=projection*view*model*vec4(aPos,1.0);
FragPos=vec3(model*vec4(aPos,1.0f));
//better calculate the inverse operation via CPU then send it to GPU
Normal = mat3(transpose(inverse(model))) * aNormal;
TexCoord=aTexCoord;
}