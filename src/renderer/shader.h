#ifndef SHADER_H
#define SHADER_H
#include <linmath.h>
#include <string>
#include <glad/glad.h>
#include <unordered_map>

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader {
public:
    unsigned int programID{};

    explicit Shader(const std::string &shaderPath);

    ~Shader();

    void Bind() const;

    void Unbind() const;

    // set uniforms
    void SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3);

    void SetUniformMatrix4fv(const std::string &name, const GLfloat *matrix, bool transpose = false);

    unsigned int GetUniformLocation(const std::string &name);

private:
    static ShaderProgramSource ParseShader(const std::string &shaderPath);

    void CreateShader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);

    static unsigned int CompileShader(unsigned int type, const std::string &source);

    std::unordered_map<std::string, unsigned int> uniformLocationCache;
};

#endif //SHADER_H
