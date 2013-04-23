/**
  @file linuxGLRenderDevice.cpp
  @author Christopher Olsen
  @date Fri Sep 10 16:00:28 2004

  Copyright (C) 2004 Christopher A Olsen
*/
#include "GLRenderDevice.h"
#include "common/log.h"
#include <GL/glx.h>
#include <stdio.h>
#include <string.h>
#include "glext.h"

// glsl extensions
// shader objects
PFNGLDELETEOBJECTARBPROC        glDeleteObject = 0;  
PFNGLGETHANDLEARBPROC           glGetHandle = 0;
PFNGLDETACHOBJECTARBPROC        glDetachObject = 0;
PFNGLCREATESHADEROBJECTARBPROC  glCreateShaderObject = 0;
PFNGLSHADERSOURCEARBPROC        glShaderSource = 0;
PFNGLCOMPILESHADERARBPROC       glCompileShader = 0;
PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObject = 0;
PFNGLATTACHOBJECTARBPROC        glAttachObject = 0;
PFNGLLINKPROGRAMARBPROC         glLinkProgram = 0;
PFNGLUSEPROGRAMOBJECTARBPROC    glUseProgramObject = 0;
PFNGLVALIDATEPROGRAMARBPROC     glValidateProgram = 0;

PFNGLUNIFORM1FARBPROC glUniform1f = 0;
PFNGLUNIFORM2FARBPROC glUniform2f = 0;
PFNGLUNIFORM3FARBPROC glUniform3f = 0;
PFNGLUNIFORM4FARBPROC glUniform4f = 0;
PFNGLUNIFORM1IARBPROC glUniform1i = 0;
PFNGLUNIFORM2IARBPROC glUniform2i = 0;
PFNGLUNIFORM3IARBPROC glUniform3i = 0;
PFNGLUNIFORM4IARBPROC glUniform4i = 0;
PFNGLUNIFORM1FVARBPROC glUniform1fv = 0;
PFNGLUNIFORM2FVARBPROC glUniform2fv = 0;
PFNGLUNIFORM3FVARBPROC glUniform3fv = 0;
PFNGLUNIFORM4FVARBPROC glUniform4fv = 0;
PFNGLUNIFORM1IVARBPROC glUniform1iv = 0;
PFNGLUNIFORM2IVARBPROC glUniform2iv = 0;
PFNGLUNIFORM3IVARBPROC glUniform3iv = 0;
PFNGLUNIFORM4IVARBPROC glUniform4iv = 0;
PFNGLUNIFORMMATRIX2FVARBPROC glUniformMatrix2fv = 0;
PFNGLUNIFORMMATRIX3FVARBPROC glUniformMatrix3fv = 0;
PFNGLUNIFORMMATRIX4FVARBPROC glUniformMatrix4fv = 0;
PFNGLGETOBJECTPARAMETERFVARBPROC glGetObjectParameterfv = 0;
PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameteriv = 0;
PFNGLGETINFOLOGARBPROC glGetInfoLog = 0;
PFNGLGETATTACHEDOBJECTSARBPROC glGetAttachedObjects = 0;
PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocation = 0;
PFNGLGETACTIVEUNIFORMARBPROC glGetActiveUniform = 0;
PFNGLGETUNIFORMFVARBPROC glGetUniformfv = 0;
PFNGLGETUNIFORMIVARBPROC glGetUniformiv = 0;
PFNGLGETSHADERSOURCEARBPROC glGetShaderSource = 0;

// glsl -- vertex shader
PFNGLVERTEXATTRIB1FARBPROC glVertexAttrib1f = 0;
PFNGLVERTEXATTRIB1SARBPROC glVertexAttrib1s = 0;
PFNGLVERTEXATTRIB1DARBPROC glVertexAttrib1d = 0;
PFNGLVERTEXATTRIB2FARBPROC glVertexAttrib2f = 0;
PFNGLVERTEXATTRIB2SARBPROC glVertexAttrib2s = 0;
PFNGLVERTEXATTRIB2DARBPROC glVertexAttrib2d = 0;
PFNGLVERTEXATTRIB3FARBPROC glVertexAttrib3f = 0;
PFNGLVERTEXATTRIB3SARBPROC glVertexAttrib3s = 0;
PFNGLVERTEXATTRIB3DARBPROC glVertexAttrib3d = 0;
PFNGLVERTEXATTRIB4FARBPROC glVertexAttrib4f = 0;
PFNGLVERTEXATTRIB4SARBPROC glVertexAttrib4s = 0;
PFNGLVERTEXATTRIB4DARBPROC glVertexAttrib4d = 0;
PFNGLVERTEXATTRIB4NUBARBPROC glVertexAttrib4Nub = 0;

