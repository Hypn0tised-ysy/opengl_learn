#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 input_color;
uniform float horizontal_offset;
out vec3 color_position;
void main()
{
gl_Position=vec4(aPos.x,aPos.y,aPos.z,1.0);
color_position=vec3(aPos);
}