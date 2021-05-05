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

#include <math.h>

#include "vectors/vector4.h"

/* equals */
inline bool tr_vec4i_equals (TrVector4i vec1, TrVector4i vec2)
{
    return tr_vec3i_equals (vec1.v3, vec2.v3) &&
           vec1.w == vec2.w;
}

inline bool tr_vec4f_equals (TrVector4f vec1, TrVector4f vec2)
{
    return tr_vec3f_equals (vec1.v3, vec2.v3) &&
           vec1.w == vec2.w;
}

/* math operations */
/* addition */
inline TrVector4i tr_vec4i_add (TrVector4i vec1, TrVector4i vec2)
{
    return (TrVector4i) { 
        .v3 = tr_vec3i_add (vec1.v3, vec2.v3), 
        .w = vec1.w + vec2.w 
    };
}

inline TrVector4f tr_vec4f_add (TrVector4f vec1, TrVector4f vec2)
{
    return (TrVector4f) { 
        .v3 = tr_vec3f_add (vec1.v3, vec2.v3), 
        .w = vec1.w + vec2.w 
    };
}

/* subtract */
inline TrVector4i tr_vec4i_sub (TrVector4i vec1, TrVector4i vec2)
{
    return (TrVector4i) { 
        .v3 = tr_vec3i_sub (vec1.v3, vec2.v3), 
        .w = vec1.w - vec2.w 
    };
}

inline TrVector4f tr_vec4f_sub (TrVector4f vec1, TrVector4f vec2)
{
    return (TrVector4f) { 
        .v3 = tr_vec3f_sub (vec1.v3, vec2.v3), 
        .w = vec1.w - vec2.w 
    };
}

/* multiplication by scalar */
inline TrVector4i tr_vec4i_mul (TrVector4i vec, float scalar)
{
    return (TrVector4i) { 
        .v3 = tr_vec3i_mul (vec.v3, scalar),
        .w = vec.w * scalar 
    };
}

inline TrVector4f tr_vec4f_mul (TrVector4f vec, float scalar)
{
    return (TrVector4f) { 
        .v3 = tr_vec3f_mul (vec.v3, scalar),
        .w = vec.w * scalar 
    };
}

/* division by scalar */
TrVector4i tr_vec4i_div (TrVector4i vec, float scalar)
{
    if (scalar == 0.0f)
    {
        return TR_ZERO4i;
    }
    return tr_vec4i_mul (vec, 1.0f / scalar);
}

TrVector4f tr_vec4f_div (TrVector4f vec, float scalar)
{
    if (scalar == 0.0f)
    {
        return TR_ZERO4f;
    }
    return tr_vec4f_mul (vec, 1.0f / scalar);
}

/* magnitude of vector */
inline float tr_vec4i_len (TrVector4i vec)
{
    return sqrt (
        vec.x * vec.x + 
        vec.y * vec.y + 
        vec.z * vec.z +
        vec.w * vec.w
    );
}

inline float tr_vec4f_len (TrVector4f vec)
{
    return sqrt (
        vec.x * vec.x + 
        vec.y * vec.y + 
        vec.z * vec.z +
        vec.w * vec.w
    );
}

/* normalize vector */
TrVector4f tr_vec4f_norm (TrVector4f vec)
{
    return tr_vec4f_div (vec, tr_vec4f_len (vec));
}

/* dot product */
inline float tr_vec4f_dot (TrVector4f vec1, TrVector4f vec2)
{
    return sqrt (
        vec1.x * vec2.x + 
        vec1.y * vec2.y + 
        vec1.z * vec2.z + 
        vec1.w * vec2.w
    );
}