PFNGLVERTEXATTRIB1FVARBPROC glVertexAttrib1fv = 0;
PFNGLVERTEXATTRIB1SVARBPROC glVertexAttrib1sv = 0;
PFNGLVERTEXATTRIB1DVARBPROC glVertexAttrib1dv = 0;
PFNGLVERTEXATTRIB2FVARBPROC glVertexAttrib2fv = 0;
PFNGLVERTEXATTRIB2SVARBPROC glVertexAttrib2sv = 0;
PFNGLVERTEXATTRIB2DVARBPROC glVertexAttrib2dv = 0;
PFNGLVERTEXATTRIB3FVARBPROC glVertexAttrib3fv = 0;
PFNGLVERTEXATTRIB3SVARBPROC glVertexAttrib3sv = 0;
PFNGLVERTEXATTRIB3DVARBPROC glVertexAttrib3dv = 0;
PFNGLVERTEXATTRIB4FVARBPROC glVertexAttrib4fv = 0;
PFNGLVERTEXATTRIB4SVARBPROC glVertexAttrib4sv = 0;
PFNGLVERTEXATTRIB4DVARBPROC glVertexAttrib4dv = 0;
PFNGLVERTEXATTRIB4IVARBPROC glVertexAttrib4iv = 0;
PFNGLVERTEXATTRIB4BVARBPROC glVertexAttrib4bv = 0;

PFNGLVERTEXATTRIB4UBVARBPROC glVertexAttrib4ubv = 0;
PFNGLVERTEXATTRIB4USVARBPROC glVertexAttrib4usv = 0;
PFNGLVERTEXATTRIB4UIVARBPROC glVertexAttrib4uiv = 0;

PFNGLVERTEXATTRIB4NBVARBPROC glVertexAttrib4Nbv = 0;
PFNGLVERTEXATTRIB4NSVARBPROC glVertexAttrib4Nsv = 0;
PFNGLVERTEXATTRIB4NIVARBPROC glVertexAttrib4Niv = 0;
PFNGLVERTEXATTRIB4NUBVARBPROC glVertexAttrib4Nubv = 0;
PFNGLVERTEXATTRIB4NUSVARBPROC glVertexAttrib4Nusv = 0;
PFNGLVERTEXATTRIB4NUIVARBPROC glVertexAttrib4Nuiv = 0;

PFNGLVERTEXATTRIBPOINTERARBPROC glVertexAttribPointer = 0;
PFNGLENABLEVERTEXATTRIBARRAYARBPROC glEnableVertexAttribArray = 0;
PFNGLDISABLEVERTEXATTRIBARRAYARBPROC glDisableVertexAttribArray = 0;
PFNGLBINDATTRIBLOCATIONARBPROC glBindAttribLocation = 0;
PFNGLGETACTIVEATTRIBARBPROC glGetActiveAttrib = 0;
PFNGLGETATTRIBLOCATIONARBPROC glGetAttribLocation = 0;
PFNGLGETVERTEXATTRIBDVARBPROC glGetVertexAttribdv = 0;
PFNGLGETVERTEXATTRIBFVARBPROC glGetVertexAttribfv = 0;
PFNGLGETVERTEXATTRIBIVARBPROC glGetVertexAttribiv = 0;
PFNGLGETVERTEXATTRIBPOINTERVARBPROC glGetVertexAttribPointerv = 0;

static Window window;
static Display* display;
static int screen;
static GLXContext glx;




void GLRenderDevice::Create(int arg1, int arg2, int arg3)
{
  window = (Window)arg1;
  display = (Display*)arg2;
  screen = arg3;


  XVisualInfo *vi;
  int glxMajorVersion, glxMinorVersion;
  Bool doubleBuffered;

  static int attrListSgl[] =
    {GLX_RGBA, GLX_RED_SIZE, 8, 
     GLX_GREEN_SIZE, 8,
     GLX_BLUE_SIZE, 8,
     GLX_DEPTH_SIZE, 16,
     None};

  static int attrListDbl[] =
    {GLX_RGBA, GLX_DOUBLEBUFFER, 
     GLX_RED_SIZE, 8, 
     GLX_GREEN_SIZE, 8, 
     GLX_BLUE_SIZE, 8, 
     GLX_DEPTH_SIZE, 16,
     None };


  /* get an appropriate visual */ 
  vi = glXChooseVisual(display, screen, attrListDbl);
  if(vi == NULL)
  {
    vi = glXChooseVisual(display, screen, attrListSgl);
    doubleBuffered = False;
  }
  else
  {
    doubleBuffered = True;
  }
  glXQueryVersion(display, &glxMajorVersion, &glxMinorVersion);

  /* create a GLX context */
  glx = glXCreateContext(display, vi, 0, GL_TRUE);

  /* connect the glx-context to the XWindow */
  glXMakeCurrent(display, window, glx);

  InitializeExtensions();
}



