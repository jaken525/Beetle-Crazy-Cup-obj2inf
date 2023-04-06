# obj2inf
This program is designed to convert obj files of 3d models into 4 files: inf, nor (not always), tri and ver. These files were used by Xpiral for games such as Beetle Crazy Cup (Beetle Buggin', Käfer Total), Radiсal Drive and Larry Ragland 4x4 Challenge.

# Instruction
The number of polygons must be no more than 2000. You won't be able to load the resulting model with a large number of polygons into the game! Textures should have a color depth of 8 bits, 256x256 pixels (for software render mode) and higher (for Direct3D render) with a resolution of 72 dpi in bmp format. The convertible model must be adjusted in size.

The UV scan or texture must be reflected vertically.

The approximate dimensions and directions of the model can be taken from the original model "1_CARRO.obj".

The model must be made of triangles.

1. Open "obj2inf.exe".
2. Choose the model file.
3. Enter the number of the texture that will replace the original one.

obj2inf.h header file to use in your program.
