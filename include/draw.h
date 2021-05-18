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

#pragma once

#ifndef __DRAW_H_INCLUDED
#define __DRAW_H_INCLUDED

#include <stdio.h>

#include "SDL2/SDL.h"

#include "types.h"
#include "texture.h"

#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

void DrawLine (int32 x0, int32 y0, int32 x1, int32 y1, uint32 color, void* pixels);
void DrawLineBicolor (int32 x0, int32 y0, int32 x1, int32 y1, uint32 color0, uint32 color1, void* pixels);

//Draws a rectangle
void DrawRect (int32 x, int32 y, uint32 w, uint32 h, uint32 color, void* pixels);

void DrawTriangleColor(int32 x0, int32 y0, int32 x1, int32 y1, int32 x2, int32 y2, uint32 color, void* pixels);
void DrawTriangleTricolor(int32 x0, int32 y0, int32 x1, int32 y1, int32 x2, int32 y2, uint32 color0, uint32 color1, uint32 color2, void* pixels);
void DrawTriangleTextured(int32 x0, int32 y0, int32 x1, int32 y1, int32 x2, int32 y2,
                          float u0, float v0, float u1, float v1, float u2, float v2,
                          STexture* texture, void* pixels);

void DrawQuadColor(int32 x0, int32 y0, int32 x1, int32 y1, int32 x2, int32 y2, int32 x3, int32 y3, uint32 color, void* pixels);
void DrawQuadFourcolor(int32 x0, int32 y0, int32 x1, int32 y1, int32 x2, int32 y2, int32 x3, int32 y3,
                       uint32 color0, uint32 color1, uint32 color2, uint32 color3, void* pixels);
void DrawQuadTextured(int32 x0, int32 y0, int32 x1, int32 y1, int32 x2, int32 y2, int32 x3, int32 y3,
                      float u0, float v0, float u1, float v1, float u2, float v2, float u3, float v3,
                      STexture* texture, void* pixels);

void ClearPixels(uint32 color, int w, int h, void* pixels);
//Draws a single point. There is probably a more efficient way of doing this.
void PutPixel(int32 x, int32 y, uint32 color, void* pixels);
//Performs simple conversions to prepare a surface for accessing pixels
uint32 GetPixel_(uint32 x, uint32 y, void* pixels);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__DRAW_H_INCLUDED
