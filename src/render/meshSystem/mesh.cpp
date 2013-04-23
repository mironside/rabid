// mesh.cpp
#include "mesh.h"
#include "render/renderDevice.h"
#include "render/renderer.h"
#include "common/log.h"
#include "public/fileSystem.h"
#include "public/cvarSystem.h"
#include "public/render.h"
#include "common/common.h"
#include "common/profile.h"
#include "render/vertexBuffer.h"
#include "render/indexBuffer.h"
#include "render/hardwareBufferMgr.h"
#include "public/render.h"
#include <stdio.h>
#include <assert.h>
#include "common/global.h"

extern IRenderer* g_renderer;
extern IRenderDevice* g_renderDevice;


CVar r_drawmeshes("r_drawmeshes", "1");
CVar r_drawtangents("r_drawtangents", "0");
CVar r_drawweights("r_drawweights", "-1");


// overflow test macro
#define INCR_PTR(ptr, size, end) \
           ptr += size; \
           if(ptr > end) \
           { \
             LOG(LOG_RED, \
                 "ERROR %s %d: Exceeded buffer length while reading file", \
                 __FILE__, __LINE__); \
             return 0; \
           }

#define COPY_PTR(addr, ptr, size, end) \
           memcpy(addr, ptr, size); \
           INCR_PTR(ptr, size, end);

#define MC_INDEX_ARRAY  1
#define MC_VERTEX_ARRAY 2
#define MC_WEIGHT_ARRAY 3


typedef struct weight_s
{
  unsigned int vertex;
  int   bone;
  float weight;
} weight_t;



Mesh::Mesh() :
  vb(0), ib(0), nVertices(0), nIndices(0), format(0),
  vbSize(0), vbdata(0), nWeights(0), weights(0)
{
}



Mesh::~Mesh()
{
  delete[] indexArray;
  delete[] vbdata;
  delete[] vbTransformed;
  delete[] weights;
}



int Mesh::LoadFile(const char* filename)
{
  LOG(0, "Mesh::LoadFile '%s'", filename);
  FileHandle file = g_fileSystem->Open((char*)filename);
  if(file < 0)
    return 0;

  int length = g_fileSystem->GetSize(file);

  unsigned char* buffer = (unsigned char*)g_fileSystem->GetData(file);
  unsigned char* ptr = buffer;
  unsigned char* end = buffer + length;

  // magic
  /// @todo CONVERT CMSH to int for test
  unsigned int magic = (('H' << 24) | ('S' << 16) | ('M' << 8) | 'C');
  if(*(unsigned int*)ptr != magic)
  {
    // close file
    LOG(0, "ERROR: '%s' not a Catharsis mesh", filename);
    return 0;
  }
  INCR_PTR(ptr, sizeof(int), end);


  // version
  //unsigned int version = *(unsigned int*)ptr;
  INCR_PTR(ptr, sizeof(int), end);


  // nChunks
  unsigned int nChunks = *(unsigned int*)ptr;
  INCR_PTR(ptr, sizeof(int), end);


  // Chunks
  unsigned int type = 0;
  unsigned int size = 0;
  for(unsigned int i = 0; i < nChunks; i++)
  {
    INCR_PTR(ptr, size, end);
    type = *(unsigned int*)ptr;
    INCR_PTR(ptr, sizeof(unsigned int), end);
    size = *(unsigned int*)ptr;
    INCR_PTR(ptr, sizeof(unsigned int), end);

    switch(type)
    {
    case MC_VERTEX_ARRAY:
      if(!ReadVertexArray(ptr, end))
        return 0;
      break;

    case MC_INDEX_ARRAY:
      if(!ReadIndexArray(ptr, end))
        return 0;
      break;

    case MC_WEIGHT_ARRAY:
      if(!ReadWeightArray(ptr, end))
        return 0;
      break;

    default:
      LOG(0, "### WARNING [Mesh]: " \
             "Unrecognized chunk (0x%X) found when loading mesh '%s'",
             type, filename);
      break;
    }
  }

  g_fileSystem->Close(file);

  return 0;
}



