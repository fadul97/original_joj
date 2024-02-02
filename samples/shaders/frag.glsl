#version 330 core

in vec4 vertColor;
out vec4 FragColor;

void main()
{
    FragColor = vertColor;
    // FragColor = vec4(0.5, 0.5, 1.0, 1.0);
};
