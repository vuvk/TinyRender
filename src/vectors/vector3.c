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

#include "vectors/vector3.h"

/* "constructors" */
inline TrVector3i tr_vec3i_new (int32 x, int32 y, int32 z)
{
    return (TrVector3i) { .v2 = tr_vec2i_new (x, y), .z = z };
    //return (TrVector3i) { .x = x, .y = y, .z = z };
}

inline TrVector3i tr_vec3i_newv (int32* v)
{
    return tr_vec3i_new (v[0], v[1], v[2]);
}

inline TrVector3f tr_vec3f_new (float x, float y, float z)
{
    return (TrVector3f) { .v2 = tr_vec2f_new (x, y), .z = z };
    //return (TrVector3f) { .x = x, .y = y, .z = z };
}

inline TrVector3f tr_vec3f_newv (float* v)
{
    return tr_vec3f_new (v[0], v[1], v[2]);
}

/* equals */
inline bool tr_vec3i_equals (TrVector3i vec1, TrVector3i vec2)
{
    return tr_vec2i_equals (vec1.v2, vec2.v2) &&
           vec1.z == vec2.z;
}

inline bool tr_vec3f_equals (TrVector3f vec1, TrVector3f vec2)
{
    return tr_vec2f_equals (vec1.v2, vec2.v2) &&
           vec1.z == vec2.z;
}

/* math operations */
/* addition */
inline TrVector3i tr_vec3i_add (TrVector3i vec1, TrVector3i vec2)
{
    return (TrVector3i) { 
        .v2 = tr_vec2i_add (vec1.v2, vec2.v2), 
        .z = vec1.z + vec2.z 
    };
}

inline TrVector3f tr_vec3f_add (TrVector3f vec1, TrVector3f vec2)
{
    return (TrVector3f) { 
        .v2 = tr_vec2f_add (vec1.v2, vec2.v2), 
        .z = vec1.z + vec2.z 
    };
}

/* subtract */
inline TrVector3i tr_vec3i_sub (TrVector3i vec1, TrVector3i vec2)
{
    return (TrVector3i) { 
        .v2 = tr_vec2i_sub (vec1.v2, vec2.v2), 
        .z = vec1.z - vec2.z 
    };
}

inline TrVector3f tr_vec3f_sub (TrVector3f vec1, TrVector3f vec2)
{
    return (TrVector3f) { 
        .v2 = tr_vec2f_sub (vec1.v2, vec2.v2), 
        .z = vec1.z - vec2.z 
    };
}

/* multiplication by scalar */
inline TrVector3i tr_vec3i_mul (TrVector3i vec, float scalar)
{
    return (TrVector3i) { 
        .v2 = tr_vec2i_mul (vec.v2, scalar),
        .z = vec.z * scalar 
    };
}

inline TrVector3f tr_vec3f_mul (TrVector3f vec, float scalar)
{
    return (TrVector3f) { 
        .v2 = tr_vec2f_mul (vec.v2, scalar),
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
inline float tr_vec3i_len (TrVector3i vec)
{
    return sqrt (vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

inline float tr_vec3f_len (TrVector3f vec)
{
    return sqrt (vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

/* normalize vector */
TrVector3f tr_vec3f_norm (TrVector3f vec)
{
    return tr_vec3f_div (vec, tr_vec3f_len (vec));
}

/* dot product */
inline float tr_vec3f_dot (TrVector3f vec1, TrVector3f vec2)
{
    return sqrt (vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z);
}

/* cross product */
inline TrVector3f tr_vec3f_cross (TrVector3f vec1, TrVector3f vec2)
{
    return tr_vec3f_new (
        vec1.y * vec2.z - vec1.z * vec2.y,
        vec1.z * vec2.x - vec1.x * vec2.z,
        vec1.x * vec2.y - vec1.y * vec2.x
    );
}

/* calculate normal */
TrVector3f tr_vec3f_calc_normal (TrVector3f vert1, TrVector3f vert2, TrVector3f vert3)
{
    TrVector3f q = tr_vec3f_sub (vert2, vert1);
    TrVector3f p = tr_vec3f_sub (vert3, vert1);

    return tr_vec3f_cross (q, p);
/*
    float Qx, Qy, Qz, Px, Py, Pz;
    TrVector3f v;

    Qx = vert2.x - vert1.x;
    Qy = vert2.y - vert1.y;
    Qz = vert2.z - vert1.z;

    Px = vert3.x - vert1.x;
    Py = vert3.y - vert1.y;
    Pz = vert3.z - vert1.z;

    v.x = Py * Qz - Pz * Qy;
    v.y = Pz * Qx - Px * Qz;
    v.z = Px * Qy - Py * Qx;

    return v;
*/
}
