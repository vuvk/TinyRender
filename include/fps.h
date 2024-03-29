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

#ifndef __FPS_H_INCLUDED
#define __FPS_H_INCLUDED

#include "types.h"

#define MAX_FPS 60

#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

double tr_update_time ();
double tr_get_delta_time ();
int tr_get_fps ();

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__FPS_H_INCLUDED
