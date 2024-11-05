#include "renderer.h"

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

