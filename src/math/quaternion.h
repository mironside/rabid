/**
  @file    quaternion.h
  @author  Chris Olsen
  @date    02-07-03
  Quaternion class

  Copyright (C) 2004 Christopher A Olsen
*/

#ifndef __QUATERNION__
#define __QUATERNION__


/**
  @class Quaternion
*/
class Quaternion
{
public:
	Quaternion();
  Quaternion(const Quaternion& rhs);
  Quaternion(float x, float y, float z, float w);
	Quaternion(const float *angles);
	~Quaternion();

  const Quaternion& operator = (const Quaternion& rhs);
  Quaternion operator * (const Quaternion &rhs) const;
  void SetRotationRadians(const float* angles);



  float x, y, z, w;  /// Quaternion: x, y, z, w

  // Storage functions
  int DiskSize();
  unsigned char* Read(unsigned char* ptr);
  unsigned char* Write(unsigned char* ptr);
};

Quaternion Normalize(const Quaternion& q);

/// Accurate but slower method of interpolation.
/// Constant velocity, shortest distance, but not communitive.
Quaternion Slerp(const Quaternion& from, const Quaternion& to, float t);

/// Less accurate but much faster method (~2.5x) of interpolation.
/// Communitive, shortest distance, but non-constant velocity.
Quaternion NLerp(const Quaternion& from, const Quaternion& to, float t);

#endif
