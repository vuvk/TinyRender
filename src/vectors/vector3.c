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

#include "vectors/vector3.h"

/* equals */
inline bool tr_vec3i_equals (TrVector3i vec1, TrVector3i vec2)
{
    return vec1.x == vec2.x &&
           vec1.y == vec2.y &&
           vec1.z == vec2.z;
}

inline bool tr_vec3f_equals (TrVector3f vec1, TrVector3f vec2)
{
    return vec1.x == vec2.x &&
           vec1.y == vec2.y &&
           vec1.z == vec2.z;
}

/* math operations */
/* addition */
inline TrVector3i tr_vec3i_add (TrVector3i vec1, TrVector3i vec2)
{
    return (TrVector3i) { 
        .x = vec1.x + vec2.x, 
        .y = vec1.y + vec2.y, 
        .z = vec1.z + vec2.z 
    };
}

inline TrVector3f tr_vec3f_add (TrVector3f vec1, TrVector3f vec2)
{
    return (TrVector3f) { 
        .x = vec1.x + vec2.x, 
        .y = vec1.y + vec2.y, 
        .z = vec1.z + vec2.z 
    };
}

/* subtract */
inline TrVector3i tr_vec3i_sub (TrVector3i vec1, TrVector3i vec2)
{
    return (TrVector3i) { 
        .x = vec1.x - vec2.x, 
        .y = vec1.y - vec2.y, 
        .z = vec1.z - vec2.z 
    };
}

inline TrVector3f tr_vec3f_sub (TrVector3f vec1, TrVector3f vec2)
{
    return (TrVector3f) { 
        .x = vec1.x - vec2.x, 
        .y = vec1.y - vec2.y, 
        .z = vec1.z - vec2.z 
    };
}

/* multiplication by scalar */
inline TrVector3i tr_vec3i_mul (TrVector3i vec, float scalar)
{
    return (TrVector3i) { 
        .x = vec.x * scalar, 
        .y = vec.y * scalar, 
        .z = vec.z * scalar 
    };
}

inline TrVector3f tr_vec3f_mul (TrVector3f vec, float scalar)
{
    return (TrVector3f) { 
        .x = vec.x * scalar, 
        .y = vec.y * scalar, 
        .z = vec.z * scalar 
    };
}

/* division by scalar */
TrVector3i tr_vec3i_div (TrVector3i vec, float scalar)
{
    if (scalar == 0.0f)
    {
        return TR_ZERO3i;
    }
    return tr_vec3i_mul (vec, 1.0f / scalar);
}

TrVector3f tr_vec3f_div (TrVector3f vec, float scalar)
{
    if (scalar == 0.0f)
    {
        return TR_ZERO3f;
    }
    return tr_vec3f_mul (vec, 1.0f / scalar);
}

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
