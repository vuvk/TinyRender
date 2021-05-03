#ifndef __TEXTURE_H_INCLUDED
#define __TEXTURE_H_INCLUDED

#include "types.h"

typedef struct
{
    void* pixels;

    int32 pitch;
    int32 bpp;  // BytesPerPixel
    int32 width;
    int32 height;
} STexture;

STexture* TextureCreate();
void TextureLoadFromFile(STexture* texture, const char* fileName);
void TextureDestroy(STexture** texture);

#endif //__TEXTURE_H_INCLUDED
