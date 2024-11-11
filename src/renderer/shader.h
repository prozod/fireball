#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <glad/glad.h>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
    void SetUniform2f(const std::string &name, float v0, float v1);

    void SetUniform1i(const std::string &name, int value);

    void SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3);

    void SetUniformMatrix4fv(const std::string &name, const glm::mat4 &matrix, bool transpose = false);

    unsigned int GetUniformLocation(const std::string &name);

private:
    static ShaderProgramSource ParseShader(const std::string &shaderPath);

    void CreateShader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);

    static unsigned int CompileShader(unsigned int type, const std::string &source);

    std::unordered_map<std::string, unsigned int> uniformLocationCache;
};

#endif //SHADER_H
