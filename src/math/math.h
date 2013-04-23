/**
  @file math.h
  @author Christopher Olsen
  @date Thu Mar  4 17:22:18 2004

  Copyright (C) 2004 Christopher A Olsen
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
#ifndef __MATH__
#define __MATH__

#include "math/matrix4x4.h"
#include "math/vector3.h"
#include <math.h>


#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

typedef struct planeEq_s
{
  Vector3f N;
  float D;
} planeEq_t;
void GetFrustumPlanes(Matrix4x4& projMatrix, Matrix4x4& viewMatrix,
                      planeEq_t* frustumPlanes);

float SmallestLoopDelta(float one, float two, float loopMax);
float DegreeToRadian(float deg);
float RadianToDegree(float rad);
float FixDegrees(float deg);
float FixRadians(float rad);
float Clampf(float val, float min, float max);
float SmallestMag(float one, float two);

#endif

