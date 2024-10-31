#shader vertex
#version 330 core
layout (location = 0) in vec4 pos;
void main()
{
    gl_Position = pos;
}

#shader fragment
#version 330 core
out vec4 color;
void main()
{
    color = vec4(0.8, 0.2, 0.5, 1.0);
}
        