void GLRenderDevice::Destroy()
{
  //  FN(0, "GLRenderer::Destroy()");
  if(glx)
  {
    if(!glXMakeCurrent(display, None, NULL))
    {
    }
    glXDestroyContext(display, glx);
    glx = NULL;
  }
}



void GLRenderDevice::Flip()
{
  glXSwapBuffers(display, window);
}



static void* _glXGetProcAddress(char* name)
{
  void* func;
  
  /* Open GL library and get pointer to the function, if it exists. */
  func = (void*)glXGetProcAddressARB((const GLubyte*)name);
  
  if(!func)
  {
    printf("GL ERROR: Extension not supported %s\n", name);
    return 0;
  }

  return func;
}



void GLRenderDevice::InitializeExtensions()
{
  
  printf("%s\n", (char*)glGetString(GL_VENDOR));
  printf("%s\n", (char*)glGetString(GL_RENDERER));
  printf("OpenGL %s\n", (char*)glGetString(GL_VERSION));

  /*
  printf("Supported Extensions:\n");
  char* ext = (char*)glGetString(GL_EXTENSIONS);
  char* tok = strtok(ext, " ");
  while(tok)
  {
    printf("%s\n", tok);
    tok = strtok(0, " ");
  }
  */

  // glsl -- Shader Objects
  glDeleteObject =         (PFNGLDELETEOBJECTARBPROC)_glXGetProcAddress("glDeleteObjectARB");
  glGetHandle =            (PFNGLGETHANDLEARBPROC)_glXGetProcAddress("glGetHandleARB");
  glDetachObject =         (PFNGLDETACHOBJECTARBPROC)_glXGetProcAddress("glDetachObjectARB");
  glCreateShaderObject =   (PFNGLCREATESHADEROBJECTARBPROC)_glXGetProcAddress("glCreateShaderObjectARB");
  glShaderSource =         (PFNGLSHADERSOURCEARBPROC)_glXGetProcAddress("glShaderSourceARB");
  glCompileShader =        (PFNGLCOMPILESHADERARBPROC)_glXGetProcAddress("glCompileShaderARB");
  glCreateProgramObject =  (PFNGLCREATEPROGRAMOBJECTARBPROC)_glXGetProcAddress("glCreateProgramObjectARB");
  glAttachObject =         (PFNGLATTACHOBJECTARBPROC)_glXGetProcAddress("glAttachObjectARB");
  glLinkProgram =          (PFNGLLINKPROGRAMARBPROC)_glXGetProcAddress("glLinkProgramARB");
  glUseProgramObject =     (PFNGLUSEPROGRAMOBJECTARBPROC)_glXGetProcAddress("glUseProgramObjectARB");
  glValidateProgram =      (PFNGLVALIDATEPROGRAMARBPROC)_glXGetProcAddress("glValidateProgramARB");
  glGetObjectParameterfv = (PFNGLGETOBJECTPARAMETERFVARBPROC)_glXGetProcAddress("glGetObjectParameterfvARB");
  glGetObjectParameteriv = (PFNGLGETOBJECTPARAMETERIVARBPROC)_glXGetProcAddress("glGetObjectParameterivARB");
  glGetInfoLog =           (PFNGLGETINFOLOGARBPROC)_glXGetProcAddress("glGetInfoLogARB");
  glGetAttachedObjects =   (PFNGLGETATTACHEDOBJECTSARBPROC)_glXGetProcAddress("glGetAttachedObjectsARB");
  glGetUniformLocation =   (PFNGLGETUNIFORMLOCATIONARBPROC)_glXGetProcAddress("glGetUniformLocationARB");
  glGetActiveUniform =     (PFNGLGETACTIVEUNIFORMARBPROC)_glXGetProcAddress("glGetActiveUniformARB");
  glGetUniformfv =         (PFNGLGETUNIFORMFVARBPROC)_glXGetProcAddress("glGetUniformfvARB");
  glGetUniformiv =         (PFNGLGETUNIFORMIVARBPROC)_glXGetProcAddress("glGetUniformivARB");
  glGetShaderSource =      (PFNGLGETSHADERSOURCEARBPROC)_glXGetProcAddress("glGetShaderSourceARB");
  glUniform1f = (PFNGLUNIFORM1FARBPROC)_glXGetProcAddress("glUniform1fARB");
  glUniform2f = (PFNGLUNIFORM2FARBPROC)_glXGetProcAddress("glUniform2fARB");
  glUniform3f = (PFNGLUNIFORM3FARBPROC)_glXGetProcAddress("glUniform3fARB");
  glUniform4f = (PFNGLUNIFORM4FARBPROC)_glXGetProcAddress("glUniform4fARB");
  glUniform1i = (PFNGLUNIFORM1IARBPROC)_glXGetProcAddress("glUniform1iARB");
  glUniform2i = (PFNGLUNIFORM2IARBPROC)_glXGetProcAddress("glUniform2iARB");
  glUniform3i = (PFNGLUNIFORM3IARBPROC)_glXGetProcAddress("glUniform3iARB");
  glUniform4i = (PFNGLUNIFORM4IARBPROC)_glXGetProcAddress("glUniform4iARB");
  glUniform1fv = (PFNGLUNIFORM1FVARBPROC)_glXGetProcAddress("glUniform1fvARB");
  glUniform2fv = (PFNGLUNIFORM2FVARBPROC)_glXGetProcAddress("glUniform2fvARB");
  glUniform3fv = (PFNGLUNIFORM3FVARBPROC)_glXGetProcAddress("glUniform3fvARB");
  glUniform4fv = (PFNGLUNIFORM4FVARBPROC)_glXGetProcAddress("glUniform4fvARB");
  glUniform1iv = (PFNGLUNIFORM1IVARBPROC)_glXGetProcAddress("glUniform1ivARB");
  glUniform2iv = (PFNGLUNIFORM2IVARBPROC)_glXGetProcAddress("glUniform2ivARB");
  glUniform3iv = (PFNGLUNIFORM3IVARBPROC)_glXGetProcAddress("glUniform3ivARB");
  glUniform4iv = (PFNGLUNIFORM4IVARBPROC)_glXGetProcAddress("glUniform4ivARB");
  glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVARBPROC)_glXGetProcAddress("glUniformMatrix2fvARB");
  glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVARBPROC)_glXGetProcAddress("glUniformMatrix3fvARB");
  glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVARBPROC)_glXGetProcAddress("glUniformMatrix4fvARB");


  // glsl -- vertex shader
  glVertexAttrib1f = (PFNGLVERTEXATTRIB1FARBPROC)_glXGetProcAddress("glVertexAttrib1fARB");
  glVertexAttrib1s = (PFNGLVERTEXATTRIB1SARBPROC)_glXGetProcAddress("glVertexAttrib1sARB");
  glVertexAttrib1d = (PFNGLVERTEXATTRIB1DARBPROC)_glXGetProcAddress("glVertexAttrib1dARB");
  glVertexAttrib2f = (PFNGLVERTEXATTRIB2FARBPROC)_glXGetProcAddress("glVertexAttrib2fARB");
  glVertexAttrib2s = (PFNGLVERTEXATTRIB2SARBPROC)_glXGetProcAddress("glVertexAttrib2sARB");
  glVertexAttrib2d = (PFNGLVERTEXATTRIB2DARBPROC)_glXGetProcAddress("glVertexAttrib2dARB");
  glVertexAttrib3f = (PFNGLVERTEXATTRIB3FARBPROC)_glXGetProcAddress("glVertexAttrib3fARB");
  glVertexAttrib3s = (PFNGLVERTEXATTRIB3SARBPROC)_glXGetProcAddress("glVertexAttrib3sARB");
  glVertexAttrib3d = (PFNGLVERTEXATTRIB3DARBPROC)_glXGetProcAddress("glVertexAttrib3dARB");
  glVertexAttrib4f = (PFNGLVERTEXATTRIB4FARBPROC)_glXGetProcAddress("glVertexAttrib4fARB");
  glVertexAttrib4s = (PFNGLVERTEXATTRIB4SARBPROC)_glXGetProcAddress("glVertexAttrib4sARB");
  glVertexAttrib4d = (PFNGLVERTEXATTRIB4DARBPROC)_glXGetProcAddress("glVertexAttrib4dARB");
  glVertexAttrib4Nub = (PFNGLVERTEXATTRIB4NUBARBPROC)_glXGetProcAddress("glVertexAttrib4NubARB");

  glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVARBPROC)_glXGetProcAddress("glVertexAttrib1fvARB");
  glVertexAttrib1sv = (PFNGLVERTEXATTRIB1SVARBPROC)_glXGetProcAddress("glVertexAttrib1svARB");
  glVertexAttrib1dv = (PFNGLVERTEXATTRIB1DVARBPROC)_glXGetProcAddress("glVertexAttrib1dvARB");
  glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVARBPROC)_glXGetProcAddress("glVertexAttrib2fvARB");
  glVertexAttrib2sv = (PFNGLVERTEXATTRIB2SVARBPROC)_glXGetProcAddress("glVertexAttrib2svARB");
  glVertexAttrib2dv = (PFNGLVERTEXATTRIB2DVARBPROC)_glXGetProcAddress("glVertexAttrib2dvARB");
  glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVARBPROC)_glXGetProcAddress("glVertexAttrib3fvARB");
  glVertexAttrib3sv = (PFNGLVERTEXATTRIB3SVARBPROC)_glXGetProcAddress("glVertexAttrib3svARB");
  glVertexAttrib3dv = (PFNGLVERTEXATTRIB3DVARBPROC)_glXGetProcAddress("glVertexAttrib3dvARB");
  glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVARBPROC)_glXGetProcAddress("glVertexAttrib4fvARB");
  glVertexAttrib4sv = (PFNGLVERTEXATTRIB4SVARBPROC)_glXGetProcAddress("glVertexAttrib4svARB");
  glVertexAttrib4dv = (PFNGLVERTEXATTRIB4DVARBPROC)_glXGetProcAddress("glVertexAttrib4dvARB");
  glVertexAttrib4iv = (PFNGLVERTEXATTRIB4IVARBPROC)_glXGetProcAddress("glVertexAttrib4ivARB");
  glVertexAttrib4bv = (PFNGLVERTEXATTRIB4BVARBPROC)_glXGetProcAddress("glVertexAttrib4bvARB");

  glVertexAttrib4ubv = (PFNGLVERTEXATTRIB4UBVARBPROC)_glXGetProcAddress("glVertexAttrib4ubvARB");
  glVertexAttrib4usv = (PFNGLVERTEXATTRIB4USVARBPROC)_glXGetProcAddress("glVertexAttrib4usvARB");
  glVertexAttrib4uiv = (PFNGLVERTEXATTRIB4UIVARBPROC)_glXGetProcAddress("glVertexAttrib4uivARB");

  glVertexAttrib4Nbv = (PFNGLVERTEXATTRIB4NBVARBPROC)_glXGetProcAddress("glVertexAttrib4NbvARB");
  glVertexAttrib4Nsv = (PFNGLVERTEXATTRIB4NSVARBPROC)_glXGetProcAddress("glVertexAttrib4NsvARB");
  glVertexAttrib4Niv = (PFNGLVERTEXATTRIB4NIVARBPROC)_glXGetProcAddress("glVertexAttrib4NivARB");
  glVertexAttrib4Nubv = (PFNGLVERTEXATTRIB4NUBVARBPROC)_glXGetProcAddress("glVertexAttrib4NubvARB");
  glVertexAttrib4Nusv = (PFNGLVERTEXATTRIB4NUSVARBPROC)_glXGetProcAddress("glVertexAttrib4NusvARB");
  glVertexAttrib4Nuiv = (PFNGLVERTEXATTRIB4NUIVARBPROC)_glXGetProcAddress("glVertexAttrib4NuivARB");

  glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERARBPROC)_glXGetProcAddress("glVertexAttribPointerARB");
  glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYARBPROC)_glXGetProcAddress("glEnableVertexAttribArrayARB");
  glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYARBPROC)_glXGetProcAddress("glDisableVertexAttribArrayARB");
  glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONARBPROC)_glXGetProcAddress("glBindAttribLocationARB");
  glGetActiveAttrib = (PFNGLGETACTIVEATTRIBARBPROC)_glXGetProcAddress("glGetActiveAttribARB");
  glGetAttribLocation = (PFNGLGETATTRIBLOCATIONARBPROC)_glXGetProcAddress("glGetAttribLocationARB");
  glGetVertexAttribdv = (PFNGLGETVERTEXATTRIBDVARBPROC)_glXGetProcAddress("glGetVertexAttribdvARB");
  glGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFVARBPROC)_glXGetProcAddress("glGetVertexAttribfvARB");
  glGetVertexAttribiv = (PFNGLGETVERTEXATTRIBIVARBPROC)_glXGetProcAddress("glGetVertexAttribivARB");
  glGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERVARBPROC)_glXGetProcAddress("glGetVertexAttribPointervARB");
}

