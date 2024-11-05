#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <glad/glad.h>

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader {
public:
    static unsigned int LoadShader(const std::string &shaderPath);

    ~Shader();

private:
    static ShaderProgramSource ParseShader(const std::string &shaderPath);

    static unsigned int CreateShader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);

    static unsigned int CompileShader(unsigned int type, const std::string &source);
};

#endif //SHADER_H
