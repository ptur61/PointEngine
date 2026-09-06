#ifndef POINTENGINE_H_
#define POINTENGINE_H_

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdint.h>


// ==================================================================
// STRUCTS
// ==================================================================

/**
 * Two-dimensional vector.
 */
typedef struct {
    double x, y;
} p2;

/**
 * Three-dimensional vector.
 */
typedef struct {
    double x, y, z;
} p3;

/**
 * Consists of two p3s for position and rotation.
 */
typedef struct {
    p3 position;
    p3 rotation;
} camera;

/**
 * Contains the data needed for a vertex array to be drawn onto the screen
 * 
 * \param VA the vertex array.
 * \param size the number of items in the vertex array.
 */
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

/**
 * Translates a three-dimensional vector to a two-dimensional one using perspective division.
 * 
 * \param p three-dimensional vector.
 * \returns normalised two-dimensional vector.
 */
static p2 p3_to_p2(p3 p);

/**
 * Returns the normal vector of a triangle.
 * 
 * \param VA three-pointed vertex array.
 * \returns plane normal.
 */
static p3 get_plane_normal(p3 VA[3]);

/**
 * Calculates whether the given three-pointed plane is 'backfacing' or not.
 * 
 * \param VA three-pointed vertex array.
 * \returns 1 if plane is backfacing.
 */
static uint8_t is_plane_backface(p3 plane[3]);



// ==================================================================
// POINT & LINE METHODS
// ==================================================================

/**
 * Renders a 3D point onto 2D space in the colour defined by the PE colour variables.
 * 
 * \param p 3D point to draw.
 */
void pePointDraw(p3 p);

/**
 * Renders a 2D line from two 3D points.
 * 
 *\param pA the first  point.
 *\param pB the second point.
 */
void peLineDraw(p3 pA, p3 pB);

    //  --------------------------------------------------------------
    //  TRANSLATION & ROTATION
    //  --------------------------------------------------------------

    /**
     * Creates a new point from an original point and a translation vector.
     * 
     * \param src original point.
     * \param v translation vector.
     * \returns translated point.
     */
    p3 pePointTranslate(p3 src, p3 v);

    /**
     * Creates a new point from an original point and x, y, z degrees in radians.
     * 
     * \param src original point.
     * \param x x rotation value.
     * \param y y rotation value.
     * \param z z rotation value.
     * \returns rotated point.
     */
    p3 pePointRotate(p3 src, double x, double y, double z);


    // p3 pePointRotateXY(p3 p, double r);
    // p3 pePointRotateXZ(p3 p, double r);



// ==================================================================
// SHAPE METHODS
// ==================================================================

/**
 * Creates new shape struct using the given vertex array and vertex matrix. 
 * !!!This function allocates the shape on the heap so remember to delete the shape using peShapeDelete after use!!!
 * 
 * \param VA vertex array.
 * \param VM vertex matrix.
 * \param size number of vertices in the matrix.
 * \returns the shape struct allocated on the heap.
 */
shape peShapeCreate(p3 *VA, int *VM, int size);

/**
 * Copies the data from one shape struct to another.
 * !!!This function allocates the shape on the heap so remember to delete the shape using peShapeDelete after use!!!
 * 
 * \param src the shape to be copied.
 * \param size the number of verticies in the shape.
 * \returns the shape struct allocated on the heap.
 */
shape peShapeCopy(shape src, int size);

/** 
 * Removes the given shape instance from the heap
 * 
 * \param tgt the shape to be deleted.
 */
void peShapeDelete(shape tgt);

/**
 * Get the centre-of-gravity (cog) of a given shape.
 * 
 * \param src the shape the cog will be taken from.
 * \returns the cog as a 3D vector.
 */
p3 peShapeGetCOG(shape src);

/**
 * Draw the given shape instance onto the screen.
 * 
 * \param src the shape to be drawn.
 * \param backface_culling should a backface-culling algorithm be applied to the shape?
 */
void peShapeDraw(shape src, uint8_t backface_culling);

    //  -------------------------------------------------------------
    //  TRANSLATION & ROTATION
    //  -------------------------------------------------------------

    /**
     * Creates a new shape from an original shape and a translation vector.
     * 
     * \param tgt original point.
     * \param vector translation vector.
     * \returns translated shape.
     */
    shape peShapeTranslate(shape tgt, p3 vector);

    /**
     * Creates a new shape from an original point and x, y, z degrees in radians.
     * 
     * \param tgt original shape.
     * \param pivot the pivot on which to rotate the shape.
     * \param x x rotation value.
     * \param y y rotation value.
     * \param z z rotation value.
     * \returns rotated shape.
     */
    shape peShapeRotate(shape tgt, p3 pivot, double x, double y, double z);

    // shape peShapeRotateXZ(shape tgt, p3 p, double r);


#endif