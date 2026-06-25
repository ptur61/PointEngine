#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#include "PointEngine.h"

#define PI 3.141592653589793
#define NV (PI * PI * 12345) // Null Value
#define NP3 {NV, NV, NV}  // Null P3

const int WIDTH = 640;
const int HEIGHT = 640;

camera global_camera = { {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0} };

int main(int argc, char* args[]) {
    int exit_code = 0;

    if (peCreateInstance("PointEngine", WIDTH, HEIGHT) == false) {
        SDL_Log("Unable to initalise program!\n");
        exit_code = 1;
    } else {
        bool quit = false;
        SDL_Event e;
        SDL_zero(e);

        p3 VA[] = { {0.2, 0.2, 1.0},  // Botton left
                    {0.2, 0.7, 1.0},  // Top left
                    {0.7, 0.2, 1.0},  // Botton Right
                    {0.7, 0.7, 1.0},  // Top Right
                    {0.2, 0.2, 1.5},  
                    {0.2, 0.7, 1.5}, 
                    {0.7, 0.2, 1.5},
                    {0.7, 0.7, 1.5} 
                };

        int CUBE1[] = { 
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

        p3 c2tran = {-1.0, -0.1, 0.5};
        p3 c3tran = {-0.2, -1.6, 1.0};

        shape cube   = peShapeCreate(VA, CUBE1, 36);
        p3 c1cog     = peShapeGetCOG(cube);

        shape cube2t = peShapeTranslate(cube, c2tran);
        p3 c2cog     = peShapeGetCOG(cube2t);

        shape cube3t = peShapeTranslate(cube, c3tran);
        p3 c3cog     = peShapeGetCOG(cube3t);

        shape cube1;
        shape cube2;
        shape cube3;

        uint16_t old = SDL_GetTicks();

        double angle = 0;

        while (quit == false) {
            while (SDL_PollEvent(&e) == true) {
                if (e.type == SDL_EVENT_QUIT) {
                    quit = true;
                }
            }

        
            cube1  = peShapeRotate(cube, c1cog, 0, angle * 1/60 * 0.01, 0);

            cube2  = peShapeRotate(cube2t, c2cog,  angle * 1/60 * 0.01, 0, 0);

            cube3  = peShapeRotate(cube3t, c3cog,  0, 0, angle * 1/60 * 0.01);
            angle++;

            SDL_SetRenderDrawColor(peGetRenderer(), 0, 0, 0, 0xFF);
            SDL_RenderClear(peGetRenderer());

            peSetDrawColour(255, 0, 0);
            peShapeDraw(cube1, 1);

            peSetDrawColour(0, 255, 0);
            peShapeDraw(cube2, 1);

            peSetDrawColour(0, 0, 255);
            peShapeDraw(cube3, 1);

            peShapeDelete(cube1);
            peShapeDelete(cube2);
            peShapeDelete(cube3);

            SDL_RenderPresent(peGetRenderer());

        }
    }
    return exit_code;
}