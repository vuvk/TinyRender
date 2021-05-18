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

static double delta_time;
static int fps;

static unsigned int cur_time = 0, prevTime = 0;
static int accumulated_fps = 0, num_frames = 0;
static double framerate_count = 0;

double tr_update_time ()
{
	prevTime = cur_time;
	cur_time = SDL_GetTicks();
	//delta_time = (cur_time - prevTime) / 1000.0;
	delta_time = (cur_time - prevTime) * 0.001;
	int int_fps = (int)(1.0 / delta_time);
	framerate_count += delta_time;
	accumulated_fps += int_fps;
	++num_frames;

	//if (num_frames > 60)
	if (framerate_count >= 0.5)
    {
        if (num_frames != 0.0)
        {
            fps = accumulated_fps / num_frames;
        }
        else
        {   
            fps = accumulated_fps;
        }

        if (fps >= 1000 || fps < 0)
        {
            fps  = 1000;
        }

		num_frames = 0;
		accumulated_fps = 0;

		framerate_count -= 0.5;
	}

/*
	//Delay if we updated faster than maxFPS
	if (fps > MAX_FPS)
    {
        if (delta_time < maxTicks)
            SDL_Delay((int)(maxTicks - delta_time));
        else
            SDL_Delay(1);
    }
*/

    /*
    if (fps > MAX_FPS)
        SDL_Delay(1);
    */

	return delta_time;
}

double tr_get_delta_time ()
{
    return delta_time;
}

int tr_get_fps ()
{
    return fps;
}

