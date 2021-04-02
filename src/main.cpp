#include <iostream>
#include <math.h>
#define GLEW_STATIC
#include <GL/glew.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "defines.h"
#include "shader.h"
#include "vertex_buffer.h"

// Config values
unsigned int screenWidth = 1600;
unsigned int screenHeight = 900;
float animationSpeed = 0.0005f;
float baseOffsetValues[] = {20.0f, 10.0f, 50.0f};  // It's kind of a seed

int main() {

    // Inti SDL
    SDL_Window * window;
    SDL_Init(SDL_INIT_EVERYTHING);

    // Init vertex buffer
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // Create window
    window = SDL_CreateWindow("Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    // Init GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cout << "Error: " << std::endl;
        std::cin.get();
        return -1;
    }

    // Define some vertecies so that they fill the entire screen drawn as triangle strip
    Vertex vertecies[] = {
        Vertex{-1.0f, -1.0f, 0.0f},
        Vertex{-1.0f, 1.0f, 0.0f},
        Vertex{1.0f, -1.0f, 0.0f},
        Vertex{1.0f, 1.0f, 0.0f}
    };
    uint32 numVertecies = 4;

    // Define vertex buffer
    VertexBuffer vertexBuffer(vertecies, numVertecies);
    vertexBuffer.unbind();

    // Define Shaders
    Shader shader("shader/basic.vs", "shader/basic.fs");
    shader.bind();

    // Init the uniforms
    GLint screenDimUniformLocation = glGetUniformLocation(shader.getShaderId(), "screenDim");
    GLint offsetUniformLocation = glGetUniformLocation(shader.getShaderId(), "offset");
    GLint baseOffsetUniformLoactionRed = glGetUniformLocation(shader.getShaderId(), "baseOffsetRed");
    GLint baseOffsetUniformLoactionGreen = glGetUniformLocation(shader.getShaderId(), "baseOffsetGreen");
    GLint baseOffsetUniformLoactionBlue = glGetUniformLocation(shader.getShaderId(), "baseOffsetBlue");
    if (screenDimUniformLocation == -1) std::cout << "screenDim\n";
    if (offsetUniformLocation == -1) std::cout << "offset\n";
    if (baseOffsetUniformLoactionRed == -1) std::cout << "Red\n";
    if (baseOffsetUniformLoactionGreen == -1) std::cout << "Green\n";
    if (baseOffsetUniformLoactionBlue == -1) std::cout << "Blue\n";
    /*if (screenDimUniformLocation == -1
    || offsetUniformLocation == -1
    || baseOffsetUniformLoactionRed == -1
    || baseOffsetUniformLoactionGreen == -1
    || baseOffsetUniformLoactionBlue == -1) {
        std::cout << "uniform error" << std::endl;
        return 0;
    }*/

    // Set the screenDim uniform once
    glUniform2f(screenDimUniformLocation, (float)screenWidth, (float)screenHeight);

    // Set base uniform offsets 
    glUniform2f(baseOffsetUniformLoactionRed, baseOffsetValues[0], baseOffsetValues[1]);
    glUniform2f(baseOffsetUniformLoactionBlue, baseOffsetValues[0], baseOffsetValues[2]);
    glUniform2f(baseOffsetUniformLoactionGreen, baseOffsetValues[1], baseOffsetValues[2]);

    // Init changing varibles for the animation
    float animationTime = 0.0f;
    float offsetX = 0.0f;
    float offsetY = 0.0f;

    // Main Loop
    bool run = true;
    while(run) {

        // Check quit
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                run = false;
            }
        }

        // Claulate the animation
        animationTime += animationSpeed;
        offsetX = cos(animationTime) * 5;
        offsetY = sin(animationTime) * 5;

        // Set the offset uniform to pass offset to frag shader
        glUniform2f(offsetUniformLocation, offsetX, offsetY);

        // Clear screen
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the image to the screen buffer
        vertexBuffer.bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, numVertecies);
        vertexBuffer.unbind();

        // Swap the 2 buffers from dualbufferiing -> show to screen
        // Also waits for V-Sync
        SDL_GL_SwapWindow(window);

    }

    return 0;
}
