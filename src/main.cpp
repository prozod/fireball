#include <iostream>
#include "linmath.h"
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <sstream>
#include "renderer/shader.h"
#include "renderer/renderer.h"
#include "renderer/vertexBuffer.h"
#include "renderer/indexBuffer.h"

mat4x4 transform;

constexpr int WINDOW_WIDTH = 1024;
constexpr int WINDOW_HEIGHT = 768;


float position[] = {
    // First rectangle
    -0.3f, -0.3f, // Bottom-left
    0.3f, -0.3f, // Bottom-right
    0.3f, 0.3f, // Top-right
    -0.3f, 0.3f, // Top-left
};

unsigned int indices[] = {
    0, 1, 2, // First rectangle, first triangle
    2, 3, 0, // First rectangle, second triangle
};


void error_callback(int error, const char *desc) {
    fprintf(stderr, "[Callback Function] Error %d: %s\n", error, desc);
}

bool keyStates[4] = {false, false, false, false}; // W, A, S, D
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_W)
        keyStates[0] = action != GLFW_RELEASE;
    if (key == GLFW_KEY_A)
        keyStates[1] = action != GLFW_RELEASE;
    if (key == GLFW_KEY_S)
        keyStates[2] = action != GLFW_RELEASE;
    if (key == GLFW_KEY_D)
        keyStates[3] = action != GLFW_RELEASE;
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
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
    GLCall(glGenVertexArrays(1, &VAO));
    GLCall(glBindVertexArray(VAO));

    VertexBuffer vb(position, sizeof(position));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
        2 * sizeof(float), nullptr));
    GLCall(glEnableVertexAttribArray(0));

    IndexBuffer ib(indices, 6 * sizeof(indices));

    GLCall(glBindVertexArray(0));

    unsigned int program_instance = Shader::LoadShader("../src/res/shader.glsl");
    if (program_instance == 0) {
        std::cerr << "[ERROR]: Shader program could not be created." << std::endl;
        return -1;
    }
    GLCall(glUseProgram(program_instance));

    GLCall(int transformLoc = glGetUniformLocation(program_instance, "transform"));
    ASSERT(transformLoc != -1);
    mat4x4_identity(transform);

    GLCall(int colorLoc = glGetUniformLocation(program_instance, "u_color"));
    ASSERT(colorLoc != -1)

    float r = 0.4f;
    float g = 0.2f;
    float r_inc = 0.05;

    float lastFrameTime = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        float speed = 1.5f;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrameTime = glfwGetTime();
        float deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        if (keyStates[0])
            mat4x4_translate_in_place(transform, 0.0f, speed * deltaTime, 0.0f);
        if (keyStates[1])
            mat4x4_translate_in_place(transform, -speed * deltaTime, 0.0f, 0.0f);
        if (keyStates[2])
            mat4x4_translate_in_place(transform, 0.0f, -speed * deltaTime, 0.0f);
        if (keyStates[3])
            mat4x4_translate_in_place(transform, speed * deltaTime, 0.0f, 0.0f);


        GLCall(glUniformMatrix4fv(transformLoc, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(transform)));
        GLCall(glUniform4f(colorLoc, r, 0.2, g, 1.0f));

        GLCall(glBindVertexArray(VAO));
        ib.Bind();

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, reinterpret_cast<void *>(0 * sizeof(unsigned int))));
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, reinterpret_cast<void *>(6 * sizeof(unsigned int))));

        if (r > 1.0f) {
            r_inc = -0.05;
        } else if (r < 0.0f) {
            r_inc = 0.05;
        }
        r += r_inc;
        g += r_inc;

        glBindVertexArray(0);
        glfwSwapBuffers(window);
        glfwSwapInterval(1);
        glfwPollEvents();
    }

    glDeleteProgram(program_instance);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
