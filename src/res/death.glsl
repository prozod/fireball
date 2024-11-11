#shader vertex
#version 330 core
layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 u_texcoord;

uniform mat4 u_mvp;
out vec2 texcoord;

void main() {
    texcoord = u_texcoord;
    gl_Position = u_mvp * vec4(pos, 0.0, 1.0);
}


#shader fragment
#version 330 core
out vec4 color;

uniform vec4 u_color;

void main() {
    color = u_color;
}
