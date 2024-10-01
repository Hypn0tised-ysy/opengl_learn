#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 input_color;
layout(location=2) in vec2 texture;
uniform float horizontal_offset;
out vec3 color_output;
out vec2 texture_coord;
void main()
{
gl_Position=vec4(aPos.x,aPos.y,aPos.z,1.0);
color_output=input_color;
texture_coord=texture;
}