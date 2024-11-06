#include "renderer.h"

#include "shader.h"
#include "indexBuffer.h"
#include "vertexArray.h"

void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool GLPrintError(const char *fn, const char *file, int line) {
    // ReSharper disable once CppDFALoopConditionNotUpdated
    while (const GLenum error = glGetError()) {
        std::cout << "[OpenGL Error " << error << "] in function: " << fn << " at line: " << line << " and file: " <<
                file <<
                std::endl;
        return false;
    };
    return true;
};

void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib, Shader &shader) {
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall(
        glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, reinterpret_cast<void *>(0 * sizeof(unsigned int))
        ));
}

void Renderer::Clear() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


