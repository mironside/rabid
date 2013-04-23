/**
  @file def.h
  @author Christopher Olsen
  @date Sun Jan 16 14:25:46 2005

  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __DEF__
#define __DEF__

#include "public/def.h"
#include "OL/ast.h"
#include "math/vector2.h"
#include "math/vector3.h"
#include "math/vector4.h"
#include "libsn/vector.h"
#include "libsn/dict.h"


/// @todo Use "sections" instead of components when referring to defs,
///       change functions appropriately.
class Def : public IDef
{
private:
  char* fullname;
  char* name;
  char* type;
  Def* parent;
  snVector<IDef*> components;
  snDict dict;
  int Create(ol_def_t* def, const char* objName);

public:
  Def();
  virtual ~Def();

  const char* GetName() const;
  const char* GetType() const;

  int Create(const char* objectName);

  // keyvalues
  void GetRelativePath(char* path, const char* file = 0) const;
  int GetInt(const char* keyname) const;
  float GetFloat(const char* keyname) const ;
  const char* GetString(const char* keyname) const;
  bool GetBool(const char* keyname) const;
  const snVector<IDef*>& GetComponents() const;
  const snDict& GetDict() const { return dict; }
  const snVector<int>& GetIntVec(const char* keyname) const;
  const snVector<int>& GetBoolVec(const char* keyname) const;
  const snVector<float>& GetFloatVec(const char* keyname) const;
  const snVector<const char*>& GetStringVec(const char* keyname) const;
  const Vector2f& GetVec2(const char* keyname) const;
  const Vector3f& GetVec3(const char* keyname) const;
  const Vector4f& GetVec4(const char* keyname) const;
  const snVector<Vector2f*>& GetVec2Vec(const char* keyname) const;
  const snVector<Vector3f*>& GetVec3Vec(const char* keyname) const;
  const snVector<Vector4f*>& GetVec4Vec(const char* keyname) const;
};

#endif
