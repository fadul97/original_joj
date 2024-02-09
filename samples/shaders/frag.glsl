#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform vec4 rect;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    vec2 adjustedTexCoord = mix(rect.xy, rect.zw, TexCoord);
    FragColor = texture(texture2, adjustedTexCoord);
}
