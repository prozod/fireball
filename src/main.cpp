#include "linmath.h"
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "components/texture.h"
#include "renderer/indexBuffer.h"
#include "renderer/renderer.h"
#include "renderer/shader.h"
#include "renderer/vertexArray.h"
#include "renderer/vertexBuffer.h"
#include "utils/collision.h"
#include "utils/coordinates.h"
#include "utils/keymaps.h"

constexpr int WINDOW_WIDTH = 1100;
constexpr int WINDOW_HEIGHT = 650;
constexpr float GAME_OVER_FLASH_DURATION = 1.0f;

void triggerGameOver(float currentTime, float &gameOverTime, bool &gameIsOver) {
  gameIsOver = true;
  gameOverTime = currentTime;
}

void error_callback(int error, const char *desc) {
  fprintf(stderr, "[Callback Function] Error %d: %s\n", error, desc);
}


int main() {
  int score = 0;
  bool gameIsOver = false;
  float lastFrameTime = 0.0f;
  float gameOverTime = 0.0f;
  srand(time(nullptr));

  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW3\n");
    return -1;
  }
  glfwSetErrorCallback(error_callback);

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
                                        "Fireball | 2D Game", nullptr, nullptr);

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

  GLCall(glEnable(GL_BLEND));
  GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  // --------- CHARACTER VAO VBO TEXTURE ---------
  VertexArray playerVA;
  VertexBuffer playerVB(playerVertices, 4 * 4 * sizeof(playerVertices));
  VertexBufferLayout playerVBL;
  Shader playerShader("../src/res/playershader.glsl");

  playerShader.Bind();
  playerVBL.Push<float>(2);
  playerVBL.Push<float>(2);
  playerVA.AddBuffer(playerVB, playerVBL);

  IndexBuffer playerIB(playerIndices, 6 * sizeof(playerIndices));

  Texture texture("../src/res/char.png");
  playerShader.SetUniform1i("u_texture", 0);
  texture.Bind(0);

  glm::mat4 playerProj = glm::ortho(0.0f, static_cast<float>(WINDOW_WIDTH), 0.0f, static_cast<float>(WINDOW_HEIGHT),
                                    -1.0f,
                                    1.0f);
  glm::mat4 playerView = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));


  // -------- BACKGROUND VAO VBO TEXTURE ------------
  VertexArray bgVA;
  VertexBuffer bgVB(backgroundVertices, 4 * 4 * sizeof(backgroundVertices));
  VertexBufferLayout bgVBL;
  Shader bgShader("../src/res/bgshader.glsl");

  bgShader.Bind();
  bgVBL.Push<float>(2);
  bgVBL.Push<float>(2);
  bgVA.AddBuffer(bgVB, bgVBL);

  IndexBuffer bgIB(backgroundIndices, 6 * sizeof(backgroundIndices));

  Texture bgTexture("../src/res/ground.png");
  bgShader.SetUniform1i("u_bg_texture", 1);
  bgTexture.Bind(1);

  // -------- BALL VAO VBO TEXTURE ------------
  VertexArray ballVA;
  VertexBuffer ballVB(ball, 4 * 4 * sizeof(ball));
  VertexBufferLayout ballVBL;
  Shader ballShader("../src/res/balls.glsl");

  ballShader.Bind();
  ballVBL.Push<float>(2);
  ballVBL.Push<float>(2);
  ballVA.AddBuffer(ballVB, ballVBL);

  IndexBuffer ballIB(ballIndice, 6 * sizeof(ballIndice));

  Texture ballTexture("../src/res/fireball.png");
  ballShader.SetUniform1i("u_texture", 2);
  ballTexture.Bind(2);

  // ------------ FULLSCREEN DEATH RED EFFFECT ------------
  VertexArray fullscreenVA;
  VertexBuffer fullscreenVB(fullscreenVertices, sizeof(fullscreenVertices));
  VertexBufferLayout fullscreenLayout;

  fullscreenLayout.Push<float>(2); // Position (x, y)
  fullscreenLayout.Push<float>(2); // Texture coordinates (optional, if needed)

  fullscreenVA.AddBuffer(fullscreenVB, fullscreenLayout);

  IndexBuffer fullscreenIB(fullscreenIndices, sizeof(fullscreenIndices) / sizeof(unsigned int));

  unsigned int characterShaderProgram = playerShader.programID;
  unsigned int backgroundShaderProgram = bgShader.programID;


  if (characterShaderProgram == 0 || backgroundShaderProgram == 0) {
    std::cerr << "[ERROR]: Shader program could not be created." << std::endl;
    return -1;
  }

  // background MVP
  float scaleFactor = 4.0f;
  glm::mat4 bgProj = glm::ortho(0.0f, static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT), 0.0f, -1.0f,
                                1.0f);
  auto bgView = glm::mat4(1.0f);
  glm::mat4 bgModel = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, -80.0f, 0.0f));
  bgModel = glm::scale(bgModel, glm::vec3(scaleFactor, scaleFactor, 1.0f));
  glm::mat4 bgMvp = bgProj * bgView * bgModel;

  // BALL PHYSICS
  std::vector<Ball> balls;
  float spawnInterval = 1.0f;
  float timeSinceLastSpawn = 0.0f;
  int ballsToSpawn = 2;
  float ballSize = 50.0f;
  float individualBallSpeed;

  // PLAYER PHYSICS OR IDK
  glm::mat4 playerTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(WINDOW_WIDTH / 4, WINDOW_HEIGHT / 2, 0));
  constexpr float playerWidth = 50.0f;
  constexpr float playerHeight = 50.0f;
  short int playerXballCollisionNumber = 0;
  constexpr short int playerLifetime = 2;
  bool playerFacingRight = true;

  // COLLISION STUFF
  constexpr float collisionCooldown = 2.0f;
  float lastCollisionTime = -collisionCooldown; // this makes it so the player can be hit immediately
  float flashTimeSinceLastHit = 0.0f; //
  constexpr float flashDuration = 0.1f;
  constexpr float flashOpacity = 0.5f;

  Renderer renderer;
  while (!glfwWindowShouldClose(window)) {
    float speed = 250.0f;
    renderer.Clear();

    // deltaTime for consistent movement across frames
    float currentFrameTime = glfwGetTime();
    float deltaTime = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;

    // Increment spawn timer (for spawning new balls)
    timeSinceLastSpawn += deltaTime;

    // If game is over, wait for flash duration then exit
    if (gameIsOver && (currentFrameTime - gameOverTime >= GAME_OVER_FLASH_DURATION)) {
      glfwSetWindowShouldClose(window, true);
    }

    if (!gameIsOver) {
      /// -------- BACKGROUND -----------
      bgShader.Bind();
      bgShader.SetUniformMatrix4fv("u_mvp", bgMvp);
      bgTexture.Bind(1);
      renderer.Draw(bgVA, bgIB, bgShader);

      if (keyStates[4]) // L-SHIFT - decrease speed
        speed -= 200.0f;
      if (keyStates[5]) // SPACE - increase speed
        speed += 200.0f;
      if (keyStates[0]) {
        if (playerTranslation[3][1] + playerHeight / 2 < WINDOW_HEIGHT)
          playerTranslation = glm::translate(playerTranslation, glm::vec3(0, speed * deltaTime, 0));
      }
      if (keyStates[1]) {
        if (playerTranslation[3][0] - playerWidth / 2 > 0) {
          playerTranslation = glm::translate(playerTranslation, glm::vec3(-speed * deltaTime, 0, 0));
          playerFacingRight = false;
        }
      }
      if (keyStates[2]) {
        if (playerTranslation[3][1] - playerHeight / 2 > 0)
          playerTranslation = glm::translate(playerTranslation, glm::vec3(0, -speed * deltaTime, 0));
      }
      if (keyStates[3]) {
        if (playerTranslation[3][0] + playerWidth / 2 < WINDOW_WIDTH) {
          playerTranslation = glm::translate(playerTranslation, glm::vec3(speed * deltaTime, 0, 0));
          playerFacingRight = true;
        }
      }


      // ------------ DRAW CHARACTER ---------
      glm::mat4 playerModel = playerTranslation;
      if (!playerFacingRight) {
        playerModel = glm::scale(playerModel, glm::vec3(-1.0f, 1.0f, 1.0f));
      }
      playerShader.Bind();
      glm::mat4 playerMVP = playerProj * playerView * playerModel;
      playerShader.SetUniformMatrix4fv("u_mvp", playerMVP);
      texture.Bind(0);
      renderer.Draw(playerVA, playerIB, playerShader);

      // ------------ SPAWN NEW BALLS LOOP ---------
      if (timeSinceLastSpawn >= spawnInterval) {
        for (int i = 0; i < ballsToSpawn; ++i) {
          Ball newBall{};
          auto randomY = static_cast<float>(rand() % WINDOW_HEIGHT);

          individualBallSpeed = 250.0f + static_cast<float>(rand() % 100);
          newBall.speedX = individualBallSpeed;
          newBall.model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, randomY, 0));
          balls.push_back(newBall);
        }
        timeSinceLastSpawn = 0.0f;
      }

      // -------- MOVE AND DRAW BALLS --------
      ballShader.Bind();
      for (auto iterator = balls.begin(); iterator != balls.end();) {
        iterator->model = glm::translate(iterator->model, glm::vec3(iterator->speedX * deltaTime, 0, 0));

        // boundcheck for balls
        glm::vec4 ballPosition = iterator->model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); // position of the ball
        float ballX = ballPosition.x; // X-coord of the ball
        float ballY = ballPosition.y; // Y-coord of the ball

        // check if ball is out of the bounds and remove if outside
        if (ballX < 0 || ballX > WINDOW_WIDTH || ballY < 0 || ballY > WINDOW_HEIGHT) {
          iterator = balls.erase(iterator);
          score += 1;
          std::cout << "Score: " << score << " points" << std::endl;
          continue;
        } else {
          glm::mat4 mvpBall = playerProj * playerView * iterator->model;
          ballShader.SetUniformMatrix4fv("u_mvp", mvpBall);
          ballTexture.Bind(2);
          renderer.Draw(ballVA, ballIB, ballShader);
          ++iterator;
        }
      }

      for (auto it = balls.begin(); it != balls.end();) {
        if (currentFrameTime - lastCollisionTime >= collisionCooldown) {
          if (checkCollision(playerTranslation, playerWidth, playerHeight, *it, ballSize)) {
            if (playerXballCollisionNumber < playerLifetime) {
              std::cout << "[[[ PROJECTILE HIT ]]]" << std::endl;
              std::cout << "You have " << playerLifetime - playerXballCollisionNumber << " lives left." << std::endl;
              playerXballCollisionNumber += 1;
              ballsToSpawn += 1;
              individualBallSpeed += 150.0f;
              flashTimeSinceLastHit = flashDuration;

              it = balls.erase(it); // removes ball that hit the player
              lastCollisionTime = currentFrameTime;

              // breaking out of the loop to prevent further collision checks in this frame
              break;
            } else {
              std::cout << "> > > [GAME OVER] < < <" << std::endl;
              triggerGameOver(currentFrameTime, gameOverTime, gameIsOver);
              std::cout << "[FINAL SCORE]: " << (score) << " points" << std::endl;
              break;
            }
          } else {
            ++it;
          }
        } else {
          ++it;
        }
      }
    }

    // renders red screen when projectile collides with character
    if (flashTimeSinceLastHit > 0.0f) {
      flashTimeSinceLastHit -= deltaTime;
      float opacity = (flashTimeSinceLastHit / flashDuration) * flashOpacity;
      Shader redOverlayShader("../src/res/death.glsl");
      redOverlayShader.Bind();
      redOverlayShader.SetUniform4f("u_color", 1.0f, 0.0f, 0.0f, opacity);
      glm::mat4 overlayModel = glm::scale(glm::mat4(1.0f), glm::vec3(WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f));
      glm::mat4 overlayMVP = playerProj * playerView * overlayModel;
      redOverlayShader.SetUniformMatrix4fv("u_mvp", overlayMVP);
      renderer.Draw(fullscreenVA, fullscreenIB, redOverlayShader);
    }

    if (gameIsOver) {
      float flashTimeLeft = GAME_OVER_FLASH_DURATION - (currentFrameTime - gameOverTime);
      if (flashTimeLeft > 0.0f) {
        auto opacity = static_cast<float>((flashTimeLeft / GAME_OVER_FLASH_DURATION) * 0.8);

        Shader redOverlayShader("../src/res/death.glsl");
        redOverlayShader.Bind();
        redOverlayShader.SetUniform4f("u_color", 1.0f, 0.0f, 0.0f, opacity);

        glm::mat4 overlayModel = glm::scale(glm::mat4(1.0f), glm::vec3(WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f));
        glm::mat4 overlayMVP = playerProj * playerView * overlayModel;
        redOverlayShader.SetUniformMatrix4fv("u_mvp", overlayMVP);

        renderer.Draw(fullscreenVA, fullscreenIB, redOverlayShader);
      }
    }
    glfwSwapBuffers(window);
    glfwSwapInterval(1); // enable vsync
    glfwPollEvents(); // poll for window events such as kb, mouse etc.
  }

  playerShader.Unbind();
  playerVA.Unbind();
  playerIB.Unbind();
  bgShader.Unbind();
  bgVA.Unbind();
  bgIB.Unbind();
  ballShader.Unbind();
  ballVA.Unbind();
  ballIB.Unbind();
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
