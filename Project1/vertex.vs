#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec2 texture;
layout(location=2) in vec3 input_color;
uniform float horizontal_offset;
uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 color_output;
out vec2 texture_coord;
void main()
{
gl_Position=projection*view*model*vec4(aPos,1.0);
color_output=input_color;
texture_coord=texture;
}