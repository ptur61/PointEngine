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

SDL_Renderer* peRenderer = NULL;
SDL_Window*   peWindow   = NULL;
int screen_width         = 0;
int screen_height        = 0;

int peRed   = 0;
int peGreen = 0;
int peBlue  = 0;

// ==================================================================
// UTILITY METHODS
// ==================================================================

int peCmpP3(p3 pA, p3 pB) { 
    if ( !(pA.x - pB.x + pA.y - pB.y + pA.z - pB.z) ) {
        return 1;
    }
    return 0;
}

p3 peSubP3(p3 pA, p3 pB) {
    p3 p = { pA.x - pB.x, pA.y - pB.y, pA.z - pB.z};
    return p;
}

bool peCreateInstance(char name[], int width, int height) {
    bool success = true;
    screen_width = width;
    screen_height = height;

    if (SDL_Init(SDL_INIT_VIDEO) == false) {
        SDL_Log("SDL could not initialise! SDL error: %s\n", SDL_GetError());
        success = false;
    } else {
        if (SDL_CreateWindowAndRenderer(name, width, height, 0, &peWindow, &peRenderer) == false ) {
            SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
            success = false;
        } else if (TTF_Init() == false) {
            SDL_Log("SDL_ttf could not initialise! SDL_ttf error: %s\n", SDL_GetError());
            success = false;
        }
    }
    return success;
}

SDL_Renderer* peGetRenderer() {
    return peRenderer;
}

SDL_Window* peGetWindow() {
    return peWindow;
}

void peSetDrawColour(int r, int g, int b) {
    peRed   = r;
    peGreen = g;
    peBlue  = b;
}


// ==================================================================
// STATIC UTILITY METHODS
// ==================================================================

static p2 p3_to_p2(p3 p) {
    double x = p.x / p.z;
    double y = p.y / p.z;
    p2 point = { ((x + 1) / 2) * screen_width, (1 - (y + 1) / 2) * screen_height };
    return point;
}

static p3 get_plane_normal(p3 VA[3]) {
    p3 u = peSubP3(VA[1], VA[0]);
    p3 v = peSubP3(VA[2], VA[0]);
    p3 normal = { u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x };
    return normal;
}

static uint8_t is_plane_backface(p3 plane[3]) {
    p3 N = get_plane_normal(plane);
    p3 V = plane[0];  // Arbitrary vector from the plane
    double D = N.x*V.x + N.y*V.y + N.z*V.z;
    if (D > 0) {
        return 0;
    }
    return 1;
}



// ===============================================
// POINT & LINE METHODS
// ===============================================

void pePointDraw(p3 p) {
    p2 point = p3_to_p2(p);
    SDL_SetRenderDrawColor(peRenderer, peRed, peGreen, peBlue, 255);
    SDL_RenderPoint(peRenderer, point.x, point.y);
}

void peLineDraw(p3 pA, p3 pB) {
    p2 point_a = p3_to_p2(pA);
    p2 point_b = p3_to_p2(pB);
    SDL_SetRenderDrawColor(peRenderer, peRed, peGreen, peBlue, 255);
    SDL_RenderLine(peRenderer, point_a.x, point_a.y, point_b.x, point_b.y);
}

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
    p3 *new_VA = malloc(size * sizeof(p3));
    memcpy(new_VA, src.VA, size * sizeof(p3));
    shape shp = { new_VA, size };
    return shp;
}

//Not needed if malloc isn't used
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

    shape peShapeRotateXZ(shape tgt, p3 p, double r) {  // NOT MEMORY SAFE
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
        shape shp_origin = peShapeTranslate(shp, npivot);
        peShapeDelete(shp);
        for (int i = 0; i < shp_origin.size; i++) {
            p3 point = shp_origin.VA[i];
            shp_origin.VA[i] = pePointRotate(point, x, y, z);
        }
        shape shp_final = peShapeTranslate(shp_origin, pivot);
        peShapeDelete(shp_origin);
        return shp_final;
    }