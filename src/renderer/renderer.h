#ifndef RENDERER_H
#define RENDERER_H
#define GLFW_INCLUDE_NONE
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include "glad/glad.h"
#include <iostream>

#define ASSERT(x) if(!(x)) __builtin_unreachable();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLPrintError(#x, __FILE__, __LINE__))

class Shader;
class IndexBuffer;
class VertexArray;

void GLClearError();

bool GLPrintError(const char *fn, const char *file, int line);

class Renderer {
public:
    void Clear() const;

    void Draw(const VertexArray &vao, const IndexBuffer &ib, Shader &shader);
};


#endif //RENDERER_H
