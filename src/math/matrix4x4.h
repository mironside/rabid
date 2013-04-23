/**
  @file    matrix4x4.h
  @author  Chris Olsen -- Vaporware Games
  @date    02-05-03
  A simple Matrix class

  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __MATRIX4x4__
#define __MATRIX4x4__

#include "math/vector3.h"
#include "math/vector4.h"
class Quaternion;


/**
  @class Matrix4x4 
  @brief Class for a 4x4 Matrix

  @remarks
    Catharsis uses column major matrices (IE: the columns form the
    vectors of the matrix).  This is compliant with OpenGL.  Unlike
    OpenGL however, X is Forward, Y is Up, and Z is Right.
  @par
    Matrices should be multiplied right to left.  This is not the
    default association and needs to be enforced by using parenthesis.

*/
class Matrix4x4
{
public:
  /// Initializes
	Matrix4x4();
	virtual ~Matrix4x4();


  /// Constructs a matrix from forward, up, right, and position
  /// vectors
  Matrix4x4(Vector3f& f, Vector3f& u, Vector3f& r, Vector3f& p);


  /// Constructs a matrix from 16 values.  Takes values in row major
  /// ordering so that when writing out the values is the proper
  /// format.
  Matrix4x4(float m11, float m21, float m31, float m41,
            float m12, float m22, float m32, float m42,
            float m13, float m23, float m33, float m43,
            float m14, float m24, float m34, float m44);


  /// Sets the translation vector of the matrix (does not out).
	void SetTranslation(float x, float y, float z);


  /// Possible matrix rotation order
  typedef enum {kXYZ, kYXZ, kYZX, kXZY, kZYX, kZXY} order_t;


  /// Uses euler angles and rotation order to set the matrix
  /// orientation.
  void FromEuler(float x, float y, float z, order_t order = kYZX);


  /// Matrix multiplication (should be ordered right to left, enforce
  /// with parentheses if necessary).
	Matrix4x4 operator * (const Matrix4x4& rhs);

  /// Point transformation
	Vector3f operator * (Vector3f& rhs);

    Vector4f operator * (Vector4f& rhs);

  /// Matrix assignment operator
  void operator = (const Matrix4x4& rhs);


  /// Transpose (Inverse in most cases).
  Matrix4x4 Transpose() const;

  inline float* GetData() { return _mat; }

protected:
	float _mat[16];
};

#endif

