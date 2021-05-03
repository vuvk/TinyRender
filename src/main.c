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
#include <stdlib.h>

#include "types.h"
#include "utils.h"
#include "color.h"
#include "render.h"
#include "render_private.h"
#include "fps.h"

#include "SDL2/SDL.h"


#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define SCREEN_BITS   32


int main(int args, char** argv)
{
    trInit("tiny renderer", SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BITS);
    trClearColorb(0, 0, 0);
    trClear();

    trMatrixMode(PROJECTION_MATRIX);
    trLoadIdentity();
    trPerspectivef(60.0, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1, 100);

    bool running = true;
    SDL_Event evt;

    /*
    float x  = 0,   y  = 0,   z  = 0;
    float x2 = 0.1, y2 = 0,   z2 = 0;
    float x3 = 0.1, y3 = 0.1, z3 = 0;

    SVector3f f0 = {0, 0, 0};
    SVector3f f1 = {0, 0, 0};
    */

    STexture* texture0 = TextureCreate();
    TextureLoadFromFile(texture0, "images/texture0.bmp");

    float z = 1.5;
    float angle = 0;
    while (running)
    {
        //trClear();

        trMatrixMode(MODELVIEW_MATRIX);
        trLoadIdentity();
        trLookAtf(0, 0, z, 0, 0, 0, 0, 1, 0);

        //angle += 30 * deltaTime;
        if (angle >= 360) angle -= 360;
        trRotateXf(DEG_TO_RAD(angle));
        trRotateYf(DEG_TO_RAD(angle));
        trRotateZf(DEG_TO_RAD(angle));
        //trRotatef(angle, 0, 1, 0);

        trTransformsPrepare();


        // 2D рисование одноцветное

        trTriangleColor2i(10, 10, 790, 10, 10, 590, RGB_TO_COLOR(0, 255, 100));
        trTriangleColor2i(10, 590, 790, 590, 10, 10, RGB_TO_COLOR(255, 255, 100));
        trTriangleColor2i(10, 590, 790, 10, 790, 590, RGB_TO_COLOR(100, 100, 255));

        //trLine2i(100, 100, 300, 100, RGB_TO_COLOR(255, 10, 100));
        //trLine2i(300, 100, 150, 250, RGB_TO_COLOR(255, 10, 100));
        //trLine2i(100, 100, 150, 250, RGB_TO_COLOR(255, 10, 100));

        trQuadColor2i(500, 300, 700, 500, 600, 590, 400, 500, RGB_TO_COLOR(100, 100, 100));



        // 2D рисование многоцветное
        //trTriangleTricolor2i(10, 10, 790, 10, 10, 590, RGB_TO_COLOR(255, 0, 0), RGB_TO_COLOR(0, 255, 0), RGB_TO_COLOR(0, 0, 255));
        //trTriangleTricolor2i(10, 590, 790, 590, 10, 10, RGB_TO_COLOR(255, 0, 0), RGB_TO_COLOR(0, 255, 0), RGB_TO_COLOR(0, 0, 255));
        //trTriangleTricolor2i(10, 590, 790, 10, 790, 590, RGB_TO_COLOR(255, 0, 0), RGB_TO_COLOR(0, 255, 0), RGB_TO_COLOR(0, 0, 255));

        trTriangleTricolor2i(100, 100, 500, 100, 250, 350, RGB_TO_COLOR(255, 0, 0), RGB_TO_COLOR(0, 255, 0), RGB_TO_COLOR(0, 0, 255));

        //trLineBicolor2i(100, 100, 300, 100, RGB_TO_COLOR(255, 0, 0), RGB_TO_COLOR(0, 255, 0));
        //trLineBicolor2i(300, 100, 150, 250, RGB_TO_COLOR(255, 0, 0), RGB_TO_COLOR(0, 255, 0));
        //trLineBicolor2i(100, 100, 150, 250, RGB_TO_COLOR(255, 0, 0), RGB_TO_COLOR(0, 255, 0));

        trQuadFourcolor2i(500, 300, 700, 500, 600, 600, 400, 500, RGB_TO_COLOR(255, 0, 0), RGB_TO_COLOR(0, 255, 0), RGB_TO_COLOR(0, 0, 255), RGB_TO_COLOR(255, 0, 255));


        // textured
        /*
        trTriangleTexture2i(100, 100, 500, 100, 250, 350, 0.0, 0.0, 1.0, 0.0, 0.5, 0.5, texture0);
        trQuadTexture2i(200, 200,
                        500, 200,
                        500, 500,
                        200, 500,

                        0.0, 0.0,
                        1.0, 0.0,
                        1.0, 1.0,
                        0.0, 1.0, texture0);
        */

        // ------ 3D ------

        // 3D точки тест
        /*
        trPoint3f(x, y, z, RGB_TO_COLOR(255, 0, 255));
        trPoint3f(x2, y2, z2, RGB_TO_COLOR(255, 0, 255));
        */


        // кубик из линий
        /*
        trLine3f(-0.5, -0.5, -0.5,
                  0.5, -0.5, -0.5, RGB_TO_COLOR(255, 0, 0));
        trLine3f( 0.5, -0.5, -0.5,
                  0.5,  0.5, -0.5, RGB_TO_COLOR(255, 0, 0));
        trLine3f( 0.5,  0.5, -0.5,
                 -0.5,  0.5, -0.5, RGB_TO_COLOR(255, 0, 0));
        trLine3f(-0.5,  0.5, -0.5,
                 -0.5, -0.5, -0.5, RGB_TO_COLOR(255, 0, 0));

        trLine3f(-0.5, -0.5, 0.5,
                  0.5, -0.5, 0.5, RGB_TO_COLOR(255, 0, 255));
        trLine3f( 0.5, -0.5, 0.5,
                  0.5,  0.5, 0.5, RGB_TO_COLOR(255, 0, 255));
        trLine3f( 0.5,  0.5, 0.5,
                 -0.5,  0.5, 0.5, RGB_TO_COLOR(255, 0, 255));
        trLine3f(-0.5,  0.5, 0.5,
                 -0.5, -0.5, 0.5, RGB_TO_COLOR(255, 0, 255));

        trLine3f(-0.5, -0.5, -0.5,
                 -0.5, -0.5,  0.5, RGB_TO_COLOR(50, 255, 0));
        trLine3f( 0.5, -0.5, -0.5,
                  0.5, -0.5,  0.5, RGB_TO_COLOR(50, 255, 0));
        trLine3f(-0.5,  0.5, -0.5,
                 -0.5,  0.5,  0.5, RGB_TO_COLOR(50, 255, 0));
        trLine3f( 0.5,  0.5, -0.5,
                  0.5,  0.5,  0.5, RGB_TO_COLOR(50, 255, 0));
        */

        /*
        // one color
        trTriangleColor3f(-0.5,  0.5, 0.0,
                           0.0, -0.5, 0.0,
                           0.5,  0.5, 0.0,
                          RGB_TO_COLOR(255, 0, 0));

        trQuadColor3f(-0.5, -0.5, 0.0,
                       0.5, -0.5, 0.0,
                       0.5,  0.5, 0.0,
                      -0.5,  0.5, 0.0,
                      RGB_TO_COLOR(255, 0, 0));
        */


        // tricolor
/*
        trTriangleTricolor3f(-0.5,  0.5, 0.0,
                              0.0, -0.5, 0.0,
                              0.5,  0.5, 0.0,
                             RGB_TO_COLOR(255, 0, 0),
                             RGB_TO_COLOR(0, 255, 0),
                             RGB_TO_COLOR(0, 0, 255));
        trQuadFourcolor3f(-0.5, -0.5, 0.0,
                           0.5, -0.5, 0.0,
                           0.5,  0.5, 0.0,
                          -0.5,  0.5, 0.0,
                          RGB_TO_COLOR(255, 0, 0),
                          RGB_TO_COLOR(0, 255, 0),
                          RGB_TO_COLOR(0, 0, 255),
                          RGB_TO_COLOR(255, 0, 255));
*/


        // textured
        /*
        trTriangleTexture3f(-0.5,  0.5, 0.0,
                             0.0, -0.5, 0.0,
                             0.5,  0.5, 0.0,
                             0.0, 0.0,
                             0.5, 1.0,
                             1.0, 0.0,
                             texture0);
        */
        trQuadTexture3f(-0.5,  0.5, 0.0,
                         0.5,  0.5, 0.0,
                         0.5, -0.5, 0.0,
                        -0.5, -0.5, 0.0,
                         0.0, 0.0,
                         1.0, 0.0,
                         1.0, 1.0,
                         0.0, 1.0,
                         texture0);

        trSwapBuffers();

        //SDL_Delay(1);

        // check if the ESC key was pressed or the window was closed:
        while (SDL_PollEvent(&evt))
        {
            switch(evt.type)
            {
                case SDL_KEYDOWN:
                {
                    switch (evt.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            running = false;
                            break;

                        case SDLK_UP :
                            z -= 0.1;
                            break;

                        case SDLK_DOWN :
                            z += 0.1;
                            break;

                        case SDLK_LEFT :
                            angle -= 10;
                            break;

                        case SDLK_RIGHT :
                            angle += 10;
                            break;

                        default :
                            break;
                    }

                    break;
                }

                case SDL_QUIT:
                    running = false;
                    break;

                default :
                    break;
            }
        }

        CalcFPS();
        char buffer[50];
        sprintf(buffer, "tinyRender fps - %d", fps);
        SDL_SetWindowTitle(window, buffer);
    }

    trClose();

    return 0;
}

