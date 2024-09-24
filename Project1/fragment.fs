#version 330 core
in vec3 color_result;
out vec4 FragColor;
uniform vec4 breath;
void main()
{
FragColor=vec4(color_result,1.0f);
}