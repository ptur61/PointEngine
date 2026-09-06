#!/bin/bash
gcc -g -o bin/example_1 source/examples/example_1.c source/PointEngine.c -L/usr/local/lib -lSDL3 -lm -I vendored/SDL/include -I include
gcc -g -o bin/example_2 source/examples/example_2.c source/PointEngine.c -L/usr/local/lib -lSDL3 -lm -I vendored/SDL/include -I include
# gcc -g -o bin/PointEditor source/PointEditor.c -L/usr/local/lib -lSDL3 -lSDL3_ttf -lSDL3_image -lm -I vendored/SDL/include -I vendored/SDL_image/include -I vendored/SDL_ttf/include -I include