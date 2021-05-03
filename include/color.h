/*
	It's part of bodhi GameEngine.
    Copyright (C) 2017 Anton "Vuvk" Shcherbatykh

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "types.h"
#include "render_private.h"

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


