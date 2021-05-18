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

#ifndef __COLOR_H_INCLUDED
#define __COLOR_H_INCLUDED

#include "types.h"
#include "render_private.h"

#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

typedef struct
{
    uint8 r, g, b;
} RGBColor;

typedef struct
{
    uint8 r, g, b, a;
} RGBAColor;

typedef struct
{
    uint8 a, r, g, b;
} ARGBColor;

typedef struct
{
    float r, g, b;
} RGBColorf;

typedef struct
{
    float r, g, b, a;
} RGBAColorf;

/* "constructors" */
RGBColor   NewRGBColor(uint8 r, uint8 g, uint8 b);
RGBColorf  NewRGBColorf(float r, float g, float b);
RGBAColor  NewRGBAColor(uint8 r, uint8 g, uint8 b, uint8 a);
RGBAColorf NewRGBAColorf(float r, float g, float b, float a);

/* equals */
bool IsEqualRGBColor(RGBColor col1, RGBColor col2);
bool IsEqualRGBColorf(RGBColorf col1, RGBColorf col2);
bool IsEqualRGBAColor(RGBAColor col1, RGBAColor col2);
bool IsEqualRGBAColorf(RGBAColorf col1, RGBAColorf col2);

/* math operations */
/* addition */
RGBColor   AddRGBColor(RGBColor col1, RGBColor col2);
RGBColorf  AddRGBColorf(RGBColorf col1, RGBColorf col2);
RGBAColor  AddRGBAColor(RGBAColor col1, RGBAColor col2);
RGBAColorf AddRGBAColorf(RGBAColorf col1, RGBAColorf col2);
/* subtract */
RGBColor   SubRGBColor(RGBColor col1, RGBColor col2);
RGBColorf  SubRGBColorf(RGBColorf col1, RGBColorf col2);
RGBAColor  SubRGBAColor(RGBAColor col1, RGBAColor col2);
RGBAColorf SubRGBAColorf(RGBAColorf col1, RGBAColorf col2);
/* multiplication by scalar */
RGBColor   MultRGBColor(RGBColor col, float scalar);
RGBColorf  MultRGBColorf(RGBColorf col, float scalar);
RGBAColor  MultRGBAColor(RGBAColor col, float scalar);
RGBAColorf MultRGBAColorf(RGBAColorf col, float scalar);
/* division by scalar */
RGBColor   DivRGBColor(RGBColor col, float scalar);
RGBColorf  DivRGBColorf(RGBColorf col, float scalar);
RGBAColor  DivRGBAColor(RGBAColor col, float scalar);
RGBAColorf DivRGBAColorf(RGBAColorf col, float scalar);

uint8 RedFromRGB(uint32 color);
uint8 GreenFromRGB(uint32 color);
uint8 BlueFromRGB(uint32 color);


#define _RGB565_PACK(r, g, b) ( ((((r) * 31 + 127) / 255) << 11) \
                               |((((g) * 63 + 127) / 255) << 5) \
                               | (((b) * 31 + 127) / 255) )
#define _RGB565_UNPACK_RED(x)   ((((x) >> 8) & 0xf8) | ((x) >> 13))
#define _RGB565_UNPACK_GREEN(x) ((((x) >> 3) & 0xfc) | (((x) >> 9) & 0x03))
#define _RGB565_UNPACK_BLUE(x)  ((((x) << 3) & 0xf8) | (((x) >> 2) & 0x07))

/*
#define RED_FROM_COLOR(color)   ((trBits == 32 || trBits == 24) ? (color >> 0)  : RGB565_UNPACK_RED(color))
#define GREEN_FROM_COLOR(color) ((trBits == 32 || trBits == 24) ? (color >> 8)  : RGB565_UNPACK_GREEN(color))
#define BLUE_FROM_COLOR(color)  ((trBits == 32 || trBits == 24) ? (color >> 16) : RGB565_UNPACK_BLUE(color))
*/

#define RGB_TO_COLOR(r, g, b) (trBits == 32) ? ((r << 0 ) | (g << 8) | (b << 16) | (255 << 24)) :  \
                              (trBits == 24) ? ((r << 0 ) | (g << 8) | (b << 16))               :  \
                                               _RGB565_PACK((r), (g), (b))

#ifdef __cplusplus
}
#endif //__cplusplus


#endif //__COLOR_H_INCLUDED
