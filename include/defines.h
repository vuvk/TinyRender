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

#ifndef __DEFINES_H_INCLUDED
#define __DEFINES_H_INCLUDED

/** ERROR CODES */
#define ERROR_ENGINE_NOT_CREATED     1
#define ERROR_ENGINE_NOT_STARTED     2
#define ERROR_WINDOW_NOT_CREATED     3
#define ERROR_RENDERER_NOT_CREATED   4
#define ERROR_GL_CONTEXT_NOT_CREATED 5

/** NAME OF DIRECTORY WITH RESOURCES */
#define BASEDIR "baseq2"

/** maximums */
#define MAX_NAME_LENGTH    64
#define MAX_PATH_LENGTH    256
#define MAX_STRING_LENGTH  1024

#define MAX_TEXTURE_WIDTH  1024
#define MAX_TEXTURE_HEIGHT 1024

#define EPSILON 0.01  // погрешность. Некое число, близкое к 0

#define FOV   60.0
#define FOV_2 30.0

/** ENABLE TESTS */
#ifdef _DEBUG
    #define _TEXTURE_DEBUG
    #define _MATERIAL_DEBUG

    #define _IMAGE_DEBUG
    #define _FONT_DEBUG
    #define _TEXT_DEBUG
#endif // DEBUG

#endif //__DEFINES_H_INCLUDED
