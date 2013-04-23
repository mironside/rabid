/**
  @file def.h
  @author Christopher Olsen
  @date Sun Jan 16 16:47:20 2005

  Copyright (C) 2004 Christopher A Olsen
*/
#ifndef __IDEF__
#define __IDEF__

#include "math/vector2.h"
#include "math/vector3.h"
#include "math/vector4.h"
#include "libsn/vector.h"
class snDict;

class IDef
{
public:
  virtual ~IDef() {};
  // keyvalues
  virtual const char* GetName() const = 0;
  virtual const char* GetType() const = 0;
  virtual void GetRelativePath(char* path, const char* file = 0) const = 0;
  virtual int GetInt(const char* keyname) const = 0;
  virtual bool GetBool(const char* keyname) const = 0;
  virtual float GetFloat(const char* keyname) const = 0;
  virtual const char* GetString(const char* keyname) const = 0;
  virtual const snDict& GetDict() const = 0;
  virtual const snVector<IDef*>& GetComponents() const = 0;
  virtual const snVector<int>& GetIntVec(const char* keyname) const = 0;
  virtual const snVector<int>& GetBoolVec(const char* keyname) const = 0;
  virtual const snVector<float>& GetFloatVec(const char* keyname) const = 0;
  virtual const snVector<const char*>& GetStringVec(const char* keyname) const = 0;
  virtual const Vector2f& GetVec2(const char* keyname) const = 0;
  virtual const Vector3f& GetVec3(const char* keyname) const = 0;
  virtual const Vector4f& GetVec4(const char* keyname) const = 0;
};

#endif

