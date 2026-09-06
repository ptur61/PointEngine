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

    if (peCreateInstance("PointEngine Example 1", WIDTH, HEIGHT) == false) {
        SDL_Log("Unable to initalise program!\n");
        exit_code = 1;
    } else {
        bool quit = false;
        SDL_Event e;
        SDL_zero(e);


        // Vertex Array
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



        /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~TRANSLATION CUBES~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            In order to remove the issue of floating-point error, these 'translation cubes' will always stay the same no matter what;
            they will be used as a template on which translations and transformations will be performed upon, with that result being
            copied to the cube1, cube2 and cube3 variables seen below. In order to perform rotation, these cubes are always rotated from
            the same point, with the angle they are rotated by being changed as to give the illusion of rotation.

            For future improvement, I'm wondering whether these translation cubes can be created automatically by the translation and 
            transformation functions, rather than have the user manually create them. This is something I will look in to, but I'm not yet
            sure on how much hand-holding I want to give the user: will PE be a user-friendly API that has a clear, easy-to-use and structured 
            way how to use it, or will it simply be a collection of functions for the user to use and figure out themselves?
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        */ 

        // Translation vectors
        p3 trans_vec_2 = {-1.0, -0.1, 0.5};
        p3 trans_vec_3 = {-0.2, -1.6, 1.0};

        // Create CUBE_T_1 with vertices defined by the CUBE1 array.
        const shape CUBE_T_1   = peShapeCreate(VA, CUBE1, 36);
        const p3 COG_1     = peShapeGetCOG(CUBE_T_1);

        // Create new cubes by translating first CUBE_T_1 with trans_vec_2 and trans_vec_3 respectively
        const shape CUBE_T_2 = peShapeTranslate(CUBE_T_1, trans_vec_2);
        const p3 COG_2     = peShapeGetCOG(CUBE_T_2);

        const shape CUBE_T_3 = peShapeTranslate(CUBE_T_1, trans_vec_3);
        const p3 COG_3     = peShapeGetCOG(CUBE_T_3);

        
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

        
            cube1  = peShapeRotate(CUBE_T_1, COG_1, 0, angle * 1/60 * 0.01, 0);
            cube2  = peShapeRotate(CUBE_T_2, COG_2,  angle * 1/60 * 0.01, 0, 0);
            cube3  = peShapeRotate(CUBE_T_3, COG_3,  0, 0, angle * 1/60 * 0.01);
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