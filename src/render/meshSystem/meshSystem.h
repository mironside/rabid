/**
  @file meshSystem.h
  @author Christopher Olsen
  @date Mon Dec  6 02:36:24 2004


*/
#ifndef __MESH_SYSTEM__
#define __MESH_SYSTEM__

#include "public/meshSystem.h"
#include "libsn/hashMap.h"



class Mesh;
class DynamicMeshInstance;

class MeshSystem : public IMeshSystem
{
protected:
  snSHashMap<Mesh*> m_meshMap;
  Mesh* m_defaultMesh;



public:
  MeshSystem();
  virtual ~MeshSystem();

  void Initialize();
  void Release();

  IMesh* GetMesh(const char* meshName);
  void FreeMesh(IMesh* mesh);
  IMesh* DefaultMesh();
  void Print();
};


#endif

