/**
  @file mesh.h
  @author Christopher Olsen
  @date Wed Dec  8 01:48:10 2004


*/
#ifndef __IMESH__
#define __IMESH__

class Transformation;

class IMesh
{
public:
  virtual int LoadFile(const char* filename) = 0;
  virtual void Draw() = 0;
  virtual void DrawTangents() = 0;
  virtual void ApplyTransformations(Transformation* joints, unsigned int nJoints) = 0;
};

#endif

