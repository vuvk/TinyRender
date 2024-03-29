/**

   Copyright 2017, 2021 Anton "Vuvk" Shcherbatykh (vuvk69@gmail.com)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SDL2/SDL.h"

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

