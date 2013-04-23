/**
  @file    matrix4x4.cpp
  @author  Chris Olsen
  @date    02-05-03
  A simple Matrix class
  
  Copyright (C) 2004 Christopher A Olsen
*/
#include "math/matrix4x4.h"
#include "math/quaternion.h"
#include "math/vector3.h"
#include "math/vector4.h"
#include <math.h>
#include <memory.h>
#include "common/global.h"



#define PI 3.1415926535897932384626433832795



/**
  Initializes matrix to the identity on construction
*/
Matrix4x4::Matrix4x4()
{
	// Load identity on construction
	_mat[0]  = _mat[5]  = _mat[10] = _mat[15] = 1.0f;
	_mat[1]  = _mat[2]  = _mat[3]  = 0.0f;
	_mat[4]  = _mat[6]  = _mat[7]  = 0.0f;
	_mat[8]  = _mat[9]  = _mat[11] = 0.0f;
	_mat[12] = _mat[13] = _mat[14] = 0.0f;
}



/**
   Constructor takes matrix values in ROW-MAJOR order, so that when
   constructing a matrix the parameters passed in form the correct
   matrix (non-transposed).
 */
Matrix4x4::Matrix4x4(float m11, float m12, float m13, float m14,
                     float m21, float m22, float m23, float m24,
                     float m31, float m32, float m33, float m34,
                     float m41, float m42, float m43, float m44)
{
  _mat[0]  = m11;
  _mat[1]  = m21;
  _mat[2]  = m31;
  _mat[3]  = m41;

  _mat[4]  = m12;
  _mat[5]  = m22;
  _mat[6]  = m32;
  _mat[7]  = m42;

  _mat[8]  = m13;
  _mat[9]  = m23;
  _mat[10] = m33;
  _mat[11] = m43;

  _mat[12] = m14;
  _mat[13] = m24;
  _mat[14] = m34;
  _mat[15] = m44;
}



Matrix4x4::Matrix4x4(Vector3f& f, Vector3f& u,
                     Vector3f& r, Vector3f& p)
{
  // column major
  _mat[0]  = f[0];
  _mat[1]  = f[1];
  _mat[2]  = f[2];
  _mat[3]  = 0.0f;

  _mat[4]  = u[0];
  _mat[5]  = u[1];
  _mat[6]  = u[2];
  _mat[7]  = 0.0f;

  _mat[8]  = r[0];
  _mat[9]  = r[1];
  _mat[10] = r[2];
  _mat[11] = 0.0f;

  _mat[12] = p[0];
  _mat[13] = p[1];
  _mat[14] = p[2];
  _mat[15] = 1.0f;
}



/**
*/
Matrix4x4::~Matrix4x4()
{
}



/**
  operator *
  Transforms a point by this matrix.
*/
Vector3f Matrix4x4::operator * (Vector3f& p)
{
  return Vector3f(_mat[0] * p[0] + _mat[4] * p[1] + _mat[8] * p[2] + _mat[12],
                  _mat[1] * p[0] + _mat[5] * p[1] + _mat[9] * p[2] + _mat[13],
                  _mat[2] * p[0] + _mat[6] * p[1] + _mat[10] * p[2] + _mat[14]);
}

Vector4f Matrix4x4::operator * (Vector4f& p)
{
  return Vector4f(_mat[0] * p[0] + _mat[4] * p[1] + _mat[8] * p[2] + _mat[12] * p[3],
                  _mat[1] * p[0] + _mat[5] * p[1] + _mat[9] * p[2] + _mat[13] * p[3],
                  _mat[2] * p[0] + _mat[6] * p[1] + _mat[10] * p[2] + _mat[14] * p[3],
                  _mat[3] * p[0] + _mat[7] * p[1] + _mat[11] * p[2] + _mat[15] * p[3]);
}

