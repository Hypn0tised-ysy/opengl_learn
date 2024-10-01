#version 330 core
in vec3 color_output;
in vec2 texture_coord;
out vec4 FragColor;
uniform vec4 breath;
uniform sampler2D texture1;
uniform sampler2D texture2;
void main()
{
FragColor=mix(texture(texture1,texture_coord)*vec4(color_output,1.0f),texture(texture2,texture_coord),0.3);
}