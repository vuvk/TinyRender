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
        struct _TrVector2i anon;
        TrVector2i v2;
    };
    int32 z;
} TrVector3i;

typedef struct _TrVector3f
{
    union
    {
        struct _TrVector2f anon;
        TrVector2f v2;
    };
    float z;
} TrVector3f;

/* zero vectors */
static const TrVector3i TR_ZERO3i = { 0 };
static const TrVector3f TR_ZERO3f = { 0 };

/* right vector */
static const TrVector3f TR_RIGHT3f = { 1.0f, 0.0f, 0.0f };

/* up vector */
static const TrVector3f TR_UP3f = { 0.0f, 1.0f, 0.0f };

/* forward vector */
static const TrVector3f TR_FORWARD3f = { 0.0f, 0.0f, 1.0f };

#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

/* transformation from .. to .. */
//TrVector3f tr_vec2f_to_vec3f (TrVector2f vec);

#ifdef __cplusplus
}
#endif  //__cplusplus

#endif //__VECTOR3_H_INCLUDED
