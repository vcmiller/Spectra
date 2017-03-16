del triangle-vert.spv
del triangle-frag.spv

..\..\Libraries\vulkan\Bin32\glslangValidator.exe -V triangle.vert
..\..\Libraries\vulkan\Bin32\glslangValidator.exe -V triangle.frag

ren vert.spv triangle-vert.spv
ren frag.spv triangle-frag.spv
pause