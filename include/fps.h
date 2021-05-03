#ifndef __FPS_H_INCLUDED
#define __FPS_H_INCLUDED

#include "types.h"

#define MAX_FPS 60

extern double deltaTime;
extern int fps;

double CalcFPS();

#endif //__FPS_H_INCLUDED