int Mesh::ReadIndexArray(unsigned char* ptr, unsigned char* end)
{
  // index array
  nIndices = *(unsigned int*)ptr;
  INCR_PTR(ptr, sizeof(unsigned int), end);

  LOG(0, "IB = %d bytes", nIndices * sizeof(unsigned int));
  ib = g_renderDevice->CreateIndexBuffer(IBF_UINT, nIndices);
  unsigned char* buf = (unsigned char*)ib->Lock(0);
  if(buf)
    memcpy(buf, ptr, sizeof(unsigned int) * nIndices);
  ib->Unlock();
  INCR_PTR(ptr, nIndices * sizeof(int), end);

  return 1;
}



int Mesh::ReadVertexArray(unsigned char* ptr, unsigned char* end)
{
  // vertex array
  format = *(unsigned int*)ptr;
  INCR_PTR(ptr, sizeof(int), end);
  nVertices = *(unsigned int*)ptr;
  INCR_PTR(ptr, sizeof(int), end);

  vertexSize = 0;
  unsigned int i = 0;
  unsigned int fmt = 0;
  if(format & R_VB_XYZ)
    fmt |= VF_XYZ;
  if(format & R_VB_COLOR)
    fmt |= VF_COLOR;
  if(format & R_VB_NORMAL)
    fmt |= VF_NORMAL;

  if(format & R_VB_TC0)
  {
    fmt |= VF_TC2(i);
    i++;
  }

  if(format & R_VB_TANGENT)
  {
    fmt |= VF_TC3(i);
    i++;
  }
  if(format & R_VB_BITANGENT)
  {
    fmt |= VF_TC3(i);
    i++;
  }

  // number of texstages flag
  fmt |= (i << 8);

  vertexSize     = VertexSize(fmt);
  vertexOffset   = VertexElementOffset(fmt, VE_XYZ);
  normalOffset   = VertexElementOffset(fmt, VE_NORMAL);
  texCoordOffset = VertexElementOffset(fmt, VE_TEXCOORD0);
  tangentOffset  = VertexElementOffset(fmt, VE_TEXCOORD1);
  binormalOffset = VertexElementOffset(fmt, VE_TEXCOORD2);


  vbSize = vertexSize * nVertices;
  vbdata = new unsigned char[vbSize];
  memcpy(vbdata, ptr, vbSize);

  vbTransformed = new unsigned char[vbSize];
  memcpy(vbTransformed, vbdata, vbSize);
  vb = g_renderDevice->CreateVertexBuffer(fmt, nVertices, 0);
  unsigned char* buf = (unsigned char*)vb->Lock(0);
  if(buf)
    memcpy(buf, vbdata, vbSize);
  vb->Unlock();
  return 1;
}



int Mesh::ReadWeightArray(unsigned char* ptr, unsigned char* end)
{
  nWeights = *(unsigned int*)ptr;
  INCR_PTR(ptr, sizeof(unsigned int), end);

  weights = new weight_t[nWeights];
  memcpy(weights, ptr, nWeights*3*sizeof(int));
  INCR_PTR(ptr, (nWeights)*3*sizeof(int), end);

  FILE* file = fopen("weights.txt", "w");
  fprintf(file, "%d weights\n\n", nWeights);
  for(unsigned int i = 0; i < nWeights; i++)
  {
    fprintf(file, "%d %d %f %d\n", weights[i].vertex, weights[i].bone,
                                   weights[i].weight, i);
  }
  fclose(file);
  return 1;
}



void Mesh::Draw()
{
  if(!vb || !ib)
    return;

  if(r_drawmeshes.GetBool())
    vb->Render(R_TRIANGLES, ib->GetNumIndices(), 0, ib);

  if(r_drawtangents.GetBool())
    DrawTangents();

  if(r_drawweights.GetInt() >= 0)
    DrawWeights(r_drawweights.GetInt());
}



