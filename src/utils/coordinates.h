#ifndef COORDINATES_H
#define COORDINATES_H

#pragma once

inline float fullscreenVertices[] = {
  -1.0f, -1.0f, 0.0f, 0.0f,
  1.0f, -1.0f, 1.0f, 0.0f,
  1.0f, 1.0f, 1.0f, 1.0f,
  -1.0f, 1.0f, 0.0f, 1.0f
};

inline unsigned int fullscreenIndices[] = {
  0, 1, 2,
  2, 3, 0
};


inline float backgroundVertices[] = {
  -250.0f, -250.0f, 0.0f, 0.0f, // 0
  250.0f, -250.0f, 1.0f, 0.0f, // 1
  250.0f, 250.0f, 1.0f, 1.0f, // 2
  -250.0f, 250.0f, 0.0f, 1.0f // 3
};

inline unsigned int backgroundIndices[] = {
  0, 1, 2,
  2, 3, 0
};

inline float playerVertices[] = {
  -50.0f, -50.0f, 0.0f, 0.0f, // 0
  50.0f, -50.0f, 1.0f, 0.0f, // 1
  50.0f, 50.0f, 1.0f, 1.0f, // 2
  -50.0f, 50.0f, 0.0f, 1.0f // 3
};

inline unsigned int playerIndices[] = {
  0, 1, 2,
  2, 3, 0
};

inline float ball[] = {
  -50.0f, -50.0f, 0.0f, 0.0f, // 0
  50.0f, -50.0f, 1.5f, 0.0f, // 1
  50.0f, 50.0f, 1.5f, 1.5f, // 2
  -50.0f, 50.0f, 0.0f, 1.5f // 3
};

inline unsigned int ballIndice[] = {
  0, 1, 2,
  2, 3, 0
};

#endif //COORDINATES_H
