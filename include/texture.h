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
