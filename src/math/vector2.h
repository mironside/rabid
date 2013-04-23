/**
  @file vector2.h
  @author Chris Olsen
  @date 9-15-03

  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __VECTOR2__
#define __VECTOR2__

#include <math.h>

/**
  @class Vector2f
*/
class Vector2f
{
public:
	float x, y;

public:
  // Constructors
  Vector2f() : x(0), y(0) { }
  Vector2f(const float* v) : x(v[0]), y(v[1]) { }
  Vector2f(float _x, float _y) : x(_x), y(_y) { }
  Vector2f(const Vector2f& v) : x(v.x), y(v.y)
  { }

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
  bool operator == (const Vector2f& v) const
  { return v.x == x && v.y == y; }
  bool operator != (const Vector2f& v) const
  { return v.x != x && v.y != y; }
  


  // Algebraic Operations
  Vector2f operator + (const Vector2f& v) const
  { return Vector2f(x + v.x, y + v.y); }
  Vector2f operator - (const Vector2f& v) const
  { return Vector2f(x - v.x, y - v.y); }
  Vector2f operator * (float s) const
  { return Vector2f(x * s, y * s); }
  Vector2f operator / (float s) const
  { return Vector2f(x / s, y / s); }
  Vector2f operator - ()
  { return Vector2f(-x, -y); }
  Vector2f operator += (const Vector2f& v)
  { x += v.x; y += v.y; return *this;}
  Vector2f operator -= (const Vector2f& v)
  { x -= v.x; y -= v.y; return *this;}
  Vector2f operator *= (float s)
  { x *= s; y *= s; return *this;}
  Vector2f operator /= (float s)
  { x /= s; y /= s; return *this;}
};


inline float SquaredLength(const Vector2f& v)
{ return v.x*v.x + v.y*v.y; }

inline float Length(const Vector2f& v)
{ return sqrt(SquaredLength(v)); }

inline float Dot(const Vector2f& v1, const Vector2f& v2)
{ return v1.x * v2.x + v1.y * v2.y; }

inline Vector2f Normalize(const Vector2f& v)
{ return v / Length(v); }

#endif

