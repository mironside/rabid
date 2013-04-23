/**
  @file dict.h
  @author Christopher Olsen
  @date Wed Mar  9 13:48:08 2005

  Copyright (C) 2004 Christopher A Olsen

  A Dictionary that maps a string key to multiple data.
*/
#ifndef __SN_DICT__
#define __SN_DICT__

#include "hashMap.h"
#include "math/vector2.h"
#include "math/vector3.h"
#include "math/vector4.h"
#include <string.h>
class GameObject;
class snDict;

void *operator new(size_t s, const char *filename, unsigned long line);

// dictionary types
union type_u
{
  bool  b;
  int   i;
  float f;
  char* str;
  GameObject* go;
  snDict* dict;
  Vector2f* vec2;
  Vector3f* vec3;
  Vector4f* vec4;
  snVector<int>* intVec;
  snVector<float>* floatVec;
  snVector<const char*>* stringVec;
  snVector<Vector2f>* vec2Vec;
  snVector<Vector3f>* vec3Vec;
  snVector<Vector4f>* vec4Vec;  
};

enum type_e
{
  SNTYPE_BOOL,
  SNTYPE_INT,
  SNTYPE_FLOAT,
  SNTYPE_STR,
  SNTYPE_GO,
  SNTYPE_VEC2,
  SNTYPE_VEC3,
  SNTYPE_VEC4,
  SNTYPE_DICT,
  SNTYPE_INTVEC,
  SNTYPE_FLOATVEC,
  SNTYPE_STRINGVEC,
  SNTYPE_VEC2VEC,
  SNTYPE_VEC3VEC,
  SNTYPE_VEC4VEC
};

class snDictItem
{
public:
  snDictItem() { valid = false; }
  ~snDictItem() { Clear(); }


  void operator=(bool b)
  { Clear(); valid = true; val.b = b; type = SNTYPE_BOOL; }
  
  void operator=(int i)
  { Clear(); valid = true; val.i = i; type = SNTYPE_INT; }
  
  void operator=(float f)
  { Clear(); valid = true; val.f = f; type = SNTYPE_FLOAT; }
  
  void operator=(const char* str)
  { Clear(); valid = true; val.str = new char[strlen(str)+1];
    memset(val.str, 0, strlen(str)+1); strcpy(val.str, str);
    type = SNTYPE_STR; }
  
  void operator=(GameObject* go)
  { Clear(); valid = true; val.go = go; type = SNTYPE_GO; }

  void operator=(const Vector2f& vec)
  { Clear(); valid = true; val.vec2 = new Vector2f(vec); type = SNTYPE_VEC2; }

  void operator=(const Vector3f& vec)
  { Clear(); valid = true; val.vec3 = new Vector3f(vec); type = SNTYPE_VEC3; }

  void operator=(const Vector4f& vec)
  { Clear(); valid = true; val.vec4 = new Vector4f(vec); type = SNTYPE_VEC4; }

  void operator=(const snVector<int>& v)
  { Clear(); valid = true; val.intVec = new snVector<int>(v); type = SNTYPE_INTVEC; }

  void operator=(const snVector<float>& v)
  { Clear(); valid = true; val.floatVec = new snVector<float>(v); type = SNTYPE_FLOATVEC; }

  void operator=(const snVector<const char*>& v)
  { Clear(); valid = true; val.stringVec = new snVector<const char*>(v); type = SNTYPE_STRINGVEC; }

  void operator=(const snVector<Vector2f>& vv)
  { Clear(); valid = true; val.vec2Vec = new snVector<Vector2f>(vv); type = SNTYPE_VEC2VEC; }

  void operator=(const snVector<Vector3f>& vv)
  { Clear(); valid = true; val.vec3Vec = new snVector<Vector3f>(vv); type = SNTYPE_VEC3VEC; }

  void operator=(const snVector<Vector4f>& vv)
  { Clear(); valid = true; val.vec4Vec = new snVector<Vector4f>(vv); type = SNTYPE_VEC4VEC; }
  
  type_e GetType() const { return type; }
  type_u GetVal() const { return val; }

  bool IsType(type_e itemType) { return itemType == type; }

  bool GetBool()          { return val.b; }
  int GetInt()            { return val.i; }
  float GetFloat()        { return val.f; }
  const char* GetString() { return val.str; }
  const GameObject* GetGO()     { return val.go; }
  const Vector2f& GetVec2()      { return *val.vec2; }
  const Vector3f& GetVec3()      { return *val.vec3; }
  const Vector4f& GetVec4()      { return *val.vec4; }
  const snDict& GetDict()       { return *val.dict; }
  const snVector<int>& GetIntVec() { return *val.intVec; }
  const snVector<float>& GetFloatVec() { return *val.floatVec; }
  const snVector<const char*>& GetStringVec() { return *val.stringVec; }
  const snVector<Vector2f>& GetVec2Vec() { return *val.vec2Vec; }
  const snVector<Vector3f>& GetVec3Vec() { return *val.vec3Vec; }
  const snVector<Vector4f>& GetVec4Vec() { return *val.vec4Vec; }

  type_e type;
  type_u val;
  bool valid;

