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

#ifndef __VECTOR3_H_INCLUDED
#define __VECTOR3_H_INCLUDED

#include "types.h"
#include "vectors/vector2.h"

typedef struct _TrVector3i
{
    union
    {
        struct
        {
            union
            {
                struct _TrVector2i;
                TrVector2i v2;
            };
            int32 z;
        };

        int32 a3[3];
    };
} TrVector3i;

typedef struct _TrVector3f
{
    union
    {
        struct
        {
            union
            {
                struct _TrVector2f;
                TrVector2f v2;
            };
            float z;
        };

        float a3[3];
    };
} TrVector3f;

/* zero vectors */
static const TrVector3i TR_ZERO3i = { 0 };
static const TrVector3f TR_ZERO3f = { 0 };

static const TrVector3f TR_RIGHT3f    = { .x = 1.0f, .y = 0.0f, .z = 0.0f };
static const TrVector3f TR_LEFT3f     = { .x =-1.0f, .y = 0.0f, .z = 0.0f };
static const TrVector3f TR_UP3f       = { .x = 0.0f, .y = 1.0f, .z = 0.0f };
static const TrVector3f TR_DOWN3f     = { .x = 0.0f, .y =-1.0f, .z = 0.0f };
static const TrVector3f TR_FORWARD3f  = { .x = 0.0f, .y = 0.0f, .z = 1.0f };
static const TrVector3f TR_BACKWARD3f = { .x = 0.0f, .y = 0.0f, .z =-1.0f };

#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

/* equals */
bool tr_vec3i_equals (TrVector3i vec1, TrVector3i vec2);
bool tr_vec3f_equals (TrVector3f vec1, TrVector3f vec2);

/* math operations */
/* addition */
TrVector3i tr_vec3i_add (TrVector3i vec1, TrVector3i vec2);
TrVector3f tr_vec3f_add (TrVector3f vec1, TrVector3f vec2);

/* subtract */
TrVector3i tr_vec3i_sub (TrVector3i vec1, TrVector3i vec2);
TrVector3f tr_vec3f_sub (TrVector3f vec1, TrVector3f vec2);

/* multiplication by scalar */
TrVector3i tr_vec3i_mul (TrVector3i vec, float scalar);
TrVector3f tr_vec3f_mul (TrVector3f vec, float scalar);

/* division by scalar */
TrVector3i tr_vec3i_div (TrVector3i vec, float scalar);
TrVector3f tr_vec3f_div (TrVector3f vec, float scalar);

/* magnitude of vector */
float tr_vec3i_len (TrVector3i vec);
float tr_vec3f_len (TrVector3f vec);

/* normalize vector */
TrVector3f tr_vec3f_norm (TrVector3f vec);

/* dot product */
float tr_vec3i_dot (TrVector3i vec1, TrVector3i vec2);
float tr_vec3f_dot (TrVector3f vec1, TrVector3f vec2);

/* cross product */
TrVector3f tr_vec3f_cross (TrVector3f vec1, TrVector3f vec2);

/* calculate normal */
TrVector3f tr_vec3f_calc_normal (TrVector3f vert1, TrVector3f vert2, TrVector3f vert3);

#ifdef __cplusplus
}
#endif  //__cplusplus

#endif //__VECTOR3_H_INCLUDED
