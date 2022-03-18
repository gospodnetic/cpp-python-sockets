
#include <string>
// STB.
// https://stackoverflow.com/questions/2076475/reading-an-image-file-in-c-c
// https://github.com/nothings/stb

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

void read_image(string path, uint8_t* rgb_image, int *width, int *height, int *bpp) 
{
    rgb_image = stbi_load(path.c_str(), width, height, bpp, 3);
    return;
}

void clear_image(uint8_t* rgb_image)
{
    stbi_image_free(rgb_image);
    return;
}
