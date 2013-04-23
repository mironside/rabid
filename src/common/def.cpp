/**
@file def.cpp
@author Christopher Olsen
@date Sun Jan 16 14:29:13 2005

Copyright (C) 2004 Christopher A Olsen
*/
#include "common/def.h"
#include "public/fileSystem.h"
#include "OL/ast.h"
#include "common/defMgr.h"
#include <stdlib.h>
#include <string.h>
#include "common/global.h"
#include "libsn/vector.h"

Def::Def() :
  parent(0), name(0), type(0), fullname(0)
{
}



Def::~Def()
{
  for(unsigned int i = 0; i < components.Size(); i++)
    delete components[i];

  delete[] name;
  delete[] type;
}



int Def::Create(const char* objName)
{
  fullname = new char[strlen(objName)+1];
  memset(fullname, 0, strlen(objName)+1);
  strcpy(fullname, objName);

  char filename[256];
  char* n = 0;
  strcpy(filename, objName);
  for(int i = strlen(filename); i >= 0; i--)
  {
    if(filename[i] == ':')
    {
      n = &filename[i+1];
      filename[i] = 0;
      break;
    }
  }
  if(!n)
    return 0;


  if(!g_fileSystem->Exists(filename))
  {
    printf("%s not found\n", filename);
    return 0;
  }


  FileHandle file = g_fileSystem->Open(filename);
  char* buffer = (char*)g_fileSystem->GetData(file);

  ol_link_t* root = olParse(buffer, g_fileSystem->GetSize(file));
  if(!root || root->type != OL_AST_DEF)
    return 0;

  for(ol_link_t* link = root; link; link = link->next)
  {
    if(link->type == OL_AST_DEF)
    {
      if(strcmp(link->u.def->name, n) == 0)
      {
        root = link;
        return Create(root->u.def, objName);
      }
    }
  }

  return 0;
}



int Def::Create(ol_def_t* root, const char* objName)
{
  fullname = new char[strlen(objName)+1];
  memset(fullname, 0, strlen(objName)+1);
  strcpy(fullname, objName);

  if(!root)
    return 0;

  if(root->name)
  {
    name = new char[strlen(root->name)+1];
    memset(name, 0, strlen(root->name)+1);
    strcpy(name, root->name);
  }
  type = new char[strlen(root->type)+1];
  memset(type, 0, strlen(root->type)+1);
  strcpy(type, root->type);

  for(ol_link_t* link = root->bodylist; link; link = link->next)
  {
    if(link->type == OL_AST_DEF)
    {
      Def* d = new Def();
      if(!d->Create(link->u.def, objName))
      {
        delete d;
        return 0;
      }
      components.PushBack(d);
    }
    else if(link->type == OL_AST_KEYVAL)
    {
      ol_keyval_t* kv = link->u.keyval;
      ol_val_t* val = link->u.keyval->value;

      switch(val->type)
      {
      case ol_val_s::OL_VAL_STRING:
        dict[kv->name] = val->u.val_string;
        break;
      case ol_val_s::OL_VAL_INT:
        dict[kv->name] = val->u.val_int;
        break;
      case ol_val_s::OL_VAL_FLOAT:
        dict[kv->name] = val->u.val_float;
        break;

      case ol_val_s::OL_VAL_VEC2:
        dict[kv->name] = Vector2f(val->u.val_vec[0],
          val->u.val_vec[1]);
        break;

      case ol_val_s::OL_VAL_VEC3:
        dict[kv->name] = Vector3f(val->u.val_vec[0],
          val->u.val_vec[1],
          val->u.val_vec[2]);
        break;

      case ol_val_s::OL_VAL_VEC4:
        dict[kv->name] = Vector4f(val->u.val_vec[0],
          val->u.val_vec[1],
          val->u.val_vec[2],
          val->u.val_vec[3]);
        break;

      case ol_val_s::OL_VAL_LIST:
        {
          ol_link_t* link = val->u.val_list;
          if(link->type == OL_AST_VALUE)
          {
            ol_val_t* v = link->u.value;
            switch(v->type)
            {
            case ol_val_s::OL_VAL_INT:
              {
                snVector<int> iVec;
                for(ol_link_t* l = link; l; l = l->next)
                {
                  ol_val_t* v = l->u.value;
                  if(v->type == ol_val_s::OL_VAL_INT)
                    iVec.PushBack(v->u.val_int);
                }
                dict[kv->name] = iVec;
              }
              break;

            case ol_val_s::OL_VAL_BOOL:
              {
                snVector<int> iVec;
                for(ol_link_t* l = link; l; l = l->next)
                {
                  ol_val_t* v = l->u.value;
                  if(v->type == ol_val_s::OL_VAL_BOOL)
                    iVec.PushBack((v->u.val_bool != 0));
                }
                dict[kv->name] = iVec;
              }
              break;

            case ol_val_s::OL_VAL_FLOAT:
              {
                snVector<float> fVec;
                for(ol_link_t* l = link; l; l = l->next)
                {
                  ol_val_t* v = l->u.value;
                  if(v->type == ol_val_s::OL_VAL_FLOAT)
                    fVec.PushBack(v->u.val_float);
                }
                dict[kv->name] = fVec;
              }
              break;

            case ol_val_s::OL_VAL_STRING:
              {
                snVector<const char*> sVec;
                for(ol_link_t* l = link; l; l = l->next)
                {
                  ol_val_t* v = l->u.value;
                  if(v->type == ol_val_s::OL_VAL_STRING)
                    sVec.PushBack(strdup(v->u.val_string));
                }
                dict[kv->name] = sVec;
              }
              break;

            case ol_val_s::OL_VAL_VEC2:
              {
                snVector<Vector2f> vec2Vec;
                for(ol_link_t* l = link; l; l = l->next)
                {
                  ol_val_t* v = l->u.value;
                  if(v->type == ol_val_s::OL_VAL_VEC2)
                    vec2Vec.PushBack(Vector2f(v->u.val_vec[0], v->u.val_vec[1]));
                }
                dict[kv->name] = vec2Vec;
              }
              break;

            case ol_val_s::OL_VAL_VEC3:
              {
                snVector<Vector3f> vec3Vec;
                for(ol_link_t* l = link; l; l = l->next)
                {
                  ol_val_t* v = l->u.value;
                  if(v->type == ol_val_s::OL_VAL_VEC3)
                    vec3Vec.PushBack(Vector3f(v->u.val_vec[0],
                                              v->u.val_vec[1],
                                              v->u.val_vec[2]));
                }
                dict[kv->name] = vec3Vec;
              }
              break;

            case ol_val_s::OL_VAL_VEC4:
              {
                snVector<Vector4f> vec4Vec;
                for(ol_link_t* l = link; l; l = l->next)
                {
                  ol_val_t* v = l->u.value;
                  if(v->type == ol_val_s::OL_VAL_VEC4)
                    vec4Vec.PushBack(Vector4f(v->u.val_vec[0],
                                              v->u.val_vec[1],
                                              v->u.val_vec[2],
                                              v->u.val_vec[3]));
                }
                dict[kv->name] = vec4Vec;
              }
              break;

            default:
              break;
            }
          }
        }
        break;
      case ol_val_s::OL_VAL_BOOL:
        dict[kv->name] = (bool)val->u.val_bool;
        break;

      default:
        break;
      }
    }
  }

  return 1;
}



