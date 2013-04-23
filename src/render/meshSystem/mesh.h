/**
  @file mesh.h
  @author Christopher Olsen
  @date Wed Dec  8 01:48:10 2004


*/
#ifndef __MESH__
#define __MESH__

#include "public/mesh.h"
#include "math/transformation.h"

struct weight_s;
class VertexBuffer;
class IndexBuffer;



class Mesh : public IMesh
{
protected:
  VertexBuffer* vb;
  IndexBuffer*  ib;

  unsigned int nVertices;
  unsigned int nIndices;
  unsigned int format;
  unsigned int vertexSize;
  unsigned int vbSize;

  unsigned int vertexOffset;
  unsigned int texCoordOffset;
  unsigned int normalOffset;
  unsigned int tangentOffset;
  unsigned int binormalOffset;
  unsigned int colorOffset;


  unsigned int*  indexArray;
  unsigned char* vbdata;
  unsigned char* vbTransformed;

  unsigned int nWeights;
  weight_s* weights;

  int ReadIndexArray(unsigned char* ptr, unsigned char* end);
  int ReadVertexArray(unsigned char* ptr, unsigned char* end);
  int ReadWeightArray(unsigned char* ptr, unsigned char* end);


public:
  Mesh();
  virtual ~Mesh();

  int LoadFile(const char* filename);

//  void Create(unsigned int format, 
  void Draw();
  void DrawTangents();
  void DrawWeights(int bone);
  void ApplyTransformations(Transformation* joints, unsigned int nJoints);
  void SetVB(VertexBuffer* _vb);
  void SetIB(IndexBuffer* _ib);
};

#endif
