/**
  @file    matrix3.h
  @author  Chris Olsen
  @date    02-05-03

  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __MATRIX3__
#define __MATRIX3__

#include "math/vector3.h"
#include "math/quaternion.h"


/**
  @class Matrix3
  @brief Class for a 3x3 Rotation Matrix

  @remarks
    Catharsis uses column major matrices (IE: the columns form the
    vectors of the matrix).  This is compliant with OpenGL.  Unlike
    OpenGL however, X is Forward, Y is Up, and Z is Right.
  @par
    Matrices should be multiplied right to left.  This is not the
    default association and needs to be enforced by using parenthesis.

*/
class Matrix3f
{
private:
  float m_mat[3*3];


public:
  // Constructors
	Matrix3f();
  Matrix3f(const Matrix3f& mat);
  Matrix3f(const Quaternion& q);
  Matrix3f(float* fmat);



  // special
  void MakeIdentity();


  // Accessors
  operator const float* () const;
  operator float* ();
  const float* operator [] (int col) const;
  float* operator [] (int col);
  float operator () (int row, int col) const;
  float& operator () (int row, int col);
  void SetRow(int row, const Vector3f& rv);
  Vector3f GetRow(int row) const;
  void SetColumn(int col, const Vector3f& cv);
  Vector3f GetColumn(int col) const;


  Vector3f Forward() const;
  Vector3f Up() const;
  Vector3f Right() const;

  Matrix3f& operator = (const Matrix3f& m);


  // Comparison
  bool operator == (const Matrix3f& m) const;
  bool operator != (const Matrix3f& m) const;


  // Arithmetic Operations
  Matrix3f operator + (const Matrix3f& m) const;
  Matrix3f operator - (const Matrix3f& m) const;
  Matrix3f operator * (const Matrix3f& m) const;
  Matrix3f operator * (const float s) const;
  Matrix3f operator / (const float s) const;
  Matrix3f operator - () const;
  Matrix3f& operator += (const Matrix3f& m);
  Matrix3f& operator -= (const Matrix3f& m);
  Matrix3f& operator *= (const float s);
  Matrix3f& operator /= (const float s);
  Vector3f operator * (const Vector3f& v) const;


  // Matrix Operations
  Matrix3f Transpose();
  float Determinant();


  // Rotation
  void FromEulerXYZ(float x, float y, float z);
  void FromEulerXZY(float x, float y, float z);
  void FromEulerYXZ(float x, float y, float z);
  void FromEulerYZX(float x, float y, float z);
  void FromEulerZXY(float x, float y, float z);
  void FromEulerZYX(float x, float y, float z);
  void FromQuaternion(const Quaternion& q);


  // disk storage
  int DiskSize();
  unsigned char* Write(unsigned char* ptr);
  unsigned char* Read(unsigned char* ptr);
};

#endif

