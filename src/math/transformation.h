/**
  @file transform.h
  @author Christopher Olsen
  @date Mon Jan  3 23:09:22 2005

  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __TRANSFORMATION__
#define __TRANSFORMATION__

#include "math/vector3.h"
#include "math/matrix3.h"

class Transformation
{
public:
  Transformation() : m_rotate(), m_translate(), m_scale(1,1,1)
    { m_rotate.MakeIdentity(); }
  Transformation(Quaternion& q) : m_rotate(q), m_translate(), m_scale(1,1,1)
    { }
  ~Transformation() {}

  void SetRotate(const Matrix3f& rotate)       { m_rotate = rotate; }
  void SetTranslate(const Vector3f& translate) { m_translate = translate; }
  void SetScale(const Vector3f& scale)         { m_scale = scale; }

  const Matrix3f& GetRotate() const    { return m_rotate; }
  const Vector3f& GetTranslate() const { return m_translate; }
  const Vector3f& GetScale() const     { return m_scale; }

  Matrix3f& Rotate()    { return m_rotate; }
  Vector3f& Translate() { return m_translate; }
  Vector3f& Scale()     { return m_scale; }

  Transformation Inverse();


  void MakeIdentity();
  Transformation operator * (const Transformation& rhs);

  Vector3f operator * (Vector3f& rhs)
  {
    Vector3f res;
    for(int i = 0; i < 3; i++)
      res[i] = Dot(rhs, m_rotate.GetRow(i)) + m_translate[i];
    return res;
  }


  /// @todo WOULD BE GREAT IF THIS EXISTED WITHOUT NEED FOR 16 FLOATS!
  operator float* ()
  {
    mat[0] = m_rotate(0,0);
    mat[1] = m_rotate(1,0);
    mat[2] = m_rotate(2,0);
    mat[3] = 0;

    mat[4] = m_rotate(0,1);
    mat[5] = m_rotate(1,1);
    mat[6] = m_rotate(2,1);
    mat[7] = 0;

    mat[8] = m_rotate(0,2);
    mat[9] = m_rotate(1,2);
    mat[10] = m_rotate(2,2);
    mat[11] = 0;

    mat[12] = m_translate[0];
    mat[13] = m_translate[1];
    mat[14] = m_translate[2];
    mat[15] = 1;

    return mat;
  }


  int DiskSize();
  unsigned char* Write(unsigned char* ptr);
  unsigned char* Read(unsigned char* ptr);
  
private:
  float mat[16];
  Matrix3f m_rotate;
  Vector3f m_translate;
  Vector3f m_scale;
};

#endif
