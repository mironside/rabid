// shaderProgramMgr.h
#ifndef __SHADER_PROGRAM_MGR__
#define __SHADER_PROGRAM_MGR__

#include "libsn/hashMap.h"

class ShaderProgram
{
protected:

public:
  ShaderProgram() { };
  virtual ~ShaderProgram() { };

  virtual void LoadVertexShader(const char* prog, int length) = 0;
  virtual void LoadFragmentShader(const char* prog, int length) = 0;
};

#endif
