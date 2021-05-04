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

#include "vectors/vector2.h"

/* equals */
inline bool tr_vec2i_equals (TrVector2i vec1, TrVector2i vec2)
{
    return vec1.x == vec2.x &&
           vec1.y == vec2.y; 
}

inline bool tr_vec2f_equals (TrVector2f vec1, TrVector2f vec2)
{
    return vec1.x == vec2.x &&
           vec1.y == vec2.y; 
}

/* math operations */
/* addition */
inline TrVector2i tr_vec2i_add (TrVector2i vec1, TrVector2i vec2)
{
    return (TrVector2i) { vec1.x + vec2.x, vec1.y + vec2.y };
}

inline TrVector2f tr_vec2f_add (TrVector2f vec1, TrVector2f vec2)
{
    return (TrVector2f) { vec1.x + vec2.x, vec1.y + vec2.y };
}

/* subtract */
inline TrVector2i tr_vec2i_sub (TrVector2i vec1, TrVector2i vec2)
{
    return (TrVector2i) { vec1.x - vec2.x, vec1.y - vec2.y };
}

inline TrVector2f tr_vec2f_sub (TrVector2f vec1, TrVector2f vec2)
{
    return (TrVector2f) { vec1.x - vec2.x, vec1.y - vec2.y };
}

/* multiplication by scalar */
inline TrVector2i tr_vec2i_mul (TrVector2i vec, float scalar)
{
    return (TrVector2i) { vec.x * scalar, vec.y * scalar };
}

inline TrVector2f tr_vec2f_mul (TrVector2f vec, float scalar)
{
    return (TrVector2f) { vec.x * scalar, vec.y * scalar };
}

/* division by scalar */
TrVector2i tr_vec2i_div (TrVector2i vec, float scalar)
{
    if (scalar == 0.0f)
    {
        return TR_ZERO2i;
    }
    return tr_vec2i_mul (vec, 1.0f / scalar);
}

TrVector2f tr_vec2f_div (TrVector2f vec, float scalar)
{
    if (scalar == 0.0f)
    {
        return TR_ZERO2f;
    }
    return tr_vec2f_mul (vec, 1.0f / scalar);
}

/* magnitude of vector */
inline float tr_vec2i_len (TrVector2i vec)
{
    return sqrt (vec.x * vec.x + vec.y * vec.y);
}

inline float tr_vec2f_len (TrVector2f vec)
{
    return sqrt (vec.x * vec.x + vec.y * vec.y);
}

/* normalize vector */
TrVector2f tr_vec2f_norm (TrVector2f vec)
{
    return tr_vec2f_div (vec, tr_vec2f_len (vec));
}

/* dot product */
inline float tr_vec2i_dot (TrVector2i vec1, TrVector2i vec2)
{
    return sqrt (vec1.x * vec2.x + vec1.y * vec2.y);
}

inline float tr_vec2f_dot (TrVector2f vec1, TrVector2f vec2)
{
    return sqrt (vec1.x * vec2.x + vec1.y * vec2.y);
}
