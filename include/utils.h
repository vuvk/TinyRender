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

#ifndef __UTILS_H_INCLUDED
#define __UTILS_H_INCLUDED

#include <math.h>

//#include "SDL/SDL_stdinc.h"

#include "types.h"
#include "color.h"
#include "vectors/vectors.h"


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

#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

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
#define tr_swap(A, B) _Generic ((A),                   \
                             int16*  : SwapShort,   \
                             int32*  : SwapInt  ,   \
                             int64*  : SwapLong ,   \
                             float*  : SwapFloat,   \
                             default : SwapPtr      \
                            )(A, B)

// operator ==
#define tr_equals(A,B) _Generic((A),    \
                        TrVector2i : tr_vec2i_equals,   \
                        TrVector2f : tr_vec2f_equals,   \
                        TrVector3i : tr_vec3i_equals,   \
                        TrVector3f : tr_vec3f_equals,   \
                        TrVector4i : tr_vec4i_equals,   \
                        TrVector4f : tr_vec4f_equals    \
                        )(A,B)

// operator '+'
#define tr_add(A, B) _Generic ((A),     \
                        TrVector2i : tr_vec2i_add,    \
                        TrVector2f : tr_vec2f_add,    \
                        TrVector3i : tr_vec3i_add,    \
                        TrVector3f : tr_vec3f_add,    \
                        TrVector4i : tr_vec4i_add,    \
                        TrVector4f : tr_vec4f_add,    \
                        RGBColor   : AddRGBColor,    \
                        RGBColorf  : AddRGBColorf,   \
                        RGBAColor  : AddRGBAColor,   \
                        RGBAColorf : AddRGBAColorf   \
                           )(A, B)
// operator '-'
#define SUB(A, B) _Generic ((A),                         \
                            TrVector2f  : SubVector2f,    \
                            TrVector2i  : SubVector2i,    \
                            TrVector3f  : SubVector3f,    \
                            TrVector3i  : SubVector3i,    \
                            TrVector4f  : SubVector4f,    \
                            TrVector4i  : SubVector4i,    \
                            RGBColor   : SubRGBColor,    \
                            RGBColorf  : SubRGBColorf,   \
                            RGBAColor  : SubRGBAColor,   \
                            RGBAColorf : SubRGBAColorf   \
                           )(A, B)
// operator '*'
#define MUL(A, B) _Generic ((A),                           \
                             TrVector2f  : MultVector2f,     \
                             TrVector2i  : MultVector2i,     \
                             TrVector3f  : MultVector3f,     \
                             TrVector3i  : MultVector3i,     \
                             TrVector4f  : MultVector4f,     \
                             TrVector4i  : MultVector4i,     \
                             RGBColor   : MultRGBColor,     \
                             RGBColorf  : MultRGBColorf,    \
                             RGBAColor  : MultRGBAColor,    \
                             RGBAColorf : MultRGBAColorf    \
                             )(A, B)
// operator '/'
#define DIV(A, B) _Generic ((A),                          \
                            TrVector2f  : DivVector2f,     \
                            TrVector2i  : DivVector2i,     \
                            TrVector3f  : DivVector3f,     \
                            TrVector3i  : DivVector3i,     \
                            TrVector4f  : DivVector4f,     \
                            TrVector4i  : DivVector4i,     \
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

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__UTILS_H_INCLUDED
