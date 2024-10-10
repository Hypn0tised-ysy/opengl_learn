#version 330 core
//bling-phong shading
in vec3 color_result;
out vec4 FragColor;

void main()
{

    FragColor = vec4(color_result,1.0f);
}