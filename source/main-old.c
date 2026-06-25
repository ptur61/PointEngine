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

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

camera global_camera = { {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0} };

int p = peSetRenderer();

//  --------------------------------------------------------------------------
//  TRANSLATION & ROTATION
//  --------------------------------------------------------------------------

    p3 pePointTranslate(p3 p, p3 v) {
        p3 point = { p.x + v.x, p.y + v.y, p.z + v.z };
        return point;
    }

    p3 pePointRotateXY(p3 p, double r) {
        p3 point;
        point.x = p.x * cos(PI/180 * r) - p.y * sin(PI/180 * r);
        point.z = p.z;
        point.y = p.x * sin(PI/180 * r) + p.y * cos(PI/180 * r);
        return point;
    }

    p3 pePointRotateXZ(p3 p, double r) {
        p3 point;
        point.x = p.x * cos(PI/180 * r) + p.z * sin(PI/180 * r);
        point.z = p.x * -sin(PI/180 * r) + p.z * cos(PI/180 * r);
        point.y = p.y;
        return point;
    }

    p3 pePointRotate(p3 src, double x, double y, double z) {
        p3 pnt;
        pnt.x = ( src.x * cos(z) * cos(y) ) + ( src.y * ( cos(z) * sin(y) * sin(x) - sin(z) * cos(x) ) ) + ( src.z * ( cos(z) * sin(y) * cos(x) + sin(z) * sin(x) ) );
        pnt.y = ( src.x * sin(z) * cos(y) ) + ( src.y * ( sin(z) * sin(y) * sin(x) + cos(z) * cos(x) ) ) + ( src.z * ( sin(z) * sin(y) * cos(x) - cos(z) * sin(x) ) );
        pnt.z = ( src.x * -sin(y) ) + ( src.y * cos(y) * sin(x) ) + ( src.z * cos(y) * cos(x) );
        return pnt;
    }



// ===============================================
// SHAPE METHODS
// ===============================================

shape peShapeCreate(p3 *VA, int *VM, int size) {
    p3 *new_VA = malloc(size * sizeof(p3));
    for (int i = 0; i < size; i++) {
        int index = VM[i];
        new_VA[i] = VA[index];
    }
    shape shp = { new_VA, size };
    return shp;
}

shape peShapeCopy(shape src, int size) {
    p3 *new_VA = malloc(size * sizeof(p3));;
    memcpy(new_VA, src.VA, size * sizeof(p3));
    shape shp = { new_VA, size };
    return shp;
}

void peShapeDelete(shape tgt) {
    free(tgt.VA);
}

p3 peShapeGetCOG(shape s) {
    // cog: centre of gravity
    double x, y, z;
    for (int i = 0; i < s.size; i++) {
            p3 point = s.VA[i];
            x += point.x;
            y += point.y;
            z += point.z;
    }
    x /= s.size;
    y /= s.size;
    z /= s.size;
    p3 cog = {x, y, z};
    return cog;
}

void peShapeDraw(shape src, uint8_t backface_culling) {
    if (!backface_culling) {
    // Consider an optimisation technique that does not draw the same lines multiple times (keep track of drawn vertices? edit VA before rendering?)
        for (int i = 0; i < src.size - 2; i+=3) {
            for (int j = 0; j < 3; j++) {
                p3 point = src.VA[j + i];
                pePointDraw(point);
                peLineDraw(point, src.VA[(j + i + 1) % 3 + i]);
            }
        }
    } else {
        for (int i = 0; i < src.size - 2; i+=3) {
            p3 plane[3] = { src.VA[i], src.VA[i + 1], src.VA[i + 2] };
            if (!is_plane_backface(plane)) {
                for (int j = 0; j < 3; j++) {
                    p3 point = src.VA[i + j];
                    pePointDraw(point);
                    peLineDraw(point, src.VA[(i + j + 1) % 3 + i]);
                }
            }
    }

    }
}

