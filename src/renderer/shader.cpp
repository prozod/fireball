#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "renderer.h"

void Shader::CreateShader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource) {
    const unsigned int program = glCreateProgram();
    const unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    const unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    glDeleteShader(vs);
    glDeleteShader(fs);

    programID = program;
};

unsigned int Shader::CompileShader(unsigned int type, const std::string &source) {
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));
    int status;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &status));
    if (status == GL_FALSE) {
        int len;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
        char *msg = static_cast<char *>(alloca(len*sizeof(char)));
        glGetShaderInfoLog(id, len, &len, msg);
        std::cout << "[ERROR]: Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<
                " shader."
                <<
                std::endl;
        std::cout << msg << std::endl;
        glDeleteShader(id);
        return 0;
    }
    //TODO: Error handling for shader, in case we missed something in our shader code (semicolons etc.)
    return id;
}

void Shader::Bind() const {
    GLCall(glUseProgram(programID));
}

void Shader::Unbind() const {
    glDeleteProgram(programID);
}

void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3) {
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMatrix4fv(const std::string &name, const GLfloat *matrix, bool transpose) {
    GLCall(
        glUniformMatrix4fv(GetUniformLocation(name), 1, transpose ? GL_TRUE : GL_FALSE, matrix));
}

unsigned int Shader::GetUniformLocation(const std::string &name) {
    if (uniformLocationCache.find(name) != uniformLocationCache.end()) {
        return uniformLocationCache[name];
    }
    // get uniform location (returns int index for the shaders uniform variable location)
    GLCall(int location = glGetUniformLocation(programID, name.c_str()));
    if (location == -1) {
        std::cout << "[WARNING]: Uniform " << name << " at location " << location << " doesn't exist!" << std::endl;
    }

    uniformLocationCache[name] = location;
    return location;
}

ShaderProgramSource Shader::ParseShader(const std::string &shaderPath) {
    std::cout << "[DEBUG]: Starting to parse shader: " << shaderPath << std::endl;
    std::ifstream fs(shaderPath);
    if (fs.fail()) {
        std::cout << "[ERROR]: Failed to load shader at path: " << shaderPath << std::endl;
        return {"", ""};
    }
    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    std::string line;
    std::stringstream ss[2]{};
    ShaderType type = ShaderType::NONE;
    while (getline(fs, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        } else if (type != ShaderType::NONE) {
            ss[static_cast<int>(type)] << line << "\n";
        }
    }
    fs.close();
    return {ss[0].str(), ss[1].str()};
}

Shader::Shader(const std::string &shaderPath) {
    ShaderProgramSource source = ParseShader(shaderPath);
    if (source.VertexSource.empty() || source.FragmentSource.empty()) {
        std::cerr << "[ERROR]: Shader source is empty.\n";
        exit(EXIT_FAILURE); // removing this return will start the app with a black window, even if shader fails
    }
    CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader() {
    std::cout << "[DEBUG]: Shader destructor initialized - shutting down shader program.\n";
}

