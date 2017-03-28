Vincent Miller

Video: https://www.youtube.com/watch?v=q0T5742MsAw

------------------------------ COMPILE AND RUN ---------------------------------------------

This project was done using a custom engine programmed in C++, using Vulkan for rendering.
Vulkan should run on the zoo lab computers, as long as it is built in RELEASE mode (64 bit).

Note: Due to using a custom engine, I had to include all of the engine files and libraries
in the submission, which will make it much larger than a Unity project.

Buliding in debug mode or 32-bit will cause the program to crash!

To compile and run:
Open Spectra.sln in Visual Studio 2015
Make sure it's set to build in release mode, 64 bit
Make sure "Computer Animation Project 1" is set as the startup project
Build the solution, then start without debugging

------------------------------ IMPLEMENTATION DETAILS ---------------------------------------------

The code specific to this project is in the project Computer Animation Project 1
If the main file you open has nothing but commented-out code, that is the wrong project.

Press space bar to swap splines (between catmull-rom and b-spline)

Note: SplineInterpolator contains code to perform forward differencing, but this is not used
as it would cause the last few points to be skipped (since they are at the same position).

------------------------------ CONTROLS ---------------------------------------------

WASD - Move Camera
RMB + Mouse - Rotate Camera
SPACE - Swap splines

------------------------------ FILES ---------------------------------------------

main - Contains code for setting up scene (and camera control)
BSplineInterpolator - handles position approximation using b spline
CatmullRomInterpolator - handles position interpolation using catmull-rom
CubicSegment - Definition for a single cubic function
CubicSegmentVector - Group of three cubic functions (x, y, z)
MyQuaternion - Custom implementation of quaternion math
PathInterpolator - Base class for interpolation/approximation functions
RotationInterpolator - handles rotation interpolation
Spline - handles reading spline file
SplineMovementComponent - handles moving an object along a spline curve (and rotating)
SplineSwapComponent - handles swapping splines in the SplineMovementComponent when spacebar is pressed

------------------------------ CREDITS ---------------------------------------------

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