#ifndef POINTENGINE_H_
#define POINTENGINE_H_

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdint.h>


// ==================================================================
// STRUCTS
// ==================================================================

typedef struct {
    double x, y;
} p2;

typedef struct {
    double x, y, z;
} p3;

typedef struct {
    p3 position;
    p3 rotation;
} camera;

typedef struct {
    p3 *VA;     // Vertex Array
    int size;
} shape;



// ==================================================================
// UTILITY METHODS
// ==================================================================

/**
 * Compares equality of two points.
 * 
 * \param p1 the first point.
 * \param p2 the second point.
 * \returns 1 on equality, 0 otherwise.
 */
int peCmpP3(p3 pA, p3 pB);

/**
 * Subtracts one point from another.
 * 
 * \param p1 the starting point.
 * \param p2 the point to be subtracted.
 * \returns p3; the resulting point.
 */
p3 peSubP3(p3 pA, p3 pB);

/**
 * Initialises all relevant SDL libraries and creates an SDL window and renderer.
 * 
 * \param name the name of the instance.
 * \param width the screen width.
 * \param height the screen height.
 * \returns true if all SDL initialisations succeed, false if otherwise.
 */
bool peCreateInstance(char name[], int width, int height);

/**
 * Returns the peRenderer global variable.
 * 
 * \returns peRenderer.
 */
SDL_Renderer* peGetRenderer();

/**
 * Returns the peWindow global variable.
 * 
 * \returns peWindow.
 */
SDL_Window* peGetWindow();

/**
 * Sets the draw colour for each point and line.
 * 
 * \param r Red value (0 - 255)
 * \param g Green value (0 - 255)
 * \param b Blue value (0 - 255)
 */
void peSetDrawColour(int r, int g, int b);


// ==================================================================
// STATIC UTILITY METHODS
// ==================================================================

static p2 p3_to_p2(p3 p);
static p3 get_plane_normal(p3 VA[3]);
static uint8_t is_plane_backface(p3 plane[3]);



// ==================================================================
// POINT & LINE METHODS
// ==================================================================

void pePointDraw(p3 p);
void peLineDraw(p3 pA, p3 pB);

    //  --------------------------------------------------------------
    //  TRANSLATION & ROTATION
    //  --------------------------------------------------------------

    p3 pePointTranslate(p3 p, p3 v);
    p3 pePointRotateXY(p3 p, double r);
    p3 pePointRotateXZ(p3 p, double r);
    p3 pePointRotate(p3 src, double x, double y, double z);



// ==================================================================
// SHAPE METHODS
// ==================================================================

shape peShapeCreate(p3 *VA, int *VM, int size);
shape peShapeCopy(shape src, int size);
void peShapeDelete(shape tgt);
p3 peShapeGetCOG(shape s);
void peShapeDraw(shape src, uint8_t backface_culling);

    //  -------------------------------------------------------------
    //  TRANSLATION & ROTATION
    //  -------------------------------------------------------------

    shape peShapeTranslate(shape tgt, p3 vector);
    shape peShapeRotateXZ(shape tgt, p3 p, double r);
    shape peShapeRotate(shape tgt, p3 pivot, double x, double y, double z);



#endif