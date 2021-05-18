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

#ifndef __MATRIX_H_INCLUDED
#define __MATRIX_H_INCLUDED

#include "vectors/vector3.h"
#include "vectors/vector4.h"

typedef float TMatrix3x3[9];
typedef float TMatrix4x4[16];

#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

void LoadIdentity_M3x3(TMatrix3x3 matrix);

/* matrix 4x4 operations */
void LoadIdentity_M4x4(TMatrix4x4 matrix);

void Multiplyf_Scalar(TMatrix4x4 matrix, float scalar);
void Multiplyf_M4x4(TMatrix4x4 dst, const TMatrix4x4 src1, const TMatrix4x4 src2);
TrVector4f Multiplyf_M4x4_V4f(const TMatrix4x4 matrix, const TrVector4f vec);

void Translatef_M4x4(TMatrix4x4 matrix, float x, float y, float z);
void Translatev_M4x4(TMatrix4x4 matrix, TrVector3f translate);

void Scalef_M4x4(TMatrix4x4 matrix, float x, float y, float z);
void Scalev_M4x4(TMatrix4x4 matrix, TrVector3f scale);

void Rotatef_M4x4(TMatrix4x4 matrix, float angle, float x, float y, float z);
void RotateRxf_M4x4(TMatrix4x4 matrix, float angle);
void RotateRyf_M4x4(TMatrix4x4 matrix, float angle);
void RotateRzf_M4x4(TMatrix4x4 matrix, float angle);


/* "camera" */
bool Orthof_M4x4(TMatrix4x4 result, const float left, const float right, const float bottom, const float top, const float zNear, const float zFar);
bool Frustumf_M4x4(TMatrix4x4 result, const float left, const float right, const float bottom, const float top, const float zNear, const float zFar);
bool Perspectivef_M4x4(TMatrix4x4 result, const float fovy, const float aspect, const float zNear, const float zFar);
void LookAtf_M4x4(TMatrix4x4 result, const float eyeX, const float eyeY, const float eyeZ, const float centerX, const float centerY, const float centerZ, const float upX, const float upY, const float upZ);
void LookAtv_M4x4(TMatrix4x4 result, const TrVector3f eye, const TrVector3f center, const TrVector3f up);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__MATRIX_H_INCLUDED
