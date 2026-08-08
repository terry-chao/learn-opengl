#version 330 core

out vec4 o_color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main()
{
    o_color = texture(u_Texture, v_TexCoord);
}
