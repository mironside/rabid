/**
  @file surface.h
  @author Christopher Olsen
  @date Fri Apr 29 15:56:22 2005

  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __SURFACE__
#define __SURFACE__

#include "math/vector2.h"
#include "math/vector3.h"
#include "libsn/vector.h"

class Texture;
class Material;
class VertexBuffer;

class Light
{
public:
  Vector3f p;
  Vector3f d;
  float    I;

public:
  Light(const Vector3f& _p, const Vector3f& _d, float _I) :
    p(_p), d(_d), I(_I)
  { }
};

enum
{
  S_NORMAL,
  S_LIGHT,
};

class Surface
{
private:
  int type;
  snVector<Vector3f> vertices;
  Vector3f T;
  Vector3f B;
  float width, height;

  VertexBuffer* vb;
  Material* material;
  Material* light;
  Material* blend;
  Material* rblend;
  Material* copy;
  Material* rcopy;

  Texture* lightmap;  // final lightmap texture
  Texture* accum;     // accumulation (should be high precision 32-bit float)
  Texture* resid;     // residual light

public:
  Vector3f c;
  Vector3f N;
  Surface();
  ~Surface();


  
  int Create(const char* material, const snVector<Vector3f>& vertices,
             const snVector<Vector2f>& texcoords);
  int CreateLight(const snVector<Vector3f>& vertices, const snVector<Vector2f>& texcoords,
                  float intensity);
  void Render(int lightmap=0);
  void ClearAccum();
  void CreateLightMap(Vector3f& lp, Vector3f& ld, snVector<Surface*>& surfaces);
  void AccumulateLight();
  void AccumulateResidualLight();
  void ClearResidualLight();
  void CopyAccumToLightMap();
  void CopyResidualToLightMap();

  void CreateLights(snVector<Light*>& lights);
  float GetPower();
  int GetNumPatches();

  /// Sets the view and projection matrices such that the resulting view
  /// frames the surface from the view of the given point.
  void Frame(Vector3f& p);


  int GetType() { return type; }
};

#endif

