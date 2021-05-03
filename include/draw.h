#pragma once

#include <stdio.h>

#ifdef USE_SDL1
#include "SDL/SDL.h"
#else  // USE_SDL2
#include "SDL2/SDL.h"
#endif // USE_SDL1

#include "types.h"
#include "texture.h"


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