//  --------------------------------------------------------------------------
//  TRANSLATION & ROTATION
//  --------------------------------------------------------------------------

    shape peShapeTranslate(shape tgt, p3 vector) {
        shape shp = peShapeCopy(tgt, tgt.size);
        for (int i = 0; i < shp.size; i++) {
            p3 point  = shp.VA[i];
            shp.VA[i] = pePointTranslate(point, vector);
        }
        return shp;
    }

    shape peShapeRotateXZ(shape tgt, p3 p, double r) {
        shape shp = peShapeCopy(tgt, tgt.size);
        p3 npivot = { -p.x, -p.y, -p.z };
        shp = peShapeTranslate(shp, npivot);
        for (int i = 0; i < shp.size; i++) {
            p3 point  = shp.VA[i];
            shp.VA[i] = pePointRotateXZ(point, r);
        }
        shp = peShapeTranslate(shp, p);
        return shp;
    }

    shape peShapeRotate(shape tgt, p3 pivot, double x, double y, double z) {
        shape shp = peShapeCopy(tgt, tgt.size);
        p3 npivot = { -pivot.x, -pivot.y, -pivot.z };
        shp = peShapeTranslate(shp, npivot);
        for (int i = 0; i < shp.size; i++) {
            p3 point = shp.VA[i];
            shp.VA[i] = pePointRotate(point, x, y, z);
        }
        shp = peShapeTranslate(shp, pivot);
        return shp;

    }



// =======================================================================================================
// SDL
// =======================================================================================================

bool init() {
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) == false) {
        SDL_Log("SDL could not initialise! SDL error: %s\n", SDL_GetError());
        success = false;
    } else {
        if (SDL_CreateWindowAndRenderer("PointEngine", WIDTH, HEIGHT, 0, &window, &renderer) == false ) {
            SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
            success = false;
        } else if (TTF_Init() == false) {
            SDL_Log("SDL_ttf could not initialise! SDL_ttf error: %s\n", SDL_GetError());
            success = false;
        }
    }
    return success;
}

int main(int argc, char* args[]) {
    int exit_code = 0;

    if (init() == false) {
        SDL_Log("Unable to initalise program!\n");
        exit_code = 1;
    } else {
        bool quit = false;
        SDL_Event e;
        SDL_zero(e);

        double t = 0.0;
        double dt = 1.0/60.0;
        double angle = 1/60;

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

        int TRI1[] = {
                    0, 1, 2,
                    3, 4, 5,

                    0, 3, 5,
                    2, 5, 0,

                    0, 3, 4,
                    0, 1, 4,

                    1, 2, 5,
                    1, 4, 5
        };

        int FACE1[] = {
                    0, 1, 2,
                    1, 6, 2
        };

        shape triangle = peShapeCreate(VA, TRI1, 24);
            
        shape s1a = peShapeCreate(VA, CUBE1, 36);
        p3 p = {-0.2, -0.2, 0};
        shape s1at = peShapeTranslate(s1a, p);
        p3 s1arcog = peShapeGetCOG(s1a);
        shape s1ar = peShapeRotate(s1a, s1arcog, 1, 1, 1);
        // shape s1ar = peShapeRotateXZ(s1a, s1arcog, 90);

        int bfc = 0;
        int pause = 0;

        // Bottom test
        // p3 planeA[3] = { VA[0], VA[2], VA[4] };
        // p3 planeB[3]  = { VA[4], VA[2], VA[6] };

        // Front test
        p3 planeA[3] = { VA[1], VA[0], VA[2] };
        p3 planeB[3] = { VA[1], VA[2], VA[3] };

        p3 normal = get_plane_normal(planeA);
        SDL_Log("%f, %f, %f", normal.x, normal.y, normal.z);

        normal = get_plane_normal(planeB);
        SDL_Log("%f, %f, %f", normal.x, normal.y, normal.z);

        while (quit == false) {
            while (SDL_PollEvent(&e) == true) {
                if (e.type == SDL_EVENT_QUIT) {
                    quit = true;
                } else if (e.type == SDL_EVENT_KEY_DOWN) {
                    if (e.key.key == SDLK_0) {
                        bfc = 0;
                    } else if (e.key.key == SDLK_1) {
                        bfc = 1;
                    } else if (e.key.key == SDLK_SPACE) {
                        if (pause) {
                            pause = 0;
                        } else {
                            pause = 1;
                        }
                    } 
                }
            }
        
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
            SDL_RenderClear(renderer);

            double x = 1.0;
            double y = angle * PI/180 * 0.01;
            double z =-70.0;

            peShapeDraw(s1ar, bfc);
            if (!pause) {
                s1ar = peShapeRotate(s1a, s1arcog, x, y, z);
            }
          
            angle += 1;

            SDL_RenderPresent(renderer);
        }
    }
}