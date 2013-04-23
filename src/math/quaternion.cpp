/**
  @file    quaternion.cpp
  @author  Chris Olsen
  @date    02-07-03

  Copyright (C) 2004 Christopher A Olsen
*/
#include "math/quaternion.h"
#include <math.h>
#include <memory.h>
#include "common/global.h"


Quaternion::Quaternion() :
 x(0), y(0), z(0), w(1)
{
}



Quaternion::Quaternion(const Quaternion& rhs)
{
  *this = rhs;
}



Quaternion::Quaternion(float x, float y, float z, float w)
{
  this->x = x;
  this->y = y;
  this->z = z;
  this->w = w;
}


Quaternion::Quaternion(const float *angles)
{
	SetRotationRadians(angles);
}



Quaternion::~Quaternion()
{
}



/**
  Creates a quaternion based on a Euler rotation
  @param angles The euler representation of the rotation
*/
void Quaternion::SetRotationRadians(const float *angles)
{
	float angle;
	float sr, sp, sy, cr, cp, cy;

	angle = angles[2]*0.5f;
	sy = sin(angle);
	cy = cos(angle);
	angle = angles[1]*0.5f;
	sp = sin(angle);
	cp = cos(angle);
	angle = angles[0]*0.5f;
	sr = sin(angle);
	cr = cos(angle);

	float crcp = cr*cp;
	float srsp = sr*sp;

	x = (sr*cp*cy-cr*sp*sy);
	y = (cr*sp*cy+sr*cp*sy);
	z = (crcp*sy-srsp*cy);
	w = (crcp*cy+srsp*sy);
}


const Quaternion& Quaternion::operator = (const Quaternion& rhs)
{
  memcpy(&x, &rhs.x, 4 * sizeof(x));
  return *this;
}


/**
  Multiplication Operator
*/
Quaternion Quaternion::operator * (const Quaternion &rhs) const
{
	/*
		Quaternion Multiplication:
		q * q' = [w*w' - v.v',  vxv' + w*v + w'*v]
	*/

	Quaternion retQuat;

	// dot product of v
	float dotV = x * rhs.x +
				       y * rhs.y +
				       z * rhs.z;

	// cross product of v
	float crossV[3] = {  y * rhs.z - z * rhs.y,
                     -(x * rhs.z - z * rhs.x),
                       x * rhs.y - y * rhs.x};

	// Multiply Quaternions
	retQuat.x = crossV[0] + w * rhs.x + rhs.z * x;
	retQuat.y = crossV[1] + w * rhs.y + rhs.z * y;
	retQuat.z = crossV[2] + w * rhs.z + rhs.z * z;
	retQuat.w = w * rhs.w - dotV;

	return retQuat;
}


/**
  SLERP
*/
Quaternion Slerp(const Quaternion& from, const Quaternion& to, float t)
{
	float to1[4];
	float omega, cosom, sinom, scale0, scale1;

	// calc cosine
	cosom = from.x * to.x + from.y * to.y +
          from.z * to.z + from.w * to.w;

	// adjust signs (if necessary)
	if(cosom < 0.0)
	{
		cosom = -cosom; 
    to1[0] = - to.x;
		to1[1] = - to.y;
		to1[2] = - to.z;
		to1[3] = - to.w;
	}
	else
	{
		to1[0] = to.x;
		to1[1] = to.y;
		to1[2] = to.z;
		to1[3] = to.w;
	}


	// calculate coefficients
	if((1.0 - cosom) > 0.01f)
	{
		// standard case (slerp)
		omega = (float)acos(cosom);
		sinom = (float)sin(omega);
		scale0 = (float)sin((1.0 - t) * omega) / sinom;
		scale1 = (float)sin(t * omega) / sinom;
	}
	else
	{
		// "from" and "to" quaternions are very close
		// ... so we can do a linear interpolation
		scale0 = 1.0f - t;
		scale1 = t;
	}


	// calculate final values
	Quaternion quat;
  for(unsigned int i = 0; i < 4; i++)
  	(&quat.x)[i] = scale0 * (&from.x)[i] + scale1 * to1[i];

  return quat;
}


Quaternion NLerp(const Quaternion& from, const Quaternion& to, float t)
{
  Quaternion q;
  float mag = 0.0f;
  for(unsigned int i = 0; i < 4; i++)
  {
    (&q.x)[i] = (&from.x)[i] + ((&to.x)[i] - (&from.x)[i]) * t;
    mag += (&q.x)[i] * (&q.x)[i];
  }
  mag = 1.0f / sqrtf(mag);

  return Normalize(q);
}


Quaternion Normalize(const Quaternion& q)
{
  float mag = 1.0f / sqrtf(q.x * q.x + 
                           q.y * q.y + 
                           q.z * q.z + 
                           q.w * q.w);

  return Quaternion(q.x * mag, q.y * mag,
                    q.z * mag, q.w * mag);
}



///////////////////////////////////////////////////////////////////////////////
// S T O R A G E

int Quaternion::DiskSize()
{
  return sizeof(float) * 4;
}


unsigned char* Quaternion::Read(unsigned char* ptr)
{
  memcpy(&x, ptr, 4 * sizeof(x));
  ptr += sizeof(float) * 4;

  return ptr;
}


unsigned char* Quaternion::Write(unsigned char* ptr)
{
  memcpy(ptr, &x, 4 * sizeof(x));
  ptr += sizeof(float) * 4;

  return ptr;
}
