#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>
#include <math.h>

#define PI 3.141592653589793

const int WIDTH = 640;
const int HEIGHT = 640;


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;


// =======================
// TYPEDEFS
// =======================

typedef struct {
    double x;
    double y;
} p2;

typedef struct {
    double x;
    double y;
    double z;
} p3;

// typedef struct {
//     p3 (*vertices)[][3];
// } shape;

p2 point_translate(p3 p) {
    double x = p.x / p.z;
    double y = p.y / p.z;
    p2 point = { ((x + 1) / 2) * WIDTH, (1 - (y + 1) / 2) * HEIGHT };
    return point;
}

void point_draw(p3 p) {
    p2 point = point_translate(p);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderPoint(renderer, point.x, point.y);
}

void point_rotate_xy(p3 *p, double r) {
    (*p).x = (*p).x * cos(PI/180 * r) - (*p).y * sin(PI/180 * r);
    (*p).y = (*p).x * sin(PI/180 * r) + (*p).y * cos(PI/180 * r);
}

void point_rotate_xz(p3 *p, double r) {
    (*p).x = (*p).x * cos(PI/180 * r) + (*p).z * sin(PI/180 * r);
    (*p).z = (*p).x * -sin(PI/180 * r) + (*p).z * cos(PI/180 * r);
}

void shape_rotate_xy(p3 shape[][3], int elements, double r) {
    for (int i = 0; i < elements; i++) {
        for (int j = 0; j < 3; j++) {
            point_rotate_xy(&shape[i][j], r);
        }
    }
}

void shape_rotate_xz(p3 shape[][3], int elements, double r) {
    for (int i = 0; i < elements; i++) {
        for (int j = 0; j < 3; j++) {
            point_rotate_xz(&shape[i][j], r);
        }
    }

}

void shape_point_rotate_xy(p3 shape[][3], int elements, p3 point, double r) {
    for (int i = 0; i < elements; i++) {
        for (int j = 0; j < 3; j++) {
            shape[i][j].x -= point.x;
            shape[i][j].y -= point.y;
            shape[i][j].z -= point.z;
        }
    }

    shape_rotate_xy(shape, elements, r);

    for (int i = 0; i < elements; i++) {
        for (int j = 0; j < 3; j++) {
            shape[i][j].x += point.x;
            shape[i][j].y += point.y;
            shape[i][j].z += point.z;
        }
    }
    
}

void shape_point_rotate_xz(p3 shape[][3], int elements, p3 point, double r) {
    for (int i = 0; i < elements; i++) {
        for (int j = 0; j < 3; j++) {
            shape[i][j].x -= point.x;
            shape[i][j].y -= point.y;
            shape[i][j].z -= point.z;
        }
    }

    shape_rotate_xz(shape, elements, r);

    for (int i = 0; i < elements; i++) {
        for (int j = 0; j < 3; j++) {
            shape[i][j].x += point.x;
            shape[i][j].y += point.y;
            shape[i][j].z += point.z;
        }
    }
    
}

p3 shape_get_cog(p3 shape[][3], int elements) {
    // cog: centre of gravity
    double x, y, z;
    for (int i = 0; i < elements; i++) {
        for (int j = 0; j < 3; j++) {
            p3 point = shape[i][j];
            x += point.x;
            y += point.y;
            z += point.z;
        }
    }
    x /= elements * 3;
    y /= elements * 3;
    z /= elements * 3;
    p3 cog = {x, y, z};
    return cog;
}


void line_draw(p3 p_a, p3 p_b) {
    p2 point_a = point_translate(p_a);
    p2 point_b = point_translate(p_b);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderLine(renderer, point_a.x, point_a.y, point_b.x, point_b.y);
}

void shape_draw(p3 shape[][3], int elements) {
    for (int i = 0; i < elements; i++) {
        for (int j = 0; j < 3; j++) {
            point_draw(shape[i][j]);
            line_draw(shape[i][j], shape[i][(j + 1)%3]);
            line_draw(shape[i][j], shape[(i + 1)%elements][j]);
        }
    }
}


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

        int now = SDL_GetPerformanceCounter();
        int last = 0;
        double dt = 0;

        double angle = 1/60;
        p3 shape[][3] = { {{-0.5, 0.2, 3.0}, {-0.5, -0.2, 3.0}, {-0.3, -0.2, 3.0}},
                          {{-0.5, 0.2, 3.2}, {-0.5, -0.2, 3.2}, {-0.3, -0.2, 3.2}},
                          {{-0.3, 0.2, 3.0}, {-0.3, -0.2, 3.0}, {-0.5,  0.2, 3.0}},
                          {{-0.3, 0.2, 3.2}, {-0.3, -0.2, 3.2}, {-0.5,  0.2, 3.2}} };

        p3 shape2[][3] = { {{-0.2, 0.5, 3.0}, {0.2, 0.5, 3.0}, {0.0, 0.0, 3.1}},
                           {{-0.2, 0.5, 3.2}, {0.2, 0.5, 3.2}, {0.0, 0.0, 3.1}}, };

        while (quit == false) {
            while (SDL_PollEvent(&e) == true) {
                if (e.type == SDL_EVENT_QUIT) {
                    quit = true;
                }
            }
        
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
            SDL_RenderClear(renderer);

            last = now;
            now = SDL_GetPerformanceCounter();
            dt = (now - last) / (double)SDL_GetPerformanceFrequency();
            
            // shape_rotate_xy(shape, 2, (100 * dt));
            // shape_rotate_xz(shape, 2, (100 * dt));
            p3 cog = shape_get_cog(shape2, 2);
            SDL_Log("%f, %f, %f", cog.x, cog.y, cog.z);
            shape_point_rotate_xz(shape2, 2, cog, (100 * dt));
            shape_draw(shape2, 2);
            angle += 1/60;

            // shape_draw(shape2, 2);

            SDL_RenderPresent(renderer);
        }
    }
}