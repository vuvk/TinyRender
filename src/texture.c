#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef USE_SDL1
#include "SDL/SDL.h"
#else  // USE_SDL2
#include "SDL2/SDL.h"
#endif // USE_SDL1

#include "texture.h"
#include "utils.h"
#include "render_private.h"

inline STexture* TextureCreate()
{
    return calloc(1, sizeof(STexture));
}

void TextureLoadFromFile(STexture* texture, const char* fileName)
{
    if (texture == NULL || !FileExist(fileName))
        return;

    SDL_Surface* surf = SDL_LoadBMP(fileName);
    if (surf == NULL)
    {
        printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
        return;
    }

    surf = SDL_ConvertSurface(surf, frameBuffer->format, 0);

    texture->width  = surf->w;
    texture->height = surf->h;
    texture->bpp    = surf->format->BytesPerPixel;
    texture->pitch  = surf->pitch;
    texture->pixels = calloc(1, surf->w * surf->h * surf->format->BytesPerPixel);
    memcpy(texture->pixels, surf->pixels, surf->w * surf->h * surf->format->BytesPerPixel);

    SDL_FreeSurface(surf);
}

void TextureDestroy(STexture** texture)
{
    if (texture == NULL || *texture == NULL)
        return;

    free((*texture)->pixels);
    free(*texture);

    (*texture)->pixels = NULL;
    *texture = NULL;
}

