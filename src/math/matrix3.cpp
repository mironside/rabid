// matrix3.cpp
#include "math/matrix3.h"
#include <memory.h>
#ifdef __SSE__
#include <xmmintrin.h>
#endif
#include "public/cvarSystem.h"
#include "common/global.h"

CVar m_sse("m_sse", "0");

Matrix3f::Matrix3f()
{ memset(m_mat, 0, sizeof(float) * 9); }
Matrix3f::Matrix3f(const Matrix3f& mat)
{ memcpy(m_mat, mat, sizeof(float) * 9); }
Matrix3f::Matrix3f(const Quaternion& q)
{ FromQuaternion(q); }
Matrix3f::Matrix3f(float* fmat)
{ memcpy(m_mat, fmat, sizeof(float) * 9); }



// special
void Matrix3f::MakeIdentity()
{
  for(int i = 0; i < 3; i++)
  {
    for(int j = 0; j < 3; j++)
    {
      if(i == j)
        (*this)(i,j) = 1;
      else
        (*this)(i,j) = 0;
    }
  }
}


// Accessors
Matrix3f::operator const float* () const
{ return m_mat; }

Matrix3f::operator float* ()
{ return m_mat; }

const float* Matrix3f::operator [] (int col) const
{
  if(col < 0)
    col = 0;
  if(col > 2)
    col = 2;

  return &m_mat[col<<2];
}

float* Matrix3f::operator [] (int col)
{
  if(col < 0)
    col = 0;
  if(col > 2)
    col = 2;

  return &m_mat[col * 3];
}

float Matrix3f::operator () (int row, int col) const
{ return m_mat[(col * 3) + row]; }

float& Matrix3f::operator () (int row, int col)
{ return m_mat[(col * 3) + row]; }


void Matrix3f::SetRow(int row, const Vector3f& rv)
{
  m_mat[row+0]  = rv.x;
  m_mat[row+3]  = rv.y;
  m_mat[row+6]  = rv.z;
}

Vector3f Matrix3f::GetRow(int row) const
{
  return Vector3f(m_mat[row+0],
    m_mat[row+3],
    m_mat[row+6]);
}

void Matrix3f::SetColumn(int col, const Vector3f& cv)
{
  m_mat[col*3 + 0] = cv.x;
  m_mat[col*3 + 1] = cv.y;
  m_mat[col*3 + 2] = cv.z;
}

Vector3f Matrix3f::GetColumn(int col) const
{
  return Vector3f(m_mat[col*3+0],
                  m_mat[col*3+1],
                  m_mat[col*3+2]);
}


Vector3f Matrix3f::Forward() const
{
  return GetColumn(0);
}

Vector3f Matrix3f::Up() const
{
  return GetColumn(1);
}

Vector3f Matrix3f::Right() const
{
  return GetColumn(2);
}



Matrix3f& Matrix3f::operator = (const Matrix3f& m)
{ memcpy(m_mat, m, sizeof(float) * 9); return (*this); }



// Comparison
bool Matrix3f::operator == (const Matrix3f& m) const
{
  for(int i = 0; i < 9; i++)
  {
    if(m_mat[i] != m.m_mat[i])
      return false;
  }

  return true;
}


bool Matrix3f::operator != (const Matrix3f& m) const
{
  for(int i = 0; i < 9; i++)
  {
    if(m_mat[i] == m.m_mat[i])
      return false;
  }

  return true;
}



// Arithmetic Operations
Matrix3f Matrix3f::operator + (const Matrix3f& m) const
{
  Matrix3f fmat;
  for(int i = 0; i < 3; i++)
    for(int j = 0; j < 3; j++)
      fmat(i,j) = (*this)(i,j) + m(i,j);

  return fmat;
}

Matrix3f Matrix3f::operator - (const Matrix3f& m) const
{
  Matrix3f fmat;
  for(int i = 0; i < 3; i++)
    for(int j = 0; j < 3; j++)
      fmat(i,j) = (*this)(i,j) - m(i,j);

  return fmat;
}

Matrix3f Matrix3f::operator * (const Matrix3f& m) const
{
  Matrix3f fmat;
  if(!m_sse.GetBool())
  {
    for(int i = 0; i < 3; i++)
    {
      for(int j = 0; j < 3; j++)
      {
        fmat(i,j) = Dot((*this).GetRow(i), m.GetColumn(j));
      }
    }
  }
  else
  {
#ifdef __SSE__
    Vector3f r;
    Vector3f c;
    __m128 v1, v2, f;

    for(int i = 0; i < 3; i++)
    {
      for(int j = 0; j < 3; j++)
      {
        r = (*this).GetRow(i);
        c = m.GetColumn(j);
        // SSE... OPTIMIZE THIS SHIT!
        v1 = _mm_set_ps(r.x, r.y, r.z, 0.0f);
        v2 = _mm_set_ps(c.x, c.y, c.z, 0.0f);
        f  = _mm_set_ps(0,0,0,0);
        f  = _mm_mul_ps(v1, v2);
        fmat(i,j) = f.m128_f32[3] + f.m128_f32[2] + f.m128_f32[1];
      }
    }
#endif
  }

  return fmat;
}