int Def::GetInt(const char* keyname) const
{
  if(dict.ExistsInt(keyname))
    return dict[keyname].GetInt();
  return 0;
}


float Def::GetFloat(const char* keyname) const
{
  if(dict.ExistsFloat(keyname))
    return dict[keyname].GetFloat();
  return 0;
}



const char* Def::GetString(const char* keyname) const
{
  if(dict.ExistsString(keyname))
    return dict[keyname].GetString();
  return 0;
}


bool Def::GetBool(const char* keyname) const
{
  if(dict.ExistsBool(keyname))
    return dict[keyname].GetBool();
  return 0;
}

const char* Def::GetName() const
{
  return name;
}


const char* Def::GetType() const
{
  return type;
}


const snVector<IDef*>& Def::GetComponents() const
{
  return components;
}


const snVector<int>& Def::GetIntVec(const char* keyname) const
{
  return dict[keyname].GetIntVec();
}


const snVector<float>& Def::GetFloatVec(const char* keyname) const
{
  return dict[keyname].GetFloatVec();
}

const snVector<const char*>& Def::GetStringVec(const char* keyname) const
{
  return dict[keyname].GetStringVec();
}


const snVector<int>& Def::GetBoolVec(const char* keyname) const
{
  return dict[keyname].GetIntVec();
}




const Vector2f& Def::GetVec2(const char* keyname) const
{
  return dict[keyname].GetVec2();
}

const Vector3f& Def::GetVec3(const char* keyname) const
{
  return dict[keyname].GetVec3();
}

const Vector4f& Def::GetVec4(const char* keyname) const
{
  return dict[keyname].GetVec4();
}



void Def::GetRelativePath(char* path, const char* file) const
{
  int end = 0;
  for(int i = strlen(fullname); i >= 0; i--)
  {
    end = i;
    if(fullname[i] == '/')
    {
      end = i+1;
      break;
    }
  }

  strncpy(path, fullname, end);
  path[end] = 0;
  if(file)
    strcat(path, file);
}

