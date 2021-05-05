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

#ifndef __VECTORS_H_INCLUDED
#define __VECTORS_H_INCLUDED

#include "vectors/vector2.h"
#include "vectors/vector3.h"
#include "vectors/vector4.h"

#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

/* transformation from .. to .. */
TrVector2f tr_vec3f_to_vec2f (TrVector3f vec);
TrVector3f tr_vec2f_to_vec3f (TrVector2f vec);
TrVector3f tr_vec4f_to_vec3f (TrVector4f vec);
TrVector4f tr_vec3f_to_vec4f (TrVector3f vec);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__VECTORS_H_INCLUDED
