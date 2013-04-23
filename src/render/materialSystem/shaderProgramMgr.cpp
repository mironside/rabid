// shaderProgramMgr.cpp
#include "shaderProgramMgr.h"
#include "render/renderDevice.h"
#include "public/fileSystem.h"
#include "common/log.h"
#include <stdio.h>
#include <string.h>
#include "common/global.h"

extern IRenderDevice* g_renderDevice;



ShaderProgramMgr::ShaderProgramMgr()
{
}



ShaderProgramMgr::~ShaderProgramMgr()
{
}



int ShaderProgramMgr::GetShaderProgram(const char* filename)
{
  // find if it exists
  snVector<shaderProgram_t*> shaders = m_shaderHandleMgr.GetActiveList();
  for(unsigned int i = 0; i < shaders.Size(); i++)
  {
    if(strcmp(shaders[i]->filename, filename) == 0)
      return i;
  }

  // create
  shaderProgram_t* shader = 0;
  int handle = m_shaderHandleMgr.Create(&shader);

  if(!LoadShaderProgram(filename, shader))
  {
    LOG(LOG_RED, "ERROR: Shader program '%s' not found!", filename);
    m_shaderHandleMgr.Destroy(handle);
    return -1;
  }

  return handle;
}



int ShaderProgramMgr::LoadShaderProgram(const char* filename, shaderProgram_t* shaderProgram)
{
  FileHandle file = g_fileSystem->Open((char*)filename);
  if(file < 0)
    return 0;
  int length = g_fileSystem->GetSize(file);
  char* data = (char*)g_fileSystem->GetData(file);


  strcpy(shaderProgram->filename, filename);
  shaderProgram->apiId = g_renderDevice->CreateShaderProgram();

  char* glsl = strstr(data, "<GLSL>");
  char* hlsl = strstr(data, "<HLSL>");
  char* end  = &data[length];
  if(!glsl)
  {
    printf("*** RENDERER ERROR: \'%s\' does not have a GLSL implementation\n", filename);
    return 0;
  }

  // using OpenGL so 'end' the file at the start of the <HLSL> section (if it's below)
  if(hlsl > glsl)
  {
    end = hlsl;
  }

  char* vs = strstr(data, "[VertexShader]");
  char* fs = strstr(data, "[FragmentShader]");

  if(vs)
  {
    vs += strlen("[VertexShader]");
    int length = 0;
    if(fs)
      length = fs - vs;
    else
      length = end - vs;

    g_renderDevice->CreateVertexShader(shaderProgram->apiId, vs, length);
  }

  if(fs)
  {
    fs += strlen("[FragmentShader]");

    int length = end - fs;
    g_renderDevice->CreateFragmentShader(shaderProgram->apiId, fs, length);
  }

  g_fileSystem->Close(file);

  return 1;
}

