#shader vertex
#version 330 core

layout (location = 0) in vec4 pos;
layout (location = 1) in vec2 texcoord;

uniform mat4 u_mvp;

out vec2 v_texcoord;

void main()
{
    gl_Position = u_mvp * pos;
    v_texcoord = texcoord;
}

#shader fragment
#version 330 core

out vec4 color;

in vec2 v_texcoord;

uniform sampler2D u_bg_texture;

void main()
{
    vec4 texColor = texture(u_bg_texture, v_texcoord);
    color = texColor;
}


