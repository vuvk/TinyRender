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

#ifndef __RENDER_H_INCLUDED
#define __RENDER_H_INCLUDED

#include "types.h"
#include "vectors/vectors.h"
#include "matrix.h"
#include "texture.h"
//#include "threadpool.h"
//#include "tinycthread.h"

#define PROJECTION_MATRIX 1
#define MODELVIEW_MATRIX  2

#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

bool trInit(const char* title, int width, int height, int bits);
void trClose();

void trClearColorb(uint8 r, uint8 g, uint8 b);
void trClearColori(uint32 color);
void trClear();

void trSwapBuffers();

/* MATRICES */
void trMatrixMode(int mode);
void trLoadIdentity();
void tr_get_matrix (int mode, TMatrix4x4 out);
bool trOrthof(const float left, const float right, const float bottom, const float top, const float zNear, const float zFar);
bool trFrustumf(const float left, const float right, const float bottom, const float top, const float zNear, const float zFar);
bool trPerspectivef(const float fovy, const float aspect, const float zNear, const float zFar);
void trLookAtf(const float eyeX, const float eyeY, const float eyeZ, const float centerX, const float centerY, const float centerZ, const float upX, const float upY, const float upZ);
void trLookAtv(const float* eye, const float* center, const float* up);

/* TRANSFORMATIONS */
void trRotatef(float angle, float x, float y, float z);
void trRotatefv(float angle, float* v);
void trRotateXf(float angle);
void trRotateYf(float angle);
void trRotateZf(float angle);

/* перемножает мировую матрицу и матрицу перспективную */
void trTransformsPrepare();

/* DRAWING */
void trPoint2i(int32 x, int32 y, uint32 color);
void trPoint2iv(int32* v, uint32 color);
void trPoint3f(float x, float y, float z, uint32 color);
void trPoint3fv(float* v, uint32 color);

void trLine2i(int32 x0, int32 y0, int32 x1, int32 y1, uint32 color);
void trLine2iv(int32* v0, int32* v1, uint32 color);
void trLineBicolor2i(int32 x0, int32 y0, int32 x1, int32 y1, uint32 color0, uint32 color1);
void trLineBicolor2iv(int32* v0, int32* v1, uint32 color0, uint32 color1);
void trLine3f(float x0, float y0, float z0, float x1, float y1, float z1, uint32 color);
void trLine3fv(float* v0, float* v1, uint32 color);
void trLineBicolor3f(float x0, float y0, float z0, float x1, float y1, float z1, uint32 color0, uint32 color1);
void trLineBicolor3fv(float* v0, float* v1, uint32 color0, uint32 color1);

void trRect(int32 x, int32 y, uint32 w, uint32 h, uint32 color);

// 2d
void trTriangleColor2i(int32 x0, int32 y0, int32 x1, int32 y1, int32 x2, int32 y2, uint32 color);
void trTriangleColor2iv(int32* v0, int32* v1, int32* v2, uint32 color);
void trTriangleTricolor2i(int32 x0, int32 y0, int32 x1, int32 y1, int32 x2, int32 y2, uint32 color0, uint32 color1, uint32 color2);
void trTriangleTricolor2iv(int32* v0, int32* v1, int32* v2, uint32 color0, uint32 color1, uint32 color2);
void trTriangleTexture2i(int32 x0, int32 y0, int32 x1, int32 y1, int32 x2, int32 y2,
                         float u0, float v0, float u1, float v1, float u2, float v2,
                         STexture* texture);
void trTriangleTexture2iv(int32*  v0, int32*  v1, int32*  v2,
                          float* uv0, float* uv1, float* uv2,
                          STexture* texture);
// 3d
void trTriangleColor3f(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, uint32 color);
void trTriangleColor3fv(float* v0, float* v1, float* v2, uint32 color);
void trTriangleTricolor3f(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, uint32 color0, uint32 color1, uint32 color2);
void trTriangleTricolor3fv(float* v0, float* v1, float* v2, uint32 color0, uint32 color1, uint32 color2);
void trTriangleTexture3f(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2,
                         float u0, float v0, float u1, float v1, float u2, float v2,
                         STexture* texture);
void trTriangleTexture3fv(float*  v0, float*  v1, float*  v2,
                          float* uv0, float* uv1, float* uv2,
                          STexture* texture);

// 2d
void trQuadColor2i(int32 x0, int32 y0, int32 x1, int32 y1, int32 x2, int32 y2, int32 x3, int32 y3, uint32 color);
void trQuadColor2iv(int32* v0, int32* v1, int32* v2, int32* v3, uint32 color);
void trQuadFourcolor2i(int32 x0, int32 y0, int32 x1, int32 y1, int32 x2, int32 y2, int32 x3, int32 y3, uint32 color0, uint32 color1, uint32 color2, uint32 color3);
void trQuadFourcolor2iv(int32* v0, int32* v1, int32* v2, int32* v3, uint32 color0, uint32 color1, uint32 color2, uint32 color3);
void trQuadTexture2i(int32 x0, int32 y0, int32 x1, int32 y1, int32 x2, int32 y2, int32 x3, int32 y3,
                     float u0, float v0, float u1, float v1, float u2, float v2, float u3, float v3,
                     STexture* texture);
void trQuadTexture2iv(int32*  v0, int32*  v1, int32*  v2, int32*  v3,
                      float* uv0, float* uv1, float* uv2, float* uv3,
                      STexture* texture);
// 3d
void trQuadColor3f(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, uint32 color);
void trQuadColor3fv(float* v0, float* v1, float* v2, float* v3, uint32 color);
void trQuadFourcolor3f(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, uint32 color0, uint32 color1, uint32 color2, uint32 color3);
void trQuadFourcolor3fv(float* v0, float* v1, float* v2, float* v3, uint32 color0, uint32 color1, uint32 color2, uint32 color3);
void trQuadTexture3f(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3,
                     float u0, float v0, float u1, float v1, float u2, float v2, float u3, float v3,
                     STexture* texture);
void trQuadTexture3fv(float*  v0, float*  v1, float*  v2, float*  v3,
                      float* uv0, float* uv1, float* uv2, float* uv3,
                      STexture* texture);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__RENDER_H_INCLUDED