Matrix3f Matrix3f::operator * (const float s) const
{
  Matrix3f fmat;
  for(int i = 0; i < 3; i++)
    for(int j = 0; j < 3; j++)
      fmat(i,j) = (*this)(i,j) * s;

  return fmat;
}

Matrix3f Matrix3f::operator / (const float s) const
{
  Matrix3f fmat;
  for(int i = 0; i < 3; i++)
    for(int j = 0; j < 3; j++)
      fmat(i,j) = (*this)(i,j) / s;

  return fmat;
}

Matrix3f Matrix3f::operator - () const
{
  Matrix3f fmat;
  for(int i = 0; i < 3; i++)
    for(int j = 0; j < 3; j++)
      fmat(i,j) = -(*this)(i,j);

  return fmat;
}


Matrix3f& Matrix3f::operator += (const Matrix3f& m)
{
  for(int i = 0; i < 3; i++)
    for(int j = 0; j < 3; j++)
      (*this)(i,j) += m(i,j);

  return *this;
}

Matrix3f& Matrix3f::operator -= (const Matrix3f& m)
{
  for(int i = 0; i < 3; i++)
    for(int j = 0; j < 3; j++)
      (*this)(i,j) -= m(i,j);

  return *this;
}

Matrix3f& Matrix3f::operator *= (const float s)
{
  for(int i = 0; i < 3; i++)
    for(int j = 0; j < 3; j++)
      (*this)(i,j) *= s;

  return *this;
}

Matrix3f& Matrix3f::operator /= (const float s)
{
  for(int i = 0; i < 3; i++)
    for(int j = 0; j < 3; j++)
      (*this)(i,j) /= s;

  return *this;
}


Vector3f Matrix3f::operator * (const Vector3f& v) const
{
  Vector3f fv;

  for(int i = 0; i < 3; i++)
    fv[i] = Dot(v, (*this).GetRow(i));

  return fv;
}



// Matrix Operations
Matrix3f Matrix3f::Transpose()
{
  Matrix3f transpose;
  for(int i = 0; i < 3; i++)
    for(int j = 0; j < 3; j++)
      transpose(i,j) = (*this)(j,i);

  return transpose;
}

float Matrix3f::Determinant()
{
  Matrix3f& m = (*this);
  return m(0,0) * m(1,1) * m(2,2) +
    m(1,0) * m(2,1) * m(3,2) + 
    m(2,0) * m(3,1) * m(0,2) + 
    m(0,3) * m(1,2) * m(2,1) -
    m(1,3) * m(2,2) * m(3,1) - 
    m(2,3) * m(3,2) * m(0,1);
}


// Rotation
void Matrix3f::FromEulerXYZ(float x, float y, float z)
{
  // convert from degrees to radians
  float rads = 3.1415972f / 180.0f;
  x *= rads;
  y *= rads;
  z *= rads;

  float cx = cos(x);
  float sx = sin(x);
  float cy = cos(y);
  float sy = sin(y);
  float cz = cos(z);
  float sz = sin(z);


  m_mat[0] = cy*cz;
  m_mat[1] = cy*sz;
  m_mat[2] = -sy;
  m_mat[3] = sx*sy*cz+cx*-sz;
  m_mat[4] = sx*sy*sz+cx*cz;
  m_mat[5] = sx*cy;
  m_mat[6] = cx*sy*cz+-sx*-sz;
  m_mat[7] = cx*sy*sz+-sx*cz;
  m_mat[8] = cx*cy;
}


void Matrix3f::FromEulerXZY(float x, float y, float z)
{
  // convert from degrees to radians
  float rads = 3.1415972f / 180.0f;
  x *= rads;
  y *= rads;
  z *= rads;

  float cx = cos(x);
  float sx = sin(x);
  float cy = cos(y);
  float sy = sin(y);
  float cz = cos(z);
  float sz = sin(z);

  m_mat[0] = cz*cy;
  m_mat[1] = sz;
  m_mat[2] = cz*-sy;
  m_mat[3] = cx*-sz*cy+sx*sy;
  m_mat[4] = cx*cz;
  m_mat[5] = cx*-sz*-sy+sx*cy;
  m_mat[6] = -sx*-sz*cy+cx*sy;
  m_mat[7] = -sx*cz;
  m_mat[8] = -sx*-sz*-sy+cx*cy;
}

