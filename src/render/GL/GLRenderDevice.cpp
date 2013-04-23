/**
  @file GLRenderDevice.cpp
  @author Christopher Olsen
  @date Thu Sep  9 00:45:10 2004

  Copyright (C) 2004 Christopher A Olsen
*/
#include "GLRenderDevice.h"
#include "render/renderDevice.h"
#include "render/renderer_defs.h"
#include <memory.h>
#include "common/log.h"
#include "GLHardwareBufferMgr.h"
#include "GLShaderProgram.h"
#include "GLRenderState.h"
#include "GLTextureState.h"
#include "GLIndexBuffer.h"
#include "GLVertexBuffer.h"
#include "render/materialSystem/textureState.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#ifndef WIN32
#define GL_GLEXT_PROTOTYPES
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#endif
#include "glext.h"

#ifdef WIN32
extern PFNGLBLENDEQUATIONPROC glBlendEquation;
extern PFNGLACTIVETEXTUREPROC glActiveTexture;
extern PFNGLMULTITEXCOORD2FPROC glMultiTexCoord2f;
#endif

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
extern PFNGLUNIFORM1FARBPROC glUniform1f;

#ifdef WIN32
#define DLL_EXPORT extern "C" __declspec(dllexport)
#else
#define DLL_EXPORT 
#endif




extern "C"
{
  DLL_EXPORT void GetRenderDevice(IRenderDevice** device)
  {
    *device = new GLRenderDevice();
  }
}


GLRenderDevice::GLRenderDevice()
{
  LOG_START("render_log.html");
  memset(apiVertexBuffers, 0, sizeof(unsigned int) * MAX_VERTEX_BUFFERS);
  hwBufferMgr = new GLHardwareBufferMgr();
  glDrawBuffer(GL_BACK);
  glFrontFace(GL_CW);
}



GLRenderDevice::~GLRenderDevice()
{
  LOG_STOP();
}



void GLRenderDevice::SetClearColor(float r, float g, float b, float a)
{
  glClearColor(r, g, b, a);
  glClearDepth(1);
}



void GLRenderDevice::Clear(unsigned int buffers)
{
  unsigned int flags = 0;

  glClearDepth(1.0);
  if(buffers & R_COLOR_BUFFER)
    flags |= GL_COLOR_BUFFER_BIT;
  if(buffers & R_DEPTH_BUFFER)
    flags |= GL_DEPTH_BUFFER_BIT;
  if(buffers & R_STENCIL_BUFFER)
    flags |= GL_STENCIL_BUFFER_BIT;

  glClear(flags);
}



void GLRenderDevice::DrawRect(float s, float x, float y, float z)
{
  glBegin(GL_TRIANGLE_STRIP);
  {
    glVertex3f(x-s, y-s, z);
    glVertex3f(x-s, y+s, z);
    glVertex3f(x+s, y-s, z);
    glVertex3f(x+s, y+s, z);
  }
  glEnd();
}



void GLRenderDevice::DrawCoordinateAxis()
{
  glBegin(GL_LINES);
  {
    glColor3f(1,0,0);
    glVertex3f(0,0,0);
    glVertex3f(10,0,0);

    glColor3f(0,1,0);
    glVertex3f(0,0,0);
    glVertex3f(0,10,0);

    glColor3f(0,0,1);
    glVertex3f(0,0,0);
    glVertex3f(0,0,10);
  }
  glEnd();
  glColor3f(1,1,1);
}



void GLRenderDevice::SetViewMatrix(float* matrix)
{
  glMatrixMode(GL_MODELVIEW);
  if(!matrix)
  {
    glLoadIdentity();
  }
  else
  {
    glLoadIdentity();
    glLoadMatrixf(matrix);
  }
}



void GLRenderDevice::MultViewMatrix(float* matrix)
{
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glMultMatrixf(matrix);
}



void GLRenderDevice::PopViewMatrix()
{
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}



void GLRenderDevice::SetProjectionMatrix(float* matrix)
{
  if(!matrix)
  {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 1.333333, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
    return;
  }

  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(matrix);
  glMatrixMode(GL_MODELVIEW);
}



void GLRenderDevice::SetViewport(int x, int y, int w, int h)
{
  glViewport(x, y, w, h);
}



void GLRenderDevice::SetFrustum(float l, float r, float b, float t, float n, float f)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(l, r, b, t, n, f);
  glMatrixMode(GL_MODELVIEW);
}






///////////////////
// SHADER PROGRAM

ShaderProgram* GLRenderDevice::CreateShaderProgram()
{
  return (ShaderProgram*)new GLShaderProgram();
}



void GLRenderDevice::BindShaderProgram(ShaderProgram* s)
{
  GLShaderProgram* glShader = (GLShaderProgram*)s;

  if(!glShader)
  {
    glUseProgramObject(0);
    return;
  }

  glUseProgramObject(glShader->GetGLHandle());
  int l = glGetUniformLocation(glShader->GetGLHandle(), "Tex0");
  glUniform1i(l, 0);
  l = glGetUniformLocation(glShader->GetGLHandle(), "Tex1");
  glUniform1i(l, 1);
  l = glGetUniformLocation(glShader->GetGLHandle(), "Tex2");
  glUniform1i(l, 2);
  l = glGetUniformLocation(glShader->GetGLHandle(), "Tex3");
  glUniform1i(l, 3);
}





