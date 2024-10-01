#version 330 core
in vec3 color_output;
in vec2 texture_coord;
out vec4 FragColor;
uniform vec4 breath;
uniform sampler2D tex;
void main()
{
FragColor=texture(tex,texture_coord)*vec4(color_output,1.0f);
}