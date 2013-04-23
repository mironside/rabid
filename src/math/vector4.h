/**
  @file vector2.h
  @author Chris Olsen
  @date 9-15-03

  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __VECTOR4__
#define __VECTOR4__

#include <math.h>

/**
  @class Vector3f
*/
class Vector4f
{
public:
	float x, y, z, w;

public:
  // Constructors
  Vector4f() : x(0), y(0), z(0), w(0) { }
  Vector4f(const float* v) : x(v[0]), y(v[1]), z(v[2]), w(v[3]) { }
  Vector4f(float _x, float _y, float _z, float _w) :
    x(_x), y(_y), z(_z), w(_w) { }
  Vector4f(const Vector4f& v) { *this = v; }

  // Accessors
  float& operator [] (int i)
  { return (&x)[i]; }
  float operator [] (int i) const
  { return (&x)[i]; }
  operator const float* () const
  { return &x; }
  operator float* ()
  { return &x; }


  // Comparison
  bool operator == (const Vector4f& v) const
  {
    return v.x == x &&
           v.y == y &&
           v.z == z &&
           v.w == w;
  }
  
  bool operator != (const Vector4f& v) const
  {
    return v.x != x &&
           v.y != y &&
           v.z != z &&
           v.w != w;
  }
  


  // Algebraic Operations
  Vector4f operator + (const Vector4f& v) const
  { return Vector4f(x + v.x, y + v.y, z + v.z, w + v.w); }
  Vector4f operator - (const Vector4f& v) const
  { return Vector4f(x - v.x, y - v.y, z - v.z, w - v.w); }
  Vector4f operator * (float s) const
  { return Vector4f(x * s, y * s, z * s, w * s); }
  Vector4f operator / (float s) const
  { return Vector4f(x / s, y / s, z / s, w / s); }
  Vector4f operator - ()
  { return Vector4f(-x, -y, -z, -w); }
  Vector4f operator += (const Vector4f& v)
  { x += v.x; y += v.y; z += v.z; w += v.w; return *this;}
  Vector4f operator -= (const Vector4f& v)
  { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this;}
  Vector4f operator *= (float s)
  { x *= s; y *= s; z *= s; w *= s; return *this;}
  Vector4f operator /= (float s)
  { x /= s; y /= s; z /= s; w /= s; return *this;}
};


inline float SquaredLength(const Vector4f& v)
{ return v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w; }

inline float Length(const Vector4f& v)
{ return sqrt(SquaredLength(v)); }

inline float Dot(const Vector4f& v1, const Vector4f& v2)
{ return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w; }

inline Vector4f Normalize(const Vector4f& v)
{ return v / Length(v); }

#endif

