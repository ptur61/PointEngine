# PointEngine
_06/09/26_

PointEngine is a 3D graphics engine that uses perspective projection of 3D points (x, y, z) to represent 3D images on screen.
The system is built using points, or vertices, where a collection of vertices is called a shape.

## Features

[x] Change colour of points and shapes
[x] Translate and rotate points and shapes
[x] Backface-culling
[ ] Viewing-frustum culling and other hidden-surface removal (to be elaborated upon when I know what needs doing...)
[ ] Lighting and shading system (Phong Shading most likely)
[ ] Application to create shapes visually with the mouse (writing backface-culling-compatible shapes manually is very tedious!)

## How to run the demo

I have compiled the software on my Linux machine using GCC. Please note that SDL3 (https://wiki.libsdl.org/SDL3/FrontPage) is required.
The compile.sh file will compile the software if your SDL3 library is located at /usr/local/lib. Otherwise, change the path stated in the file to the location of 
your SDL3 install, or use another method of compilation. The demo files included are:
    * example_1.c
    * example_2.c

You can run the demo files by executing the bash file run.sh with the file number you wish to open. For example, to open example_1, you would write in your command line 
```
bash run.sh 1
```

Please contact me if there are any problems (ptur61@gmail.com)