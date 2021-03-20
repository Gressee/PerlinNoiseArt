#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "defines.h"
#include "shader.h"
#include "vertex_buffer.h"

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
    window = SDL_CreateWindow("Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_OPENGL);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    // Init GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cout << "Error: " << std::endl;
        std::cin.get();
        return -1;
    }

    // Define some vertecies
    Vertex vertecies[] = {
        Vertex{-1.0f, -1.0f, 0.0f},
        Vertex{-0.0f, 1.0f, 0.0f},
        Vertex{1.0f, -1.0f, 0.0f}
    };
    uint32 numVertecies = 3;

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, numVertecies * sizeof(Vertex), vertecies, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

    // Define Shaders
    Shader shader("shader/basic.vs", "shader/basic.fs");
    shader.bind();

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

        // Clear screen
        glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the image to the screen buffer
        glDrawArrays(GL_TRIANGLES, 0, numVertecies);

        // Swap the 2 buffers from dualbufferiing -> show to screen
        // Also waits for V-Sync
        SDL_GL_SwapWindow(window);

    }

    return 0;
}
