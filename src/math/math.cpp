/**
  @file math.cpp
  @author Christopher Olsen
  @date Tue Oct 19 22:55:08 2004

  Copyright (C) 2004 Christopher A Olsen
*/
#include "math/math.h"
#include "common/global.h"


// smallest difference between two angles in [0,360]
float SmallestLoopDelta(float one, float two, float loopMax)
{
  if (two - one > loopMax / 2.0f) {
    two -= loopMax;
  }
  if (two - one < -loopMax / 2.0f) {
    two += loopMax;
  }
  return two - one;
}

// closest to 0
float SmallestMag(float one, float two)
{
  if (fabsf(one) < fabs(two))
    return one;
  return two;
}
float Clampf(float val, float min, float max)
{
  if (val < min)
    val = min;
  if (val > max)
    val = max;
  return val;
}
float FixDegrees(float deg) 
{
  while (deg < 0.0f)
    deg += 360.0f;
  while (deg > 360.0f)
    deg -= 360.0f;
  return deg;
}
float FixRadians(float rad)
{
  static const float TWOPI = M_PI * 2.0f;
  while (rad < 0.0f)
    rad += TWOPI;
  while (rad > TWOPI)
    rad -= TWOPI;
  return rad;
}
float DegreeToRadian(float deg)
{
  return deg * M_PI / 180.0f;
}
float RadianToDegree(float rad)
{
  return rad * 180.0f / M_PI;
}


void GetFrustumPlanes(Matrix4x4& projMatrix, Matrix4x4& viewMatrix,
                      planeEq_t* frustumPlanes)
{
  Matrix4x4 pvMatrix = projMatrix * viewMatrix;

  planeEq_t* plane;
  //double l=0;
  float l = 0.0f;
  float* mat = pvMatrix.GetData();

  // compute the frustal planes
  // left
  plane = &frustumPlanes[0];
	plane->N[0] = -( mat[3] + mat[0]);
	plane->N[1] = -( mat[7] + mat[4]);
	plane->N[2] = -(mat[11] + mat[8]);
	plane->D         = -(mat[15] + mat[12]);

  l = Length(plane->N);
  plane->N[0] /= l;
  plane->N[1] /= l;
  plane->N[2] /= l;
  plane->D /= l;

  

  // right
  plane = &frustumPlanes[1];
	plane->N[0] = -( mat[3] - mat[0]);
	plane->N[1] = -( mat[7] - mat[4]);
	plane->N[2] = -(mat[11] - mat[8]);
	plane->D         = -(mat[15] - mat[12]);

  l = Length(plane->N);
  plane->N[0] /= l;
  plane->N[1] /= l;
  plane->N[2] /= l;
  plane->D /= l;
  

  // top
  plane = &frustumPlanes[2];
	plane->N[0] = -( mat[3] - mat[1]);
	plane->N[1] = -( mat[7] - mat[5]);
	plane->N[2] = -(mat[11] - mat[9]);
	plane->D         = -(mat[15] - mat[13]);

  l = Length(plane->N);
  plane->N[0] /= l;
  plane->N[1] /= l;
  plane->N[2] /= l;
  plane->D /= l;
  

  // bottom
  plane = &frustumPlanes[3];
	plane->N[0] = -( mat[3] + mat[1]);
	plane->N[1] = -( mat[7] + mat[5]);
	plane->N[2] = -(mat[11] + mat[9]);
	plane->D         = -(mat[15] + mat[13]);

  l = Length(plane->N);
  plane->N[0] /= l;
  plane->N[1] /= l;
  plane->N[2] /= l;
  plane->D /= l;


  // back
  plane = &frustumPlanes[4];
	plane->N[0] = -( mat[3] - mat[2]);
	plane->N[1] = -( mat[7] - mat[6]);
	plane->N[2] = -(mat[11] - mat[10]);
	plane->D         = -(mat[15] - mat[14]);

  l = Length(plane->N);
  plane->N[0] /= l;
  plane->N[1] /= l;
  plane->N[2] /= l;
  plane->D /= l;



  // front
  plane = &frustumPlanes[5];
	plane->N[0] = -( mat[3] + mat[2]);
	plane->N[1] = -( mat[7] + mat[6]);
	plane->N[2] = -(mat[11] + mat[10]);
	plane->D         = -(mat[15] + mat[14]);  

  l = Length(plane->N);
  plane->N[0] /= l;
  plane->N[1] /= l;
  plane->N[2] /= l;
  plane->D /= l;
}