void GLRenderDevice::DrawLine(float x1, float y1, float z1,
                              float x2, float y2, float z2)
{
  glBegin(GL_LINES);
  {
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y2, z2);
  }
  glEnd();
}



void GLRenderDevice::SetColor(float r, float g, float b)
{
  glColor3f(r, g, b);
}



void GLRenderDevice::SetLight(unsigned int l, float x, float y, float z)
{
  light[0] = x;
  light[1] = y;
  light[2] = z;
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_POSITION, light);
}



void GLRenderDevice::DrawPoint(float size, float x, float y, float z)
{
  glPointSize(size);
  glBegin(GL_POINTS);
  glVertex3f(x, y, z);
  glEnd();
}



void GLRenderDevice::DrawRect(float x, float y, float sx, float sy, float tx, float ty, float stx, float sty)
{
  glBegin(GL_TRIANGLE_STRIP);
  {
    glMultiTexCoord2f(GL_TEXTURE0, tx, ty);
    glMultiTexCoord2f(GL_TEXTURE1, tx, ty);
    glMultiTexCoord2f(GL_TEXTURE2, tx, ty);
    glMultiTexCoord2f(GL_TEXTURE3, tx, ty);
    glVertex2f(x,    y);

    glMultiTexCoord2f(GL_TEXTURE0, tx, ty + sty);
    glMultiTexCoord2f(GL_TEXTURE1, tx, ty + sty);
    glMultiTexCoord2f(GL_TEXTURE2, tx, ty + sty);
    glMultiTexCoord2f(GL_TEXTURE3, tx, ty + sty);
    glVertex2f(x,    y+sy);

    glMultiTexCoord2f(GL_TEXTURE0, tx + stx, ty);
    glMultiTexCoord2f(GL_TEXTURE1, tx + stx, ty);
    glMultiTexCoord2f(GL_TEXTURE2, tx + stx, ty);
    glMultiTexCoord2f(GL_TEXTURE3, tx + stx, ty);
    glVertex2f(x+sx, y);

    glMultiTexCoord2f(GL_TEXTURE0, tx + stx, ty + sty);
    glMultiTexCoord2f(GL_TEXTURE1, tx + stx, ty + sty);
    glMultiTexCoord2f(GL_TEXTURE2, tx + stx, ty + sty);
    glMultiTexCoord2f(GL_TEXTURE3, tx + stx, ty + sty);
    glVertex2f(x+sx, y+sy);
  }
  glEnd();
}



// render state
RenderState* GLRenderDevice::CreateRenderState(int _srcBlend, int _dstBlend, int _blendFunc, int _alphaTest,
                                               float _alphaRef, int _depthTest, int _depthWrite, int _cullMode)
{
  return (RenderState*)new GLRenderState(_srcBlend, _dstBlend, _blendFunc, _alphaTest,
                                         _alphaRef, _depthTest, _depthWrite, _cullMode);
}


TextureState* GLRenderDevice::CreateTextureState(int _colorCombiner, int _alphaCombiner,
                                                 int _wrapMode, int _filterMode)
{
  return (TextureState*)new GLTextureState(_colorCombiner, _alphaCombiner,
                                           _wrapMode, _filterMode);
}


VertexBuffer* GLRenderDevice::CreateVertexBuffer(unsigned int format, unsigned int nVertices, unsigned int usage)
{
  return new GLVertexBuffer(format, nVertices, usage);
}


IndexBuffer*  GLRenderDevice::CreateIndexBuffer(unsigned int format, unsigned int nIndices)
{
  return new GLIndexBuffer(format, nIndices);
}


void GLRenderDevice::BindRenderState(RenderState* rs)
{
  if(!rs)
  {
    glBlendFunc(GL_ONE, GL_ZERO);
    glBlendEquation(GL_FUNC_ADD);
    glDepthFunc(GL_LESS);
    glDepthMask(true);
    glAlphaFunc(GL_ALWAYS, 0.0);
    glCullFace(GL_BACK);
  }
  else
  {
    rs->Bind();
  }
}


void GLRenderDevice::BindTextureState(int stage, TextureState* ts)
{
  if(ts)
    ts->Bind(stage);
}


void GLRenderDevice::SetLightDir(unsigned int light, float x, float y, float z)
{
  float dir[3] = {x,y,z};
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);
}


void GLRenderDevice::SetLightIntensity(unsigned int light, float I)
{
  glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, I);
}


void GLRenderDevice::SetLightFraction(unsigned int light, float f)
{
  glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, f);
}


void GLRenderDevice::SetUniform(ShaderProgram* s, const char* name, float v)
{
  GLShaderProgram* glShader = (GLShaderProgram*)s;
  glUseProgramObject(glShader->GetGLHandle());
  int l = glGetUniformLocation(glShader->GetGLHandle(), name);
  glUniform1f(l, v);
}

