/**
  @file vector3.h
  @author Chris Olsen
  @date 9-15-03

  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __VECTOR3__
#define __VECTOR3__

#include <math.h>
#include <memory.h>

/**
  @class Vector3f
*/
class Vector3f
{
public:
	float x, y, z;

public:
  // Constructors
  Vector3f() : x(0), y(0), z(0) { }
  Vector3f(const float* v) : x(v[0]), y(v[1]), z(v[2]) { }
  Vector3f(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { }
  Vector3f(const Vector3f& v) : x(v.x), y(v.y), z(v.z) { }

  // Accessors
  float& operator [] (int i)
  { return (&x)[i]; }
  float& operator [] (unsigned int i)
  { return (&x)[i]; }
  float operator [] (int i) const
  { return (&x)[i]; }
  operator const float* () const
  { return &x; }
  operator float* ()
  { return &x; }


  // Comparison
  bool operator == (const Vector3f& v) const
  { return v.x == x && v.y == y && v.z == z; }
  
  bool operator != (const Vector3f& v) const
  { return v.x != x && v.y != y && v.z != z; }


  // Algebraic Operations
  Vector3f operator + (const Vector3f& v) const
  { return Vector3f(x + v.x, y + v.y, z + v.z); }
  Vector3f operator - (const Vector3f& v) const
  { return Vector3f(x - v.x, y - v.y, z - v.z); }
  Vector3f operator * (float s) const
  { return Vector3f(x * s, y * s, z * s); }
  Vector3f operator / (float s) const
  { return Vector3f(x / s, y / s, z / s); }
  Vector3f operator - ()
  { return Vector3f(-x, -y, -z); }
  Vector3f operator += (const Vector3f& v)
  { x += v.x; y += v.y; z += v.z; return *this; }
  Vector3f operator -= (const Vector3f& v)
  { x -= v.x; y -= v.y; z -= v.z; return *this; }
  Vector3f operator *= (float s)
  { x *= s; y *= s; z *= s; return *this; }
  Vector3f operator /= (float s)
  { x /= s; y /= s; z /= s; return *this; }

  Vector3f Rotate(const Vector3f& r, float angle)
  {
    angle *=  3.1415972 / 180.0f;
    float c = cos(angle);
    float s = sin(angle);

    Vector3f v;
    //* untransposed
    v[0] = x * (c + (1-c)*r.x*r.x) + 
           y * ((1-c)*r.x*r.y - r.z*s) +
           z * ((1-c)*r.x*r.z + r.y*s);
    v[1] = x * ((1-c)*r.x*r.y + r.z*s) +
           y * (c + (1-c)*r.y*r.y) + 
           z * ((1-c)*r.y*r.z - r.x*s);
    v[2] = x * ((1-c)*r.x*r.z - r.y*s) + 
           y * ((1-c)*r.y*r.z + r.x*s) + 
           z * (c + (1-c)*r.z*r.z);
    //*/


    return v;
  }

  unsigned int DiskSize() const
  { return 3 * sizeof(float); }

  unsigned char* Write(unsigned char* ptr) const
  { memcpy(ptr, &x, 3*sizeof(float)); ptr += 3*sizeof(float); return ptr; }

  unsigned char* Read(unsigned char* ptr)
  { memcpy(&x, ptr, 3*sizeof(float)); ptr += 3*sizeof(float); return ptr; }

  
};



inline float SquaredLength(const Vector3f v)
{ return v.x*v.x + v.y*v.y + v.z*v.z; }

inline float Length(const Vector3f& v)
{ return sqrt(SquaredLength(v)); }

inline Vector3f Cross(const Vector3f& v1, const Vector3f& v2)
{
  return Vector3f(v1.y * v2.z - v1.z * v2.y,
                  v1.z * v2.x - v1.x * v2.z,
                  v1.x * v2.y - v1.y * v2.x);
}

inline float Dot(const Vector3f& v1, const Vector3f& v2)
{ return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }

inline Vector3f Normalize(const Vector3f& v)
{ return v / Length(v); }

#endif

