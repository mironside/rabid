// shaderProgramMgr.h
#ifndef __SHADER_PROGRAM_MGR__
#define __SHADER_PROGRAM_MGR__

#include "common/handle.h"

typedef struct shaderProgram_s
{
  char filename[32];
  int apiId;
} shaderProgram_t;

class ShaderProgramMgr
{
protected:
  HandleMgr<shaderProgram_t> m_shaderHandleMgr;
  int LoadShaderProgram(const char* filename, shaderProgram_t* shaderProgram);

public:
  ShaderProgramMgr();
  virtual ~ShaderProgramMgr();

  int GetShaderProgram(const char* filename);
  void BindShaderProgram(int shaderProgram);
  void ReloadShaderProgram(int shaderProgram);
};

#endif
