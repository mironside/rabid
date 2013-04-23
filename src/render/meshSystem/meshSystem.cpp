/**
  @file meshSystem.cpp
  @author Christopher Olsen
  @date Mon Dec  6 02:43:03 2004


*/

//#include <stdio.h>
#include "meshSystem.h"
#include "mesh.h"
#include "common/log.h"
#include "public/fileSystem.h"
#include "render/indexBuffer.h"
#include "render/vertexBuffer.h"
#include "render/renderer_defs.h"
#include <string.h>
#include <stdio.h>
#include "public/render.h"
#include "render/renderDevice.h"
#include "common/global.h"
extern IRenderDevice* g_renderDevice;

MeshSystem::MeshSystem()
{
}



MeshSystem::~MeshSystem()
{
  snVector<const char*> keys = m_meshMap.GetKeys();
  for(unsigned int i = 0; i < keys.Size(); i++)
  {
    delete m_meshMap[keys[i]];
    delete[] keys[i];
  }

//  delete m_defaultMesh;
  m_defaultMesh = 0;
}



void MeshSystem::Initialize()
{
  m_defaultMesh = new Mesh();

  float vbd[120] = {-5,  5,  5, -1,  1,  -5,  5, -5, -1, -1,    5,  5, -5,  1, -1,   5,  5, 5,  1, 1,
                   -5, -5,  5, -1,  1,  -5, -5, -5, -1, -1,    5, -5, -5,  1, -1,   5, -5, 5,  1, 1,
                   -5, -5,  5, -1, -1,  -5,  5,  5, -1,  1,    5,  5,  5,  1,  1,   5, -5,  5,  1,-1,
                   -5, -5, -5, -1, -1,  -5,  5, -5, -1,  1,    5,  5, -5,  1,  1,   5, -5, -5,  1,-1,
                   -5, -5, -5, -1, -1,  -5,  5, -5, -1,  1,   -5,  5,  5,  1,  1,  -5, -5,  5,  1,-1,
                    5, -5, -5, -1, -1,   5,  5, -5, -1,  1,    5,  5,  5,  1,  1,   5, -5,  5,  1,-1};

  unsigned int ibd[36] = {0,1,2,    0,2,3,    4,6,5,    4,7,6,
                         8,9,10,   8,10,11,  12,14,13, 12,15,14,
                         16,17,18, 16,18,19, 20,22,21, 20,23,22};

  VertexBuffer* vb = g_renderDevice->CreateVertexBuffer(VF_XYZ | VF_TEX1, 24, 0);
  unsigned char* buf = 0;
  buf = (unsigned char*)vb->Lock(0);
  memcpy(buf, vbd, sizeof(float) * 120);
  vb->Unlock();

  IndexBuffer* ib = g_renderDevice->CreateIndexBuffer(IBF_UINT, 36);
  buf = (unsigned char*)ib->Lock(0);
  memcpy(buf, ibd, sizeof(unsigned int) * 36);
  ib->Unlock();

  m_defaultMesh->SetVB(vb);
  m_defaultMesh->SetIB(ib);
}



void MeshSystem::Release()
{
  snVector<const char*> keys = m_meshMap.GetKeys();
  for(unsigned int i = 0; i < keys.Size(); i++)
  {
    delete m_meshMap[keys[i]];
    delete[] keys[i];
    m_meshMap.Remove(keys[i]);
  }
}



IMesh* MeshSystem::GetMesh(const char* meshName)
{
  FN(0, "MeshSystem::GetMesh");

  Mesh* m = 0;
  if(m_meshMap.Exists(meshName))
  {
    m = m_meshMap[meshName];
  }
  else
  {
    LOG(0, "MESH -- From File '%s'", meshName);
    char filename[256];
    sprintf(filename, "%s.mesh", meshName);
    if(g_fileSystem->Exists(filename))
    {
      m = new Mesh();
      m->LoadFile(filename);
    }
    else
    {
      m = (Mesh*)DefaultMesh();
    }

    char* n = new char[strlen(meshName)+1];
    memset(n, 0, strlen(meshName)+1);
    strcpy(n, meshName);
    m_meshMap[n] = m;
    return m;
  }

  return m_defaultMesh;
}


void MeshSystem::FreeMesh(IMesh* mesh)
{
}



IMesh* MeshSystem::DefaultMesh()
{
  return m_defaultMesh;
}


void MeshSystem::Print()
{
}

