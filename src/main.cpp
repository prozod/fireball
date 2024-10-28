#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <algorithm>
#define GL_SILENCE_DEPRECATION
#include <glm/mat2x2.hpp>
#include <glm/vec2.hpp>

#include "common/shader.hpp"
#include <iostream>

GLFWwindow *window;

int main() {
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    return -1;
  }
  glfwWindowHint(GLFW_SAMPLES, 4);               // 4x antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
                 GLFW_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE,
                 GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

  // Open a window and create its OpenGL context
  window = glfwCreateWindow(800, 800, "2D transformations", NULL, NULL);
  if (window == NULL) {
    fprintf(stderr,
            "Failed to open GLFW window. If you have an Intel GPU, they are "
            "not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window); // Initialize GLAD

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  // Create and compile our GLSL program from the shaders
  GLuint programID =
      LoadShaders("../src/shaders/SimpleVertexShader.glsl", "../src/shaders/SimpleFragmentShader.glsl");

  GLuint vertexbuffer;
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

  float posx = 0.0f, posy = 0.0f, scaleIn = 0.3f, scaleOut = 0.3f, theta = 0.0f, rotSpeed = 0.15f;

  glm::vec2 originalVertice1 = glm::vec2(-0.5f, -0.5f);
  glm::vec2 originalVertice2 = glm::vec2(0.5f, -0.5f);
  glm::vec2 originalVertice3 = glm::vec2(0.0f, 0.5f);

  do {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Update position based on key input
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
      posx += 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
      posx -= 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
      posy += 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
      posy -= 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
      scaleOut += 0.01f;
      scaleOut = std::min(scaleOut + 0.01f, 2.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
      scaleOut -= 0.01f;
      scaleOut = std::max(scaleOut, 0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
      scaleIn -= 0.01f;
      scaleIn = std::max(scaleIn, 0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
      scaleIn += 0.01f;
      scaleIn = std::min(scaleIn + 0.01f, 2.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
      theta -= rotSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
      theta += rotSpeed;
    }

    // Update translation vector
    glm::vec2 myT = glm::vec2(posx, posy);
    glm::mat2 scaleMatrix = glm::mat2(scaleIn, 0.0f, 0.0f, scaleOut);
    glm::mat2 rotation =
        glm::mat2(cos(theta), -sin(theta), sin(theta), cos(theta));

    // Apply transformations (scaling + rotation + translation) to the original
    // vertices
    glm::vec2 transformedVertice1 =
        scaleMatrix * rotation * originalVertice1 + myT;
    glm::vec2 transformedVertice2 =
        scaleMatrix * rotation * originalVertice2 + myT;
    glm::vec2 transformedVertice3 =
        scaleMatrix * rotation * originalVertice3 + myT;

    // Recompute vertex buffer data with new transformed vertices
    const GLfloat g_vertex_buffer_data[] = {
        transformedVertice1[0], transformedVertice1[1], 0.0f,
        transformedVertice2[0], transformedVertice2[1], 0.0f,
        transformedVertice3[0], transformedVertice3[1], 0.0f,
    };

    // Update buffer data with the new transformed vertices
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
                 g_vertex_buffer_data, GL_STATIC_DRAW);

    // Use our shader program
    glUseProgram(programID);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    // Draw the triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();

  } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);
}


