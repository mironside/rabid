// GLShaderProgram.cpp
#include "GLShaderProgram.h"
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#define GL_GLEXT_PROTOTYPES
#endif
#include <GL/gl.h>
#include "glext.h"
#include "common/log.h"
#include <stdio.h>

extern PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObject;
extern PFNGLSHADERSOURCEARBPROC glShaderSource;
extern PFNGLCOMPILESHADERARBPROC glCompileShader;
extern PFNGLDELETEOBJECTARBPROC glDeleteObject;
extern PFNGLGETINFOLOGARBPROC glGetInfoLog;
extern PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameteriv;
extern PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObject;
extern PFNGLATTACHOBJECTARBPROC glAttachObject;
extern PFNGLLINKPROGRAMARBPROC glLinkProgram;
extern PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObject;
extern PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocation;
extern PFNGLUNIFORM3FARBPROC glUniform3f;
extern PFNGLUNIFORM1IARBPROC glUniform1i;


GLShaderProgram::GLShaderProgram() :
  glShaderProg(0)
{
  glShaderProg = glCreateProgramObject();
}



GLShaderProgram::~GLShaderProgram()
{
}




void GLShaderProgram::LoadVertexShader(const char* prog, int length)
{
  printf("VS:\n%s\n", prog);
  unsigned int vs = glCreateShaderObject(GL_VERTEX_SHADER_ARB);
  glShaderSource(vs, 1, (const GLcharARB**)&prog, &length);
  glCompileShader(vs);

  int status;
  glGetObjectParameteriv(vs, GL_OBJECT_COMPILE_STATUS_ARB, &status);
  if(status == GL_FALSE)
  {
    char msg[256];
    glGetInfoLog(vs, 256, 0, msg);
    LOG(LOG_RED, "*** VERTEX SHADER ERROR: %s", msg);
    return;
  }

  glAttachObject(GetGLHandle(), vs);
  glLinkProgram(GetGLHandle());

  glGetObjectParameteriv(GetGLHandle(), GL_OBJECT_LINK_STATUS_ARB, &status);
  if(status == GL_FALSE)
  {
    char msg[256];
    glGetInfoLog(vs, 256, 0, msg);
    LOG(LOG_RED, "*** VERTEX SHADER ERROR: %s", msg);
    return;
  }
}



void GLShaderProgram::LoadFragmentShader(const char* prog, int length)
{
  unsigned int fs = glCreateShaderObject(GL_FRAGMENT_SHADER_ARB);
  if(!fs)
  {
    LOG(LOG_RED, "*** GLSL ERROR: Fragment Shaders Not Supported!");
    return;
  }

  glShaderSource(fs, 1, (const GLcharARB**)&prog, &length);
  glCompileShader(fs);

  int status;
  glGetObjectParameteriv(fs, GL_OBJECT_COMPILE_STATUS_ARB, &status);
  if(status == GL_FALSE)
  {
    char msg[256];
    glGetInfoLog(fs, 256, 0, msg);
    LOG(LOG_RED, "*** GLSL ERROR: %s", msg);
    return;
  }

  glAttachObject(GetGLHandle(), fs);
  glLinkProgram(GetGLHandle());

  glGetObjectParameteriv(GetGLHandle(), GL_OBJECT_LINK_STATUS_ARB, &status);
  if(status == GL_FALSE)
  {
    char msg[256];
    glGetInfoLog(fs, 256, 0, msg);
    LOG(LOG_RED, "*** GLSL ERROR: %s", msg);
    return;
  }
}

