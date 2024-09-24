#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 input_color;
out vec3 color_result;
void main()
{
gl_Position=vec4(aPos.x,-aPos.y,aPos.z,1.0);
color_result=input_color;
}