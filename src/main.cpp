#define GLFW_INCLUDE_NONE
#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <iostream>
#include "linmath.h"
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <sstream>
#include "renderer/shader.h"


constexpr int WINDOW_WIDTH = 1024;
constexpr int WINDOW_HEIGHT = 768;

float position[] = {
    -0.5f, -0.5f, // 0
    0.5f, -0.5f, // 1
    0.5f, 0.5f, // 2
    -0.5f, 0.5f, // 3
    -0.5f, 1.0f, // 4
};

unsigned int indices[] = {
    0, 1, 2,
    2, 3, 4
};

void error_callback(int error, const char *desc) {
    fprintf(stderr, "[Callback Function] Error %d: %s\n", error, desc);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main() {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW3\n");
        return -1;
    }
    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Fireball | 2D Game", nullptr, nullptr);

    if (!window) {
        fprintf(stderr, "OpenGL or Window context creation failed.");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    unsigned int program_instance = Shader::LoadShader("../src/res/shader.glsl");
    if (program_instance == 0) {
        std::cerr << "[ERROR]: Shader program could not be created." << std::endl;
        return -1;
    }
    glUseProgram(program_instance);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwSwapInterval(1);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(program_instance);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
