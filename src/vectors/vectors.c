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

#include "vectors/vectors.h"

/* transformation from .. to .. */
TrVector2f tr_vec3f_to_vec2f (TrVector3f vec)
{
    float z = (vec.z != 0.0f) ? (1.0f / vec.z) : 0.0f;
    return (TrVector2f) {
        .x = vec.x * z,
        .y = vec.y * z
    };
}

inline TrVector3f tr_vec2f_to_vec3f (TrVector2f vec)
{
    return (TrVector3f) {
        .x = vec.x,
        .y = vec.y,
        .z = 1.0f
    };
}

TrVector3f tr_vec4f_to_vec3f (TrVector4f vec)
{
    float w = (vec.w != 0.0f) ? (1.0f / vec.w) : 0.0f;
    return (TrVector3f) {
        .x = vec.x * w,
        .y = vec.y * w,
        .z = vec.z * w
    };
}

inline TrVector4f tr_vec3f_to_vec4f (TrVector3f vec)
{
    return (TrVector4f) {
        .x = vec.x,
        .y = vec.y,
        .z = vec.z,
        .w = 1.0f
    };
}
