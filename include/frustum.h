/**

   Copyright 2021 Anton "Vuvk" Shcherbatykh (vuvk69@gmail.com)

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

#ifndef __FRUSTUM_H_INCLUDED
#define __FRUSTUM_H_INCLUDED

#include "types.h"
#include "vectors/vectors.h"

#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

void tr_frustum_update ();
bool tr_frustum_has_pointf (float x, float y, float z);
bool tr_frustum_has_point (TrVector3f point);
bool tr_frustum_has_spheref (float x, float y, float z, float radius);
bool tr_frustum_has_sphere (TrVector3f center, float radius);
bool tr_frustum_has_cubef (float x, float y, float z, float radius);
bool tr_frustum_has_cube (TrVector3f center, float radius);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__FRUSTUM_H_INCLUDED
