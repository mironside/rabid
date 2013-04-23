// mayaImportMesh.h
#ifndef __MAYA_IMPORT_MESH__
#define __MAYA_IMPORT_MESH__

#include <vector>

class MayaImportMesh
{
protected:
  char name[64];
  std::vector<struct vert_s*> vertices;

public:
  MayaImportMesh();
  virtual ~MayaImportMesh();

  int Import();
  void ExtractVertices();
  void Write(const char* filename);
};

#endif