void Matrix3f::FromEulerYXZ(float x, float y, float z)
{
  // convert from degrees to radians
  float rads = 3.1415972f / 180.0f;
  x *= rads;
  y *= rads;
  z *= rads;

  float cx = cos(x);
  float sx = sin(x);
  float cy = cos(y);
  float sy = sin(y);
  float cz = cos(z);
  float sz = sin(z);

  m_mat[0] = cy*cz+-sy*-sx*-sz;
  m_mat[1] = cy*sz+-sy*-sx*cz;
  m_mat[2] = -sy*cx;
  m_mat[3] = cx*-sz;
  m_mat[4] = cx*cz;
  m_mat[5] = sx;
  m_mat[6] = sy*cz+cy*-sx*-sz;
  m_mat[7] = sy*sz+cy*-sx*cz;
  m_mat[8] = cy*cx;
}

void Matrix3f::FromEulerYZX(float x, float y, float z)
{
  // convert from degrees to radians
  float rads = 3.1415972f / 180.0f;
  x *= rads;
  y *= rads;
  z *= rads;

  float cx = cos(x);
  float sx = sin(x);
  float cy = cos(y);
  float sy = sin(y);
  float cz = cos(z);
  float sz = sin(z);


  m_mat[0] = cy*cz;
  m_mat[1] = cy*sz*cx+-sy*-sx;
  m_mat[2] = cy*sz*sx+-sy*cx;
  m_mat[3] = -sz;
  m_mat[4] = cz*cx;
  m_mat[5] = cz*sx;
  m_mat[6] = sy*cz;
  m_mat[7] = sy*sz*cx+cy*-sx;
  m_mat[8] = sy*sz*sx+cy*cx;
}

void Matrix3f::FromEulerZXY(float x, float y, float z)
{
  // convert from degrees to radians
  float rads = 3.1415972f / 180.0f;
  x *= rads;
  y *= rads;
  z *= rads;

  float cx = cos(x);
  float sx = sin(x);
  float cy = cos(y);
  float sy = sin(y);
  float cz = cos(z);
  float sz = sin(z);

  m_mat[0] = cz*cy+sz*sx*sy;
  m_mat[1] = sz*cx;
  m_mat[2] = cz*-sy+sz*sx*cy;
  m_mat[3] = -sz*cy+cz*sx*sy;
  m_mat[4] = cz*cx;
  m_mat[5] = -sz*-sy+cz*sx*cy;
  m_mat[6] = cx*sy;
  m_mat[7] = -sx;
  m_mat[8] = cx*cy;
}

void Matrix3f::FromEulerZYX(float x, float y, float z)
{
  // convert from degrees to radians
  float rads = 3.1415926535897932384626433832795f / 180.0f;
  x *= rads;
  y *= rads;
  z *= rads;

  float cx = cos(x);
  float sx = sin(x);
  float cy = cos(y);
  float sy = sin(y);
  float cz = cos(z);
  float sz = sin(z);

  m_mat[0] = cz*cy;
  m_mat[1] = sz*cx+cz*-sy*-sx;
  m_mat[2] = sz*sx+cz*-sy*cx;
  m_mat[3] = -sz*cy;
  m_mat[4] = cz*cx+-sz*-sy*-sx;
  m_mat[5] = cz*sx+-sz*-sy*cx;
  m_mat[6] = sy;
  m_mat[7] = cy*-sx;
  m_mat[8] = cy*cx;
}


void Matrix3f::FromQuaternion(const Quaternion& q)
{
  /*
  return Matrix4x4(1.0f-2.0f*(y*y+z*z),      2.0f*(x*y+w*z),      2.0f*(x*z-w*y), 0.0,
  2.0f*(x*y-w*z), 1.0f-2.0f*(x*x+z*z),      2.0f*(y*z+w*x), 0.0,
  2.0f*(x*z+w*y),      2.0f*(y*z-w*x), 1.0f-2.0f*(x*x+y*y), 0.0,
  */
  float xx = q.x * q.x;
  float yy = q.y * q.y;
  float zz = q.z * q.z;

  float xy = q.x * q.y;
  float xz = q.x * q.z;
  float yz = q.y * q.z;

  float wx = q.w * q.x;
  float wy = q.w * q.y;
  float wz = q.w * q.z;

  // return matrix
  m_mat[0] = 1.0f-2.0f*(yy+zz);
  m_mat[1] = 2.0f*(xy+wz);
  m_mat[2] = 2.0f*(xz-wy);

  m_mat[3] = 2.0f*(xy-wz);
  m_mat[4] = 1.0f-2.0f*(xx+zz);
  m_mat[5] = 2.0f*(yz+wx);

  m_mat[6] = 2.0f*(xz+wy);
  m_mat[7] = 2.0f*(yz-wx);
  m_mat[8] = 1.0f-2.0f*(xx+yy);
}


// disk storage
int Matrix3f::DiskSize()
{
  return sizeof(float) * 9;
}

unsigned char* Matrix3f::Write(unsigned char* ptr)
{
  memcpy(ptr, m_mat, sizeof(float)*9);
  ptr += sizeof(float) * 9;

  return ptr;
}

unsigned char* Matrix3f::Read(unsigned char* ptr)
{
  memcpy(m_mat, ptr, sizeof(float)*9);
  ptr += sizeof(float) * 9;

  return ptr;
}

