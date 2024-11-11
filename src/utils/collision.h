#ifndef COLLISION_H
#define COLLISION_H
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct Ball {
    glm::mat4 model;
    float speedX;
};

inline bool checkCollision(const glm::mat4 &playerTransform, float playerWidth, float playerHeight, const Ball &ball,
                           float ballSize) {
    // player
    auto playerPosition = glm::vec3(playerTransform[3]);
    float playerLeft = playerPosition.x - playerWidth / 2.0f;
    float playerRight = playerPosition.x + playerWidth / 2.0f;
    float playerTop = playerPosition.y + playerHeight / 2.0f;
    float playerBottom = playerPosition.y - playerHeight / 2.0f;

    // ball
    auto ballPosition = glm::vec3(ball.model[3]);
    float ballLeft = ballPosition.x - ballSize / 2.0f;
    float ballRight = ballPosition.x + ballSize / 2.0f;
    float ballTop = ballPosition.y + ballSize / 2.0f;
    float ballBottom = ballPosition.y - ballSize / 2.0f;

    // check if ball and player collide
    return (playerRight > ballLeft && playerLeft < ballRight &&
            playerTop > ballBottom && playerBottom < ballTop);
}

#endif //COLLISION_H
