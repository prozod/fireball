#ifndef KEYMAPS_H
#define KEYMAPS_H

inline bool keyStates[6] = {false, false, false, false, false, false}; // W, A, S, D, L_SHIFT, SPACE
static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods) {
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
  if (key == GLFW_KEY_LEFT_SHIFT)
    keyStates[4] = action != GLFW_RELEASE;
  if (key == GLFW_KEY_SPACE)
    keyStates[5] = action != GLFW_RELEASE;
}

#endif //KEYMAPS_H
