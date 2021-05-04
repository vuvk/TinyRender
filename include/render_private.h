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

#ifndef __RENDER_PRIVATE_H_INCLUDED
#define __RENDER_PRIVATE_H_INCLUDED

#include "SDL2/SDL.h"

//#include "threadpool/threadpool.h"
//#include "tinycthread/tinycthread.h"

#include "matrix.h"

extern SDL_Window*   window;
/*
extern SDL_Renderer* renderer;
extern SDL_Texture*  screen;
*/
extern SDL_Surface*  frameBuffer;

extern int trWidth, trHeight, trBits, trBpp, trPitch;
extern int trHalfWidth, trHalfHeight;

extern float trNearPlane, trFarPlane;

extern uint32 clearColor;

extern TMatrix4x4 proj;
extern TMatrix4x4 modl;

#define THREAD 32
#define QUEUE  256
//extern threadpool_t* threadPool;
//extern mtx_t renderLock;

#endif //__RENDER_PRIVATE_H_INCLUDED
