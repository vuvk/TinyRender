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

#ifndef __VECTOR4_H_INCLUDED
#define __VECTOR4_H_INCLUDED

#include "types.h"
#include "vectors/vector3.h"

typedef struct _TrVector4i
{
    //union
    //{
    //    struct
    //    {
            union
            {
                struct _TrVector3i;
                TrVector3i v3;
            };
            int32 w;
    //    };

    //    int32 a4[4];
    //};
} TrVector4i;

typedef struct _TrVector4f
{
    //union
    //{
    //    struct
    //    {
            union
            {
                struct _TrVector3f;
                TrVector3f v3;
            };
            float w;
    //    };

    //   float a4[4];
    //};
} TrVector4f;

/* zero vectors */
static const TrVector4i TR_ZERO4i = { 0 };
static const TrVector4f TR_ZERO4f = { 0 };

#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

/* "constructors" */
TrVector4i tr_vec4i_new (int32 x, int32 y, int32 z, int32 w);
TrVector4i tr_vec4i_newv (int32* v);
TrVector4f tr_vec4f_new (float x, float y, float z, float w);
TrVector4f tr_vec4f_newv (float* v);

/* equals */
bool tr_vec4i_equals (TrVector4i vec1, TrVector4i vec2);
bool tr_vec4f_equals (TrVector4f vec1, TrVector4f vec2);

/* math operations */
/* addition */
TrVector4i tr_vec4i_add (TrVector4i vec1, TrVector4i vec2);
TrVector4f tr_vec4f_add (TrVector4f vec1, TrVector4f vec2);

/* subtract */
TrVector4i tr_vec4i_sub (TrVector4i vec1, TrVector4i vec2);
TrVector4f tr_vec4f_sub (TrVector4f vec1, TrVector4f vec2);

/* multiplication by scalar */
TrVector4i tr_vec4i_mul (TrVector4i vec, float scalar);
TrVector4f tr_vec4f_mul (TrVector4f vec, float scalar);

/* division by scalar */
TrVector4i tr_vec4i_div (TrVector4i vec, float scalar);
TrVector4f tr_vec4f_div (TrVector4f vec, float scalar);

/* magnitude of vector */
float tr_vec4i_len (TrVector4i vec);
float tr_vec4f_len (TrVector4f vec);

/* normalize vector */
TrVector4f tr_vec4f_norm (TrVector4f vec);

/* dot product */
float tr_vec4f_dot (TrVector4f vec1, TrVector4f vec2);

#ifdef __cplusplus
}
#endif  //__cplusplus

#endif //__VECTOR4_H_INCLUDED
