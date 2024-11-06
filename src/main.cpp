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
#include "renderer/vertexArray.h"

mat4x4 transform;

constexpr int WINDOW_WIDTH = 1024;
constexpr int WINDOW_HEIGHT = 768;


float position[] = {
    -0.3f, -0.3f, // Bottom-left
    0.3f, -0.3f, // Bottom-right
    0.3f, 0.3f, // Top-right
    -0.3f, 0.3f, // Top-left
};

unsigned int indices[] = {
    0, 1, 2, // first triangle
    2, 3, 0, // second triangle
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
    glfwSetKeyCallback(window, key_callback); // handle kb inputs

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    VertexArray va;
    VertexBuffer vb(position, 4 * 2 * sizeof(position));
    VertexBufferLayout vbl;
    Shader shader("../src/res/shader.glsl");

    shader.Bind();
    vbl.Push<float>(2);
    va.AddBuffer(vb, vbl);

    IndexBuffer ib(indices, 6 * sizeof(indices));

    va.Unbind();
    shader.Unbind();
    vb.Unbind();
    ib.Unbind();

    unsigned int program_instance = shader.programID;


    if (program_instance == 0) {
        std::cerr << "[ERROR]: Shader program could not be created." << std::endl;
        return -1;
    }


    mat4x4_identity(transform);

    float r = 0.4f;
    float g = 0.2f;
    float r_inc = 0.05;

    float lastFrameTime = 0.0f;

    Renderer renderer;

    while (!glfwWindowShouldClose(window)) {
        float speed = 1.5f;
        renderer.Clear();

        // calculate deltaTime for consistent movement across frames
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


        shader.SetUniformMatrix4fv("transform", *transform, false);
        shader.SetUniform4f("u_color", r, g, 0.3f, 1.0f);

        renderer.Draw(va, ib, shader);

        if (r > 1.0f) {
            r_inc = -0.05;
        } else if (r < 0.0f) {
            r_inc = 0.05;
        }
        r += r_inc;
        g += r_inc;

        glfwSwapBuffers(window);
        glfwSwapInterval(1); // enable vsync
        glfwPollEvents(); // poll for window events such as kb, mouse etc.
    }

    shader.Unbind();
    va.Unbind();
    ib.Unbind();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
