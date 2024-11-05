#shader vertex
#version 330 core
layout (location = 0) in vec4 pos;
uniform mat4 transform;
void main()
{
    gl_Position = transform * pos;
}

#shader fragment
#version 330 core
out vec4 color;
uniform vec4 u_color;
void main()
{
    color = u_color;
}
        
