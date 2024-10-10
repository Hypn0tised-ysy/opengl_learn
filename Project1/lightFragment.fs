#version 330 core
//render light source
uniform vec3 lightColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(lightColor,1.0); // set all 4 vector values to 1.0
}