void Mesh::DrawTangents()
{
  if(!(format & R_VB_TANGENT && format & R_VB_BITANGENT))
    return;

  g_renderer->BindMaterial(0);


  float* d = 0;
  for(unsigned int i = 0; i < nVertices; i++)
  {
    unsigned char* tvData = &vbTransformed[i * vertexSize];

    float* v = (float*)&tvData[vertexOffset];

    d = (float*)&tvData[tangentOffset];
    g_renderDevice->SetColor(1,0,0);
    g_renderDevice->DrawLine(v[0], v[1], v[2],
                             v[0] + d[0], v[1] + d[1], v[2] + d[2]);

    d = (float*)&tvData[binormalOffset];
    g_renderDevice->SetColor(0,1,0);
    g_renderDevice->DrawLine(v[0], v[1], v[2],
                             v[0] + d[0], v[1] + d[1], v[2] + d[2]);

    d = (float*)&tvData[normalOffset];
    g_renderDevice->SetColor(0,0,1);
    g_renderDevice->DrawLine(v[0], v[1], v[2],
                             v[0] + d[0], v[1] + d[1], v[2] + d[2]);
  }

  g_renderDevice->SetColor(1,1,1);
}



void Mesh::DrawWeights(int bone)
{
  if(!weights)
    return;

  for(unsigned int i = 0; i < nWeights; i++)
  {
    if(weights[i].bone == bone)
    {
      float* v = (float*)&vbTransformed[weights[i].vertex * vertexSize];
      g_renderer->SetColor(weights[i].weight, weights[i].weight, weights[i].weight);
      g_renderer->DrawPoint(5.0, v[0], v[1], v[2]);
    }
  }
}



void Mesh::ApplyTransformations(Transformation* joints, unsigned int nJoints)
{
  PROFILE("mesh_transform");

  if(!weights)
    return;

  // transform data into transformation buffer
  memset(vbTransformed, 0, vbSize);

  //int idx = 0;
  //unsigned int offset = 0;
  float* d = 0;
  float* td = 0;
  Vector3f p;
  for(unsigned int i = 0; i < nWeights; i++)
  {
    unsigned char* vData  = &vbdata[weights[i].vertex * vertexSize];
    unsigned char* tvData = &vbTransformed[weights[i].vertex * vertexSize];

    // NOTE: the idea here is that d is the bind data and td is the
    //       transformed data for a vertex.  Set d and td to the
    //       appropriate places in the bind vertex buffer and the
    //       transformed vertex buffer then operate on those data.



    // vertex
    d  = (float*)&vData[vertexOffset];
    td = (float*)&tvData[vertexOffset];

    p = Vector3f(d[0], d[1], d[2]);
    p = (joints[weights[i].bone] * p) * weights[i].weight;

    td[0] += p.x;
    td[1] += p.y;
    td[2] += p.z;



    // normal
    d  = (float*)&vData[normalOffset];
    td = (float*)&tvData[normalOffset];

    p = Vector3f(d[0], d[1], d[2]);
    p = (joints[weights[i].bone].Rotate() * p) * weights[i].weight;

    td[0] += p.x;
    td[1] += p.y;
    td[2] += p.z;



    // texcoord
    d  = (float*)&vData[texCoordOffset];
    td = (float*)&tvData[texCoordOffset];

    td[0] = d[0];
    td[1] = d[1];



    // tangent
    d  = (float*)&vData[tangentOffset];
    td = (float*)&tvData[tangentOffset];

    p = Vector3f(d[0], d[1], d[2]);
    p = (joints[weights[i].bone].Rotate() * p) * weights[i].weight;

    td[0] += p.x;
    td[1] += p.y;
    td[2] += p.z;



    // binormal
    d  = (float*)&vData[binormalOffset];
    td = (float*)&tvData[binormalOffset];

    p = Vector3f(d[0], d[1], d[2]);
    p = (joints[weights[i].bone].Rotate() * p) * weights[i].weight;

    td[0] += p.x;
    td[1] += p.y;
    td[2] += p.z;
  }

  unsigned char* buf = (unsigned char*)vb->Lock(0);
  memcpy(buf, vbTransformed, vbSize);
  vb->Unlock();
}



void Mesh::SetVB(VertexBuffer* _vb)
{
  if(vb)
    delete vb;
  vb = _vb;
}



void Mesh::SetIB(IndexBuffer* _ib)
{
  if(ib)
    delete ib;
  ib = _ib;
}

