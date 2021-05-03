#ifndef __RENDER_PRIVATE_H_INCLUDED
#define __RENDER_PRIVATE_H_INCLUDED

//#define USE_SDL1

#ifdef USE_SDL1
#include "SDL/SDL.h"
#else  // USE_SDL2
#include "SDL2/SDL.h"
#endif // USE_SDL1

//#include "threadpool/threadpool.h"
//#include "tinycthread/tinycthread.h"

#include "matrix.h"

#ifndef USE_SDL1
extern SDL_Window*   window;
#endif // USE_SDL1
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
