#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

const int WIDTH  = 640;
const int HEIGHT = 640;

SDL_Renderer *RENDERER;
SDL_Window   *WINDOW;

bool init(char name[], int width, int height) {
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) == false) {
        SDL_Log("SDL could not initialise! SDL error: %s\n", SDL_GetError());
        success = false;
    } else {
        if (SDL_CreateWindowAndRenderer(name, width, height, 0, &WINDOW, &RENDERER) == false ) {
            SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
            success = false;
        } else if (TTF_Init() == false) {
            SDL_Log("SDL_ttf could not initialise! SDL_ttf error: %s\n", SDL_GetError());
            success = false;
        // } else if (SDL_SetWindowIcon(CSIM_WINDOW, icon) == false) {
        //     SDL_Log("Window thumbnail could not be set! SDL error: %s\n", SDL_GetError());
        //     success = false;
        } else if (SDL_SetWindowMinimumSize(&WINDOW, WIDTH, HEIGHT) == false) {
            SDL_Log("Failed to set minimum CSIM_WINDOW size! SDL error: %s\n", SDL_GetError());
            success = false;
        }
    }
    return success;
}

int main(int argc, char* args[]) {
    int exit_code = 0;

    if (init("PointEditor", WIDTH, HEIGHT) == false) {
        SDL_Log("Unable to initalise program!\n");
        exit_code = 1;
    } else {
        bool quit = false;
        SDL_Event e;
        SDL_zero(e);

        while (quit == false) {
            while (SDL_PollEvent(&e) == true) {
                if (e.type == SDL_EVENT_QUIT) {
                    quit = true;
                }
            }
        }

    }
}