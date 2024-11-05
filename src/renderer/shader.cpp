#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>


unsigned int Shader::CreateShader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource) {
    const unsigned int program = glCreateProgram();
    const unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    const unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
};

unsigned int Shader::CompileShader(unsigned int type, const std::string &source) {
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    int status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
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

unsigned int Shader::LoadShader(const std::string &shaderPath) {
    ShaderProgramSource source = ParseShader(shaderPath);
    if (source.VertexSource.empty() || source.FragmentSource.empty()) {
        std::cerr << "[ERROR]: Shader source is empty.\n";
        return 0; // removing this return will start the app with a black window, even if shader fails
    }
    return CreateShader(source.VertexSource, source.FragmentSource);
}


Shader::~Shader() {
    std::cout << "[DEBUG]: Shader destructor initialized - shutting down shader program.\n";
}

