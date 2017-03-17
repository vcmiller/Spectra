Vincent Miller

This project was done using a custom engine programmed in C++, using Vulkan for rendering.
Vulkan will run on the zoo lab computers, as long as it is built in RELEASE mode (64 bit).

Note: Due to using a custom engine, I had to include all of the engine files and libraries
in the submission, which will make it much larger than a Unity project.

Buliding in debug mode or 32-bit will cause the program to crash!

To compile and run:
Open Spectra.sln in Visual Studio 2015
Make sure it's set to build in release mode, 64 bit
Make sure "Computer Animation Project 0" is set as the startup project
Build the solution, then start without debugging

The code specific to this project is in the "main.cpp" in project Computer Animation Project 0
If the main file you open has nothing but commented-out code, that is the wrong project.

The main file contains three things:
a Component class for defining the motion of the object (same as MonoBehaviour in Unity)
a Scene class for loading assets and creating objects (like a scene file in Unity)
a two-line main function for starting the engine

Libraries used by engine:
Vulkan (LunarG SDK)
GLFW
GLM
tinyobjloader
stb
jsoncpp

Assets used:
Texture from textures.com
Monkey from blender primatives