#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include "PointEngine.h"

const int WIDTH = 640;
const int HEIGHT = 640;

int main(int argc, char* args[]) {
    int exit_code = 0;

    if (peCreateInstance("PointEngine Example 2", WIDTH, HEIGHT) == false) {
        SDL_Log("Unable to initalise program!\n");
        exit_code = 1;
    } else {
        bool quit = false;
        SDL_Event e;
        SDL_zero(e);


        // Vertex Array
        p3 VA_EYE[] = {
                    {0.8, 0.8, 1.0},  // Top left
                    {0.8, 0.6, 1.0},  // Bottom left
                    {0.6, 0.8, 1.0},  // Top right
                    {0.6, 0.6, 1.0},  // Bottom right

                    {0.8, 0.8, 1.2},  // Top left
                    {0.8, 0.6, 1.2},  // Bottom left
                    {0.6, 0.8, 1.2},  // Top right
                    {0.6, 0.6, 1.2},  // Bottom right
        };

        int EYE1[] = { 
                    // Front face
                    0, 2, 1,
                    1, 2, 3,
                    
                    // Back face
                    5, 6, 4,
                    7, 6, 5,

                    // Left side
                    4, 0, 5,
                    5, 0, 1 ,

                    // Right side
                    2, 6, 3,
                    3, 6, 7,

                    // Top
                    1, 3, 5,
                    5, 3, 7,

                    // Bottom
                    6, 2, 4,
                    4, 2, 0
        };

        p3 VA_NOSE[] = {
                    {-0.2, 0.2, 1.0}, // Bottom left
                    { 0.2, 0.2, 1.0},
                    { 0.2, 0.6, 1.0},

                    {-0.2, 0.2, 1.2}, // Bottom left
                    { 0.2, 0.2, 1.2},
                    { 0.2, 0.6, 1.2},
        };

        int NOSE[] = {
                    // Front face
                    0, 1, 2,

                    // Back face
                    5, 4, 3,

                    // Left side
                    5, 3, 0,
                    0, 2, 5,

                    // Right side
                    5, 2, 1,
                    1, 4, 5, // I could be wrong

                    // Bottom side
                    0, 3, 4,
                    4, 1, 0 // I could be wrong



        };

        p3 VA_MOUTH[] = {
                    {-0.7, -0.5, 1.0},
                    {-0.8, -0.6, 1.0},
                    { 0.7, -0.5, 1.0},
                    { 0.8, -0.6, 1.0},

                    {-0.7, -0.5, 1.2},
                    {-0.8, -0.6, 1.2},
                    { 0.7, -0.5, 1.2},
                    { 0.8, -0.6, 1.2},


        };

        int MOUTH[] = {
                    2, 0, 1,
                    1, 3, 2,

                    6, 4, 5,
                    5, 7, 6
        };

        

        
        shape eye1  = peShapeCreate(VA_EYE, EYE1, 36);
        shape eye2  = peShapeTranslate(eye1, (p3){-1.4, 0.0, 0.0});
        shape nose  = peShapeCreate(VA_NOSE, NOSE, 24);
        shape mouth = peShapeCreate(VA_MOUTH, MOUTH, 6);
        p3 nose_cog = peShapeGetCOG(nose);
        nose = peShapeRotate(nose, nose_cog, 0.0, -1.5708 * 0.75, 0.0);

        shape eye1_t;
        shape eye2_t;
        shape nose_t;
        shape mouth_t;

        uint16_t old = SDL_GetTicks();

        double angle = 0;

        while (quit == false) {
            while (SDL_PollEvent(&e) == true) {
                if (e.type == SDL_EVENT_QUIT) {
                    quit = true;
                }
            }

            eye1_t = peShapeTranslate(eye1, (p3){0.0, 0.0, sin(angle) * 1/60 * 20});
            eye2_t = peShapeTranslate(eye2, (p3){0.0, 0.0, sin(angle) * 1/60 * 20});
            nose_t = peShapeTranslate(nose, (p3){0.0, 0.0, sin(angle) * 1/60 * 20});
            mouth_t = peShapeTranslate(mouth, (p3){0.0, 0.0, sin(angle) * 1/60 * 20});
            angle += 0.001;


            SDL_SetRenderDrawColor(peGetRenderer(), 0, 0, 0, 0xFF);
            SDL_RenderClear(peGetRenderer());

            peSetDrawColour(0, 255, 0);
            peShapeDraw(eye1_t, 1);
            peShapeDraw(eye2_t, 1);
            peShapeDraw(nose_t, 1);
            peShapeDraw(mouth_t, 1);

            peShapeDelete(eye1_t);
            peShapeDelete(eye2_t);
            peShapeDelete(nose_t);
            peShapeDelete(mouth_t);

            SDL_RenderPresent(peGetRenderer());

        }
    }
    return exit_code;
}