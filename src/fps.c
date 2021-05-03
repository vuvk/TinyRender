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

#include <stdio.h>

#include "SDL2/SDL.h"

#include "fps.h"

double deltaTime;
int fps;

static unsigned int curTime = 0, prevTime = 0;
static int accumulatedFPS = 0, numFrames = 0;
static double framerateCount = 0;

double CalcFPS ()
{
	prevTime = curTime;
	curTime = SDL_GetTicks();
	//deltaTime = (curTime - prevTime) / 1000.0;
	deltaTime = (curTime - prevTime) * 0.001;
	int intFPS = (int)(1.0 / deltaTime);
	framerateCount += deltaTime;
	accumulatedFPS += intFPS;
	++numFrames;

	//if (numFrames > 60)
	if (framerateCount >= 0.5)
    {
        if (numFrames != 0.0)
            fps = accumulatedFPS / numFrames;
        else
            fps = accumulatedFPS;

        if (fps >= 1000 || fps < 0)
            fps  = 1000;

		numFrames = 0;
		accumulatedFPS = 0;

		framerateCount -= 0.5;
	}

/*
	//Delay if we updated faster than maxFPS
	if (fps > MAX_FPS)
    {
        if (deltaTime < maxTicks)
            SDL_Delay((int)(maxTicks - deltaTime));
        else
            SDL_Delay(1);
    }
*/

    /*
    if (fps > MAX_FPS)
        SDL_Delay(1);
    */

	return deltaTime;
}