/**
  operator *
  Matrix post-multiplication operator
  @param rhs The matrix to post multiply the matrix by
  @return A matrix containing the result of multiplying the two matrices
*/
Matrix4x4 Matrix4x4::operator * (const Matrix4x4& rhs)
{
  float tmp[16];
	// Post-Multiply Matrices
  // so that transformations go right to left

  tmp[0] = rhs._mat[0]  * _mat[0] +
           rhs._mat[4]  * _mat[1] +
           rhs._mat[8]  * _mat[2] +
           rhs._mat[12] * _mat[3];

  tmp[4] = rhs._mat[0]  * _mat[4] +
           rhs._mat[4]  * _mat[5] +
           rhs._mat[8]  * _mat[6] +
           rhs._mat[12] * _mat[7];

  tmp[8] = rhs._mat[0]  * _mat[8] +
           rhs._mat[4]  * _mat[9] +
           rhs._mat[8]  * _mat[10] +
           rhs._mat[12] * _mat[11];

  tmp[12]= rhs._mat[0]  * _mat[12] +
           rhs._mat[4]  * _mat[13] +
           rhs._mat[8]  * _mat[14] +
           rhs._mat[12] * _mat[15];


  tmp[1] = rhs._mat[1]  * _mat[0] +
           rhs._mat[5]  * _mat[1] +
           rhs._mat[9]  * _mat[2] +
           rhs._mat[13] * _mat[3];

  tmp[5] = rhs._mat[1]  * _mat[4] +
           rhs._mat[5]  * _mat[5] +
           rhs._mat[9]  * _mat[6] +
           rhs._mat[13] * _mat[7];

  tmp[9] = rhs._mat[1]  * _mat[8] +
           rhs._mat[5]  * _mat[9] +
           rhs._mat[9]  * _mat[10] +
           rhs._mat[13] * _mat[11];

  tmp[13]= rhs._mat[1]  * _mat[12] +
           rhs._mat[5]  * _mat[13] +
           rhs._mat[9]  * _mat[14] +
           rhs._mat[13] * _mat[15];


  tmp[2] = rhs._mat[2]  * _mat[0] +
           rhs._mat[6]  * _mat[1] +
           rhs._mat[10] * _mat[2] +
           rhs._mat[14] * _mat[3];

  tmp[6] = rhs._mat[2]  * _mat[4] +
           rhs._mat[6]  * _mat[5] +
           rhs._mat[10] * _mat[6] +
           rhs._mat[14] * _mat[7];

  tmp[10]= rhs._mat[2]  * _mat[8] +
           rhs._mat[6]  * _mat[9] +
           rhs._mat[10] * _mat[10] +
           rhs._mat[14] * _mat[11];

  tmp[14]= rhs._mat[2]  * _mat[12] +
           rhs._mat[6]  * _mat[13] +
           rhs._mat[10] * _mat[14] +
           rhs._mat[14] * _mat[15];


  tmp[3] = rhs._mat[3]  * _mat[0] +
           rhs._mat[7]  * _mat[1] +
           rhs._mat[11] * _mat[2] +
           rhs._mat[15] * _mat[3];

  tmp[7] = rhs._mat[3]  * _mat[4] +
           rhs._mat[7]  * _mat[5] +
           rhs._mat[11] * _mat[6] +
           rhs._mat[15] * _mat[7];

  tmp[11]= rhs._mat[3]  * _mat[8] +
           rhs._mat[7]  * _mat[9] +
           rhs._mat[11] * _mat[10] +
           rhs._mat[15] * _mat[11];

  tmp[15]= rhs._mat[3]  * _mat[12] +
           rhs._mat[7]  * _mat[13] +
           rhs._mat[11] * _mat[14] +
           rhs._mat[15] * _mat[15];

	return Matrix4x4(tmp[0], tmp[4],  tmp[8], tmp[12],
                   tmp[1], tmp[5],  tmp[9], tmp[13],
                   tmp[2], tmp[6], tmp[10], tmp[14],
                   tmp[3], tmp[7], tmp[11], tmp[15]);
}



void Matrix4x4::operator = (const Matrix4x4& rhs)
{
  for(int i = 0; i < 16; i++)
  {
    _mat[i] = rhs._mat[i];
  }
}



Matrix4x4 Matrix4x4::Transpose() const
{
  // transpose the matrix
  return Matrix4x4(_mat[0], _mat[1],  _mat[2], -_mat[12],
                   _mat[4], _mat[5],  _mat[6], -_mat[13],
                   _mat[8], _mat[9], _mat[10], -_mat[14],
                   0, 0, 0, 1);
}



void Matrix4x4::SetTranslation(float x, float y, float z)
{
  _mat[12] = x;
  _mat[13] = y;
  _mat[14] = z;
}



/**
   The matrices for each combination could be explicitly expanded
   instead of using matrix multiplication.
*/
void Matrix4x4::FromEuler(float x, float y, float z, order_t order)
{
  float xrad = x / 180.0f * 3.1415927f;
  float sx = sinf(xrad);
  float cx = cosf(xrad);
  Matrix4x4 xMat(1.0, 0.0, 0.0, 0.0,
                 0.0,  cx, -sx, 0.0,
                 0.0,  sx,  cx, 0.0,
                 0.0, 0.0, 0.0, 1.0);

  float yrad = y / 180.0f * 3.1415927f;
  float sy = sinf(yrad);
  float cy = cosf(yrad);
  Matrix4x4 yMat( cy, 0.0,  sy, 0.0,
                 0.0, 1.0, 0.0, 0.0,
                 -sy, 0.0,  cy, 0.0,
                 0.0, 0.0, 0.0, 1.0);

  float zrad = z / 180.0f * 3.1415927f;
  float sz = sinf(zrad);
  float cz = cosf(zrad);
  Matrix4x4 zMat( cz, -sz, 0.0, 0.0,
                  sz,  cz, 0.0, 0.0,
                 0.0, 0.0, 1.0, 0.0,
                 0.0, 0.0, 0.0, 1.0);

  switch(order)
  {
  case kXYZ:
    *this = zMat * (yMat * xMat);
    break;

  case kYXZ:
    *this = zMat * (xMat * yMat);
    break;

  case kYZX:
    *this = xMat * (zMat * yMat);
    break;

  case kXZY:
    *this = yMat * (zMat * xMat);
    break;

  case kZYX:
    *this = xMat * (yMat * zMat);
    break;

  case kZXY:
    *this = yMat * (xMat * zMat);
    break;

  default:
    *this = xMat * (yMat * zMat);
    break;
  }
}