  void Clear()
  {
    if (!valid)
      return;
    if (GetType() == SNTYPE_STR)
      delete val.str;
    if (GetType() == SNTYPE_VEC2)
      delete val.vec2;
    if (GetType() == SNTYPE_VEC3)
      delete val.vec3;
    if (GetType() == SNTYPE_VEC4)
      delete val.vec4;
    if (GetType() == SNTYPE_DICT)
      delete val.dict;
    if (GetType() == SNTYPE_INTVEC)
      delete val.intVec;
    if (GetType() == SNTYPE_FLOATVEC)
      delete val.floatVec;
    if (GetType() == SNTYPE_STRINGVEC)
    {
      for(unsigned int i = 0; i < val.stringVec->Size(); i++)
        delete[] (*val.stringVec)[i];
      delete val.stringVec;
    }
    if (GetType() == SNTYPE_VEC2VEC)
      delete val.vec2Vec;
    if (GetType() == SNTYPE_VEC3VEC)
      delete val.vec3Vec;
    if (GetType() == SNTYPE_VEC4VEC)
      delete val.vec4Vec;

    valid = false;
  }
};

class snDict
{
protected:
  snSHashMap<snDictItem*> hashMap;

public:
  snDict() {}
  ~snDict()
  {
    snVector<const char*> keys = hashMap.GetKeys();
    for(unsigned int i = 0; i < keys.Size(); i++)
      delete hashMap[keys[i]];
  }
  bool Exists(const char*key) const { return hashMap.Exists(key); }

  bool ExistsBool(const char*key) const    { return Exists(key) && IsBool(key); }
  bool ExistsInt(const char*key) const     { return Exists(key) && IsInt(key); }
  bool ExistsFloat(const char*key) const   { return Exists(key) && IsFloat(key); }
  bool ExistsString(const char*key) const  { return Exists(key) && IsString(key); }
  bool ExistsGo(const char*key) const      { return Exists(key) && IsGo(key); }
  bool ExistsVec2(const char*key) const     { return Exists(key) && IsVec2(key); }
  bool ExistsVec3(const char*key) const     { return Exists(key) && IsVec3(key); }
  bool ExistsVec4(const char*key) const     { return Exists(key) && IsVec4(key); }
  bool ExistsDict(const char*key) const    { return Exists(key) && IsDict(key); }
  bool ExistsIntVec(const char* key) const { return Exists(key) && IsIntVec(key); }
  bool ExistsFloatVec(const char* key) const { return Exists(key) && IsFloatVec(key); }
  bool ExistsStringVec(const char* key) const { return Exists(key) && IsStringVec(key); }
  bool ExistsVec2Vec(const char* key) const { return Exists(key) && IsVec2Vec(key); }
  bool ExistsVec3Vec(const char* key) const { return Exists(key) && IsVec3Vec(key); }
  bool ExistsVec4Vec(const char* key) const { return Exists(key) && IsVec4Vec(key); }

  bool ExistsType(const char*key, type_e type) const
  { return Exists(key) && IsType(key, type); }

  bool IsBool(const char*key) const    { return hashMap[key]->IsType(SNTYPE_BOOL); }
  bool IsInt(const char*key) const     { return hashMap[key]->IsType(SNTYPE_INT); }
  bool IsFloat(const char*key) const   { return hashMap[key]->IsType(SNTYPE_FLOAT); }
  bool IsString(const char*key) const  { return hashMap[key]->IsType(SNTYPE_STR); }
  bool IsGo(const char*key) const      { return hashMap[key]->IsType(SNTYPE_GO); }
  bool IsVec2(const char*key) const     { return hashMap[key]->IsType(SNTYPE_VEC2); }
  bool IsVec3(const char*key) const     { return hashMap[key]->IsType(SNTYPE_VEC3); }
  bool IsVec4(const char*key) const     { return hashMap[key]->IsType(SNTYPE_VEC4); }
  bool IsDict(const char*key) const    { return hashMap[key]->IsType(SNTYPE_DICT); }
  bool IsIntVec(const char* key) const { return hashMap[key]->IsType(SNTYPE_INTVEC); }
  bool IsFloatVec(const char* key) const { return hashMap[key]->IsType(SNTYPE_FLOATVEC); }
  bool IsStringVec(const char* key) const { return hashMap[key]->IsType(SNTYPE_STRINGVEC); }
  bool IsVec2Vec(const char* key) const { return hashMap[key]->IsType(SNTYPE_VEC2VEC); }
  bool IsVec3Vec(const char* key) const { return hashMap[key]->IsType(SNTYPE_VEC3VEC); }
  bool IsVec4Vec(const char* key) const { return hashMap[key]->IsType(SNTYPE_VEC4VEC); }

  bool IsType(const char*key, type_e type) const { return hashMap[key]->IsType(type); }

  type_e GetType(const char*key) const { return hashMap[key]->type; }

  snDictItem& operator[](const char*key) const { return *hashMap[key]; }
  snDictItem& operator[](const char*key)
  {
    if (!hashMap.Exists(key)) {
      snDictItem* item = new snDictItem();
      hashMap[key] = item;
      return *item;
    }
    else
    {
      return *hashMap[key];
    }
  }

  const snVector<const char*>& GetKeys() const { return hashMap.GetKeys(); }

};

#endif
