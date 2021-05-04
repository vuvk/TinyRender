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

#ifndef __VECTOR2_H_INCLUDED
#define __VECTOR2_H_INCLUDED

#include "types.h"

typedef struct _TrVector2i
{
	int32 x, y;
} TrVector2i;

typedef struct _TrVector2f
{
	float x, y;
} TrVector2f;

/* zero vectors */
static const TrVector2i TR_ZERO2i = { 0 };
static const TrVector2f TR_ZERO2f = { 0 };

#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

/* equals */
bool tr_vec2i_equals (TrVector2i, TrVector2i);
bool tr_vec2f_equals (TrVector2f, TrVector2f);

/* math operations */
/* addition */
TrVector2i tr_vec2i_add (TrVector2i vec1, TrVector2i vec2);
TrVector2f tr_vec2f_add (TrVector2f vec1, TrVector2f vec2);

/* subtract */
TrVector2i tr_vec2i_sub (TrVector2i vec1, TrVector2i vec2);
TrVector2f tr_vec2f_sub (TrVector2f vec1, TrVector2f vec2);

/* multiplication by scalar */
TrVector2i tr_vec2i_mul (TrVector2i vec, float scalar);
TrVector2f tr_vec2f_mul (TrVector2f vec, float scalar);

/* division by scalar */
TrVector2i tr_vec2i_div (TrVector2i vec, float scalar);
TrVector2f tr_vec2f_div (TrVector2f vec, float scalar);

/* magnitude of vector */
float tr_vec2i_len (TrVector2i vec);
float tr_vec2f_len (TrVector2f vec);

/* normalize vector */
TrVector2f tr_vec2f_norm (TrVector2f vec);

/* dot product */
float tr_vec2i_dot (TrVector2i vec1, TrVector2i vec2);
float tr_vec2f_dot (TrVector2f vec1, TrVector2f vec2);

#ifdef __cplusplus
}
#endif  //__cplusplus

#endif //__VECTOR2_H_INCLUDED
