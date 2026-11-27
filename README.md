# PointEngine
_06/09/26_

PointEngine is a 3D graphics engine that uses perspective projection of 3D points (x, y, z) to represent 3D images on screen.
The system is built using points, or vertices, where a collection of vertices is called a shape.

## Features
#### Completed
1. Change colour of points and shapes
2. Translate and rotate points and shapes
3. Backface-culling

#### Coming soon
* Viewing-frustum culling and other hidden-surface removal (to be elaborated upon when I know what needs doing...)
* Lighting and shading system (Phong Shading most likely)
* Application to create shapes visually with the mouse (writing backface-culling-compatible shapes manually is very tedious!)

## How to run the demo

### With CMake
Compiling with CMake is recommended.\
```cmake -B build```\
```cmake --build build```\
Executables can be found in /build

Please contact me if there are any problems (ptur61@gmail.com)
