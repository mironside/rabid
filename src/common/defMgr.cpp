/**
  @file defMgr.cpp
  @author Christopher Olsen
  @date Wed Dec 22 22:29:02 2004

  Copyright (C) 2004 Christopher A Olsen
*/
#include "common/defMgr.h"
#include <memory.h>
#include <stdio.h>
#include "common/common.h"
#include <string.h>
#include "common/def.h"
#include "common/global.h"



DefMgr::~DefMgr()
{
  snVector<const char*> keys = defs.GetKeys();
  for(unsigned int i = 0; i < keys.Size(); i++)
    delete defs[keys[i]];
}


IDef* DefMgr::GetDef(const char* name)
{
  if(!name || !name[0])
    return 0;

  char defName[256];
  strcpy(defName, name);

  if(!strstr(name, ":"))
  {
    int start = 0;
    int end = strlen(name);
    for(int i = strlen(name); i >= 0; i--)
    {
      if(name[i] == '.')
      {
        end = i;
        for(int j = i; j >= 0; j--)
        {
          if(name[j] == '/')
            break;
          start = j;
        }
      }
    }
    memset(defName, 0, 256);
    sprintf(defName, "%s:", name);
    strncat(defName, &name[start], end-start);
  }


  // search cached object defs
  if(defs.Exists(defName))
    return (IDef*)defs[defName];

  // parse object def file, create new def
  Def* def = new Def();
  if(!def->Create(defName))
  {
    delete def;
    return 0;
  }

  defs[defName] = def;

  return (IDef*)def;
}



int DefMgr::GetNumObjects()
{
  return 0;
}



void DefMgr::GetName(const char* file, const char* objName,
                     const char* ext, char* name)
{
  // coming from 'file' only (ext == 0)
  // if no extension ('ui/themes/grey/button')
  //   if !objName
  //     => 'ui/themes/grey/grey.ext:button'
  //   else
  //     => 'ui/themes/grey/button.ext:<objName>'
  // else ('ui/themes/grey/button.ext')
  //   if !objName
  //     => 'ui/themes/grey/grey.ext:grey'
  //   else
  //     => 'ui/themes/grey/grey.ext:<objName>'

  if(!strstr(file, ".") && !strstr(file, "/"))
  {
    sprintf(name, "%s.%s:%s", file, ext, file);
  }
  else if(!strstr(file, "."))
  {
    if(!objName)
    {
      int start = 0;
      int end = strlen(file);
      int found = 0;
      for(int i = strlen(file); i >= 0; i--)
      {
        if(file[i] == '/')
        {
          found = true;
          end = i;
          for(int j = i; j >= 0; j--)
          {
            if(file[j] == '/')
              break;
            start = j;
          }
        }
      }
      if(found)
      {
        memset(name, 0, 256);
        strncpy(name, file, end+1);
        strncat(name, &file[start], end-start);
        sprintf(name, "%s.%s:%s", name, ext, &file[end+1]);
      }
    }
    else
    {
      sprintf(name, "%s.%s:%s", file, ext, objName);
    }
  }
  else
  {
    if(!objName)
    {
      int start = 0;
      int end = 0;
      for(int i = strlen(file); i >= 0; i--)
      {
        if(file[i] == '.')
        {
          end = i-1;
          for(int j = i; j >= 0; j--)
          {
            if(file[j] == '/')
              break;
            start = j;
          }
        }
      }
      sprintf(name, "%s:", file);
      strncat(name, &file[start], end-start+1);
    }
    else
    {
      sprintf(name, "%s:%s", file, objName);
    }
  }
}
