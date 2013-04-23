/**
  @file win32GLRenderDevice.cpp
  @author Christopher Olsen
  @date Tue Oct  5 13:38:52 2004

  Copyright (C) 2004 Christopher A Olsen
*/
#include "GLRenderDevice.h"
#include "common/log.h"
#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#include "render/GL/glext.h"


static HWND hWnd;
static HINSTANCE hInstance;
static HDC hDC;
static HGLRC hRC;

PFNGLBINDBUFFERARBPROC       glBindBuffer;
PFNGLGENBUFFERSARBPROC       glGenBuffers;
PFNGLBUFFERDATAARBPROC       glBufferData;
PFNGLBUFFERSUBDATAPROC       glBufferSubData;
PFNGLDELETEBUFFERSARBPROC    glDeleteBuffers;
PFNGLMAPBUFFERPROC           glMapBuffer;
PFNGLUNMAPBUFFERPROC         glUnmapBuffer;
PFNGLCLIENTACTIVETEXTUREPROC glClientActiveTexture;
PFNGLACTIVETEXTUREPROC       glActiveTexture;
PFNGLBLENDEQUATIONPROC       glBlendEquation;
PFNGLMULTITEXCOORD2FPROC     glMultiTexCoord2f;

PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;


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




void GLRenderDevice::Create(int arg1, int arg2, int arg3)
{
  FN(0, "GLRenderDevice::Create");

  hWnd = (HWND)arg1;
  hInstance = (HINSTANCE)arg2;

	GLuint		PixelFormat;
  hWnd = (HWND)arg1;
  hInstance = (HINSTANCE)arg2;

	static	PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};


  if(!(hDC = GetDC(hWnd)))
	{
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return;
	}

	if(!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))
	{
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return;
	}

	if(!SetPixelFormat(hDC, PixelFormat,&pfd))
	{
		MessageBox(NULL, "Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return;
	}

	if(!(hRC = wglCreateContext(hDC)))
	{
		MessageBox(NULL, "Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return;
	}

	if(!wglMakeCurrent(hDC, hRC))
	{
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return;
	}

  InitializeExtensions();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  Clear(GL_COLOR_BUFFER_BIT);

  InitializeExtensions();
  char* ext = (char*)glGetString(GL_EXTENSIONS);
  FILE* file = fopen("extensions.txt", "w");
  char* tok = strtok(ext, " ");
  do
  {
    fprintf(file, "%s\n", tok);
  } while(tok = strtok(0, " "));
  
  fclose(file);
}



void GLRenderDevice::Destroy()
{
  FN(0, "GLRenderDevice::Destroy");
	if(hRC)
	{
		if(!wglMakeCurrent(NULL,NULL))
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if(!wglDeleteContext(hRC))
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;
	}

	if(hDC && !ReleaseDC(hWnd, hDC))
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if(hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
	}

	if(!UnregisterClass("OpenGL", hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
	}

  LOG_STOP();
  delete this;
}



void GLRenderDevice::Flip()
{
  FN(0, "GLRenderDevice::Flip");
  SwapBuffers(hDC);
}


static void* _wglGetProcAddress(char* name)
{
  void* p = wglGetProcAddress(name);
  if(!p)
  {
    char err[256];
    sprintf(err, "GL Extension not supported %s", name);
    MessageBox(0, err, "ERROR", MB_OK);
    return 0;
  }
  return p;
}


void GLRenderDevice::InitializeExtensions()
{
  FN(0, "GLRenderDevice::InitializeExtensions");
  glBindBuffer    = (PFNGLBINDBUFFERPROC)_wglGetProcAddress("glBindBuffer");
  glGenBuffers    = (PFNGLGENBUFFERSPROC)_wglGetProcAddress("glGenBuffers");
  glBufferData    = (PFNGLBUFFERDATAPROC)_wglGetProcAddress("glBufferData");
  glBufferSubData = (PFNGLBUFFERSUBDATAPROC)_wglGetProcAddress("glBufferSubData");
  glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)_wglGetProcAddress("glDeleteBuffers");
  glMapBuffer     = (PFNGLMAPBUFFERPROC)_wglGetProcAddress("glMapBuffer");
  glUnmapBuffer   = (PFNGLUNMAPBUFFERPROC)_wglGetProcAddress("glUnmapBuffer");
  glClientActiveTexture = (PFNGLCLIENTACTIVETEXTUREPROC)_wglGetProcAddress("glClientActiveTexture");
  glActiveTexture = (PFNGLACTIVETEXTUREPROC)_wglGetProcAddress("glActiveTexture");
  glBlendEquation = (PFNGLBLENDEQUATIONPROC)_wglGetProcAddress("glBlendEquation");

  glMultiTexCoord2f = (PFNGLMULTITEXCOORD2FPROC)_wglGetProcAddress("glMultiTexCoord2f");

  glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC)_wglGetProcAddress("glCompressedTexImage2D");



  // glsl -- Shader Objects
  glDeleteObject =         (PFNGLDELETEOBJECTARBPROC)_wglGetProcAddress("glDeleteObjectARB");
  glGetHandle =            (PFNGLGETHANDLEARBPROC)_wglGetProcAddress("glGetHandleARB");
  glDetachObject =         (PFNGLDETACHOBJECTARBPROC)_wglGetProcAddress("glDetachObjectARB");
  glCreateShaderObject =   (PFNGLCREATESHADEROBJECTARBPROC)_wglGetProcAddress("glCreateShaderObjectARB");
  glShaderSource =         (PFNGLSHADERSOURCEARBPROC)_wglGetProcAddress("glShaderSourceARB");
  glCompileShader =        (PFNGLCOMPILESHADERARBPROC)_wglGetProcAddress("glCompileShaderARB");
  glCreateProgramObject =  (PFNGLCREATEPROGRAMOBJECTARBPROC)_wglGetProcAddress("glCreateProgramObjectARB");
  glAttachObject =         (PFNGLATTACHOBJECTARBPROC)_wglGetProcAddress("glAttachObjectARB");
  glLinkProgram =          (PFNGLLINKPROGRAMARBPROC)_wglGetProcAddress("glLinkProgramARB");
  glUseProgramObject =     (PFNGLUSEPROGRAMOBJECTARBPROC)_wglGetProcAddress("glUseProgramObjectARB");
  glValidateProgram =      (PFNGLVALIDATEPROGRAMARBPROC)_wglGetProcAddress("glValidateProgramARB");
  glGetObjectParameterfv = (PFNGLGETOBJECTPARAMETERFVARBPROC)_wglGetProcAddress("glGetObjectParameterfvARB");
  glGetObjectParameteriv = (PFNGLGETOBJECTPARAMETERIVARBPROC)_wglGetProcAddress("glGetObjectParameterivARB");
  glGetInfoLog =           (PFNGLGETINFOLOGARBPROC)_wglGetProcAddress("glGetInfoLogARB");
  glGetAttachedObjects =   (PFNGLGETATTACHEDOBJECTSARBPROC)_wglGetProcAddress("glGetAttachedObjectsARB");
  glGetUniformLocation =   (PFNGLGETUNIFORMLOCATIONARBPROC)_wglGetProcAddress("glGetUniformLocationARB");
  glGetActiveUniform =     (PFNGLGETACTIVEUNIFORMARBPROC)_wglGetProcAddress("glGetActiveUniformARB");
  glGetUniformfv =         (PFNGLGETUNIFORMFVARBPROC)_wglGetProcAddress("glGetUniformfvARB");
  glGetUniformiv =         (PFNGLGETUNIFORMIVARBPROC)_wglGetProcAddress("glGetUniformivARB");
  glGetShaderSource =      (PFNGLGETSHADERSOURCEARBPROC)_wglGetProcAddress("glGetShaderSourceARB");
  glUniform1f = (PFNGLUNIFORM1FARBPROC)_wglGetProcAddress("glUniform1fARB");
  glUniform2f = (PFNGLUNIFORM2FARBPROC)_wglGetProcAddress("glUniform2fARB");
  glUniform3f = (PFNGLUNIFORM3FARBPROC)_wglGetProcAddress("glUniform3fARB");
  glUniform4f = (PFNGLUNIFORM4FARBPROC)_wglGetProcAddress("glUniform4fARB");
  glUniform1i = (PFNGLUNIFORM1IARBPROC)_wglGetProcAddress("glUniform1iARB");
  glUniform2i = (PFNGLUNIFORM2IARBPROC)_wglGetProcAddress("glUniform2iARB");
  glUniform3i = (PFNGLUNIFORM3IARBPROC)_wglGetProcAddress("glUniform3iARB");
  glUniform4i = (PFNGLUNIFORM4IARBPROC)_wglGetProcAddress("glUniform4iARB");
  glUniform1fv = (PFNGLUNIFORM1FVARBPROC)_wglGetProcAddress("glUniform1fvARB");
  glUniform2fv = (PFNGLUNIFORM2FVARBPROC)_wglGetProcAddress("glUniform2fvARB");
  glUniform3fv = (PFNGLUNIFORM3FVARBPROC)_wglGetProcAddress("glUniform3fvARB");
  glUniform4fv = (PFNGLUNIFORM4FVARBPROC)_wglGetProcAddress("glUniform4fvARB");
  glUniform1iv = (PFNGLUNIFORM1IVARBPROC)_wglGetProcAddress("glUniform1ivARB");
  glUniform2iv = (PFNGLUNIFORM2IVARBPROC)_wglGetProcAddress("glUniform2ivARB");
  glUniform3iv = (PFNGLUNIFORM3IVARBPROC)_wglGetProcAddress("glUniform3ivARB");
  glUniform4iv = (PFNGLUNIFORM4IVARBPROC)_wglGetProcAddress("glUniform4ivARB");
  glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVARBPROC)_wglGetProcAddress("glUniformMatrix2fvARB");
  glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVARBPROC)_wglGetProcAddress("glUniformMatrix3fvARB");
  glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVARBPROC)_wglGetProcAddress("glUniformMatrix4fvARB");


  // glsl -- vertex shader
  glVertexAttrib1f = (PFNGLVERTEXATTRIB1FARBPROC)_wglGetProcAddress("glVertexAttrib1fARB");
  glVertexAttrib1s = (PFNGLVERTEXATTRIB1SARBPROC)_wglGetProcAddress("glVertexAttrib1sARB");
  glVertexAttrib1d = (PFNGLVERTEXATTRIB1DARBPROC)_wglGetProcAddress("glVertexAttrib1dARB");
  glVertexAttrib2f = (PFNGLVERTEXATTRIB2FARBPROC)_wglGetProcAddress("glVertexAttrib2fARB");
  glVertexAttrib2s = (PFNGLVERTEXATTRIB2SARBPROC)_wglGetProcAddress("glVertexAttrib2sARB");
  glVertexAttrib2d = (PFNGLVERTEXATTRIB2DARBPROC)_wglGetProcAddress("glVertexAttrib2dARB");
  glVertexAttrib3f = (PFNGLVERTEXATTRIB3FARBPROC)_wglGetProcAddress("glVertexAttrib3fARB");
  glVertexAttrib3s = (PFNGLVERTEXATTRIB3SARBPROC)_wglGetProcAddress("glVertexAttrib3sARB");
  glVertexAttrib3d = (PFNGLVERTEXATTRIB3DARBPROC)_wglGetProcAddress("glVertexAttrib3dARB");
  glVertexAttrib4f = (PFNGLVERTEXATTRIB4FARBPROC)_wglGetProcAddress("glVertexAttrib4fARB");
  glVertexAttrib4s = (PFNGLVERTEXATTRIB4SARBPROC)_wglGetProcAddress("glVertexAttrib4sARB");
  glVertexAttrib4d = (PFNGLVERTEXATTRIB4DARBPROC)_wglGetProcAddress("glVertexAttrib4dARB");
  glVertexAttrib4Nub = (PFNGLVERTEXATTRIB4NUBARBPROC)_wglGetProcAddress("glVertexAttrib4NubARB");

  glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVARBPROC)_wglGetProcAddress("glVertexAttrib1fvARB");
  glVertexAttrib1sv = (PFNGLVERTEXATTRIB1SVARBPROC)_wglGetProcAddress("glVertexAttrib1svARB");
  glVertexAttrib1dv = (PFNGLVERTEXATTRIB1DVARBPROC)_wglGetProcAddress("glVertexAttrib1dvARB");
  glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVARBPROC)_wglGetProcAddress("glVertexAttrib2fvARB");
  glVertexAttrib2sv = (PFNGLVERTEXATTRIB2SVARBPROC)_wglGetProcAddress("glVertexAttrib2svARB");
  glVertexAttrib2dv = (PFNGLVERTEXATTRIB2DVARBPROC)_wglGetProcAddress("glVertexAttrib2dvARB");
  glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVARBPROC)_wglGetProcAddress("glVertexAttrib3fvARB");
  glVertexAttrib3sv = (PFNGLVERTEXATTRIB3SVARBPROC)_wglGetProcAddress("glVertexAttrib3svARB");
  glVertexAttrib3dv = (PFNGLVERTEXATTRIB3DVARBPROC)_wglGetProcAddress("glVertexAttrib3dvARB");
  glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVARBPROC)_wglGetProcAddress("glVertexAttrib4fvARB");
  glVertexAttrib4sv = (PFNGLVERTEXATTRIB4SVARBPROC)_wglGetProcAddress("glVertexAttrib4svARB");
  glVertexAttrib4dv = (PFNGLVERTEXATTRIB4DVARBPROC)_wglGetProcAddress("glVertexAttrib4dvARB");
  glVertexAttrib4iv = (PFNGLVERTEXATTRIB4IVARBPROC)_wglGetProcAddress("glVertexAttrib4ivARB");
  glVertexAttrib4bv = (PFNGLVERTEXATTRIB4BVARBPROC)_wglGetProcAddress("glVertexAttrib4bvARB");

  glVertexAttrib4ubv = (PFNGLVERTEXATTRIB4UBVARBPROC)_wglGetProcAddress("glVertexAttrib4ubvARB");
  glVertexAttrib4usv = (PFNGLVERTEXATTRIB4USVARBPROC)_wglGetProcAddress("glVertexAttrib4usvARB");
  glVertexAttrib4uiv = (PFNGLVERTEXATTRIB4UIVARBPROC)_wglGetProcAddress("glVertexAttrib4uivARB");

  glVertexAttrib4Nbv = (PFNGLVERTEXATTRIB4NBVARBPROC)_wglGetProcAddress("glVertexAttrib4NbvARB");
  glVertexAttrib4Nsv = (PFNGLVERTEXATTRIB4NSVARBPROC)_wglGetProcAddress("glVertexAttrib4NsvARB");
  glVertexAttrib4Niv = (PFNGLVERTEXATTRIB4NIVARBPROC)_wglGetProcAddress("glVertexAttrib4NivARB");
  glVertexAttrib4Nubv = (PFNGLVERTEXATTRIB4NUBVARBPROC)_wglGetProcAddress("glVertexAttrib4NubvARB");
  glVertexAttrib4Nusv = (PFNGLVERTEXATTRIB4NUSVARBPROC)_wglGetProcAddress("glVertexAttrib4NusvARB");
  glVertexAttrib4Nuiv = (PFNGLVERTEXATTRIB4NUIVARBPROC)_wglGetProcAddress("glVertexAttrib4NuivARB");

  glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERARBPROC)_wglGetProcAddress("glVertexAttribPointerARB");
  glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYARBPROC)_wglGetProcAddress("glEnableVertexAttribArrayARB");
  glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYARBPROC)_wglGetProcAddress("glDisableVertexAttribArrayARB");
  glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONARBPROC)_wglGetProcAddress("glBindAttribLocationARB");
  glGetActiveAttrib = (PFNGLGETACTIVEATTRIBARBPROC)_wglGetProcAddress("glGetActiveAttribARB");
  glGetAttribLocation = (PFNGLGETATTRIBLOCATIONARBPROC)_wglGetProcAddress("glGetAttribLocationARB");
  glGetVertexAttribdv = (PFNGLGETVERTEXATTRIBDVARBPROC)_wglGetProcAddress("glGetVertexAttribdvARB");
  glGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFVARBPROC)_wglGetProcAddress("glGetVertexAttribfvARB");
  glGetVertexAttribiv = (PFNGLGETVERTEXATTRIBIVARBPROC)_wglGetProcAddress("glGetVertexAttribivARB");
  glGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERVARBPROC)_wglGetProcAddress("glGetVertexAttribPointervARB");
}

