cmake_minimum_required(VERSION 3.29)
project(fireball)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/src/main.cpp
        ${CMAKE_SOURCE_DIR}/src/glad.c
        ${CMAKE_SOURCE_DIR}/src/renderer/shader.cpp
        ${CMAKE_SOURCE_DIR}/src/renderer/shader.h
        src/renderer/renderer.cpp
        src/renderer/vertexBuffer.h
        src/renderer/vertexBuffer.cpp
        src/renderer/indexBuffer.h
        src/renderer/indexBuffer.cpp
        src/renderer/vertexArray.h
        src/renderer/vertexArray.cpp
        src/renderer/vertexBufferLayout.h
        src/renderer/vertexBufferLayout.cpp
        src/components/texture.h
        src/components/texture.cpp
        dependencies/include/stb_image.cpp
        src/utils/coordinates.h
        src/utils/collision.h
        src/utils/keymaps.h
)


add_executable(fireball ${SOURCE_FILES})


# Include directories for dependencies
target_include_directories(fireball PRIVATE
        ${CMAKE_SOURCE_DIR}/dependencies/include
)

# Link directories for libraries
target_link_directories(fireball PRIVATE ${CMAKE_SOURCE_DIR}/dependencies/lib)

find_package(OpenGL REQUIRED)

target_link_libraries(fireball
        ${CMAKE_SOURCE_DIR}/dependencies/lib/libglfw3.a
        OpenGL::GL
        "-framework OpenGL"    # Use OpenGL framework
        "-framework Cocoa"      # Required for macOS GUI
        "-framework IOKit"
        "-framework CoreVideo"
)
