#version 330 core
in vec3 color_position;
out vec4 FragColor;
uniform vec4 breath;
void main()
{
FragColor=vec4(color_position,1.0f);
}