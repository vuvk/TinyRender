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

#ifndef __UTILS_H_INCLUDED
#define __UTILS_H_INCLUDED

#include <math.h>

//#include "SDL/SDL_stdinc.h"

#include "types.h"
#include "color.h"
#include "vector.h"


// fix math
#include "libfixmath/fixmath.h"
#define dbl_to_fix  fix16_from_dbl
#define fix_to_dbl  fix16_to_dbl
#define flt_to_fix  fix16_from_float
#define fix_to_flt  fix16_to_float
#define int_to_fix  fix16_from_int
#define fix_to_int  fix16_to_int
#define f_add       fix16_add
#define f_sub       fix16_sub
#define f_mul       fix16_mul
#define f_div       fix16_div
#define f_sqrt      fix16_sqrt
#define f_abs       fix16_abs
#define f_floor     fix16_floor
#define f_mod       fix16_mod
#define f_one       fix16_one


#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif


#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define SQR(a) (a)*(a)

/* math */
//static const float PI_2 = 2.0f*M_PI;

float fast_sqrt(float val);

#define DEG_TO_RAD(angle) (angle) * 0.01745329251994329576923690768489  // M_PI / 180.0
#define RAD_TO_DEG(angle) (angle) * 57.295779513082320876798154814105   // 180.0 / M_PI

/* limit number in range */
#define LIMIT(number, min, max) if (number < (min)) number = (min); \
                                if (number > (max)) number = (max);

/* swap anything */
void SwapPtr(void** a, void** b);
void SwapShort(int16* a, int16* b);
void SwapInt(int32* a, int32* b);
void SwapLong(int64* a, int64* b);
void SwapFloat(float* a, float* b);
#define SWAP(A, B) _Generic ((A),                   \
                             int16*  : SwapShort,   \
                             int32*  : SwapInt  ,   \
                             int64*  : SwapLong ,   \
                             float*  : SwapFloat,   \
                             default : SwapPtr      \
                            )(A, B)

// operator '+'
#define ADD(A, B) _Generic ((A),                         \
                            SVector2f  : AddVector2f,    \
                            SVector2i  : AddVector2i,    \
                            SVector3f  : AddVector3f,    \
                            SVector3s  : AddVector3s,    \
                            SVector3i  : AddVector3i,    \
                            SVector4f  : AddVector4f,    \
                            SVector4i  : AddVector4i,    \
                            RGBColor   : AddRGBColor,    \
                            RGBColorf  : AddRGBColorf,   \
                            RGBAColor  : AddRGBAColor,   \
                            RGBAColorf : AddRGBAColorf   \
                           )(A, B)
// operator '-'
#define SUB(A, B) _Generic ((A),                         \
                            SVector2f  : SubVector2f,    \
                            SVector2i  : SubVector2i,    \
                            SVector3f  : SubVector3f,    \
                            SVector3s  : SubVector3s,    \
                            SVector3i  : SubVector3i,    \
                            SVector4f  : SubVector4f,    \
                            SVector4i  : SubVector4i,    \
                            RGBColor   : SubRGBColor,    \
                            RGBColorf  : SubRGBColorf,   \
                            RGBAColor  : SubRGBAColor,   \
                            RGBAColorf : SubRGBAColorf   \
                           )(A, B)
// operator '*'
#define MUL(A, B) _Generic ((A),                           \
                             SVector2f  : MultVector2f,     \
                             SVector2i  : MultVector2i,     \
                             SVector3f  : MultVector3f,     \
                             SVector3s  : MultVector3s,     \
                             SVector3i  : MultVector3i,     \
                             SVector4f  : MultVector4f,     \
                             SVector4i  : MultVector4i,     \
                             RGBColor   : MultRGBColor,     \
                             RGBColorf  : MultRGBColorf,    \
                             RGBAColor  : MultRGBAColor,    \
                             RGBAColorf : MultRGBAColorf    \
                             )(A, B)
// operator '/'
#define DIV(A, B) _Generic ((A),                          \
                            SVector2f  : DivVector2f,     \
                            SVector2i  : DivVector2i,     \
                            SVector3f  : DivVector3f,     \
                            SVector3s  : DivVector3s,     \
                            SVector3i  : DivVector3i,     \
                            SVector4f  : DivVector4f,     \
                            SVector4i  : DivVector4i,     \
                            RGBColor   : DivRGBColor,     \
                            RGBColorf  : DivRGBColorf,    \
                            RGBAColor  : DivRGBAColor,    \
                            RGBAColorf : DivRGBAColorf    \
                           )(A, B)

/* strings */
/** Safely compares strings
 *  return:
 *      true  if compares
 *      false if not compares
 *      -1    if error
 */
int8 StrEqual (const char* str1, const char* str2);
/** Safely copy string from src(source) to dst(destination)
 *  limit length string as maxLength WITH null-terminator \0!!
 */
void StrCopy(char* dst, const char* src, uint32 maxLength);
/** Safely calculate length of string WITHOUT null-terminator \0!!
 *  limit length string as MAX_STRING_LENGTH
 *  return -1 if error
 */
int32 StrLength(const char* string);

bool FileExist(const char* fileName);

#endif //__UTILS_H_INCLUDED
