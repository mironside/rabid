/**
  @file transform.cpp
  @author Christopher Olsen
  @date Mon Jan  3 23:21:45 2005

  Copyright (C) 2004 Christopher A Olsen
*/

#include "math/transformation.h"
#include "common/global.h"


void Transformation::MakeIdentity()
{
  m_rotate.MakeIdentity();
  m_translate[0] = m_translate[1] = m_translate[2] = 0;
  m_scale[0] =   m_scale[1] =   m_scale[2] = 0;
}



/// @todo account for scale
Transformation Transformation::operator * (const Transformation& rhs)
{
  Transformation transform;
  Vector3f t = rhs.GetTranslate();

  // calculate translation
  for(int i = 0; i < 3; i++)
    transform.m_translate[i] = Dot(t, (*this).GetRotate().GetRow(i)) + (*this).GetTranslate()[i];

  // calculate rotation
  transform.m_rotate = (*this).GetRotate() * rhs.GetRotate();
  return transform;
}



int Transformation::DiskSize()
{
  return m_rotate.DiskSize() + m_translate.DiskSize();
}


unsigned char* Transformation::Write(unsigned char* ptr)
{
  ptr = m_rotate.Write(ptr);
  ptr = m_translate.Write(ptr);

  return ptr;
}



unsigned char* Transformation::Read(unsigned char* ptr)
{
  ptr = m_rotate.Read(ptr);
  ptr = m_translate.Read(ptr);
  //  m_scale.Read(ptr);
  return ptr;
}


Transformation Transformation::Inverse()
{
  Transformation t;
  t.Rotate() = m_rotate.Transpose();
  t.Translate()[0] = -m_translate[0] * m_rotate(0, 0) +
                     -m_translate[1] * m_rotate(1, 0) +
                     -m_translate[2] * m_rotate(2, 0);
  
  t.Translate()[1] = -m_translate[0] * m_rotate(0, 1) +
                     -m_translate[1] * m_rotate(1, 1) +
                     -m_translate[2] * m_rotate(2, 1);
  
  t.Translate()[2] = -m_translate[0] * m_rotate(0, 2) +
                     -m_translate[1] * m_rotate(1, 2) +
                     -m_translate[2] * m_rotate(2, 2);

  return t;
}
