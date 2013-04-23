// GLShaderProgram.h
#ifndef __GL_SHADER_PROGRAM__
#define __GL_SHADER_PROGRAM__

#include "render/materialSystem/shaderProgram.h"

class GLShaderProgram : public ShaderProgram
{
protected:
  unsigned int glShaderProg;

public:
  GLShaderProgram();
  virtual ~GLShaderProgram();

  void LoadVertexShader(const char* prog, int length);
  void LoadFragmentShader(const char* prog, int length);

  unsigned int GetGLHandle() { return glShaderProg; }
};

#endif
