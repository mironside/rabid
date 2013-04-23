// mayaImportMesh.cpp
#define _BOOL
#include <maya/MStatus.h>
#include <maya/MString.h> 
#include <maya/MFileIO.h>
#include <maya/MLibrary.h>
#include <maya/MItDag.h>
#include <maya/MDagPath.h>
#include <maya/MGlobal.h>
#include <maya/MPointArray.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnMesh.h>
#include <maya/Mpoint.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MFnAnimCurve.h>
#include <maya/MAnimControl.h>
#include <maya/MItDependencyGraph.h>
#include <maya/MTime.h>
#include <maya/MFnIkJoint.h>
#include <maya/MFnTransform.h>
#include <maya/MEulerRotation.h>
#include <maya/MMatrix.h>
#include <maya/MQuaternion.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MFnSkinCluster.h>
#include <maya/MDagPathArray.h>
#include <maya/MItGeometry.h>
#include <maya/MItKeyframe.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFloatMatrix.h>
#include <maya/MSelectionList.h>
#include <maya/MGlobal.h>
#include <maya/MItSelectionList.h>
#include <vector>
#include <math.h>
#include "mayaImportMesh.h"
#include "math/vector3.h"
MStringArray  UVSets;

typedef struct vert_s
{
  float v[3];
  float tc[2];
} vert_t;

MayaImportMesh::MayaImportMesh()
{
}



MayaImportMesh::~MayaImportMesh()
{
}


int MayaImportMesh::Import()
{
  ExtractVertices();
  return true;
}



void MayaImportMesh::ExtractVertices()
{
  MStatus stat;
  MAnimControl::setCurrentTime(MTime(0, MTime::kSeconds));

  MSelectionList SelList;
  MGlobal::getActiveSelectionList(SelList);

  MItSelectionList iter(SelList, MFn::kMesh);
  for(; !iter.isDone(); iter.next())
  {
    MDagPath dagPath;
    iter.getDagPath(dagPath);
    MFnDagNode dagNode( dagPath, &stat );

    // this object cannot be intermediate, and it must be a mesh
    // and it can't be a transform.
    // Intermediate objects are special meshes
    // that are not drawn used for mesh morphs or something.
    if ( dagNode.isIntermediateObject()) continue;
    if ( !dagPath.hasFn( MFn::kMesh )) continue;
    if ( dagPath.hasFn( MFn::kTransform )) continue;



    MFnMesh fnMesh(dagPath);
    MPointArray vertexList;
    fnMesh.getPoints(vertexList, MSpace::kWorld);

    MStringArray  UVSets;
    MFloatArray   u, v;
    fnMesh.getUVSetNames( UVSets );
    fnMesh.getUVs(u, v);


    int idx = 0;
    MItMeshPolygon piter(dagPath);
    for(int tri = 0; !piter.isDone(); piter.next(), tri++)
    {
      MIntArray vertexIdxs;
      piter.getVertices(vertexIdxs);

      if(vertexIdxs.length() != 4)
      {
        // error, not a quad, SKIP!
        continue;
      }

      for(unsigned int i=0; i < 4; i++)
      {
        int uv = 0;
        piter.getUVIndex(i, uv);

        vert_t* vert = new vert_t;
        vert->v[0] = vertexList[vertexIdxs[i]].cartesianize()[0];
        vert->v[1] = vertexList[vertexIdxs[i]].cartesianize()[1];
        vert->v[2] = vertexList[vertexIdxs[i]].cartesianize()[2];
        vert->tc[0] = v[uv];
        vert->tc[1] = u[uv];
        vertices.push_back(vert);
      }
    }
  }
}








void MayaImportMesh::Write(const char* filename)
{
  char fn[256];
  memset(fn, 0, 256);
  strcpy(fn, filename);
  FILE* file = fopen(fn, "wb");

  char* name = fn;
  for(int i = 0; i < strlen(fn); i++)
  {
    if(fn[i] == '/')
      name = &fn[i+1];
  }
  for(int i = 0; i < strlen(fn); i++)
  {
    if(fn[i] == '.')
      fn[i] = 0;
  }

  fprintf(file, "<map %s>\n{\n", name);

  for(unsigned int i = 0; i < vertices.size(); i+=4)
  {
    fprintf(file, "  [surface]\n"
                  "  {\n"
                  "    vertices = [\n"
                  "      <%f, %f, %f>,\n"
                  "      <%f, %f, %f>,\n"
                  "      <%f, %f, %f>,\n"
                  "      <%f, %f, %f>\n"
                  "    ];\n"
                  "\n"
                  "    texcoords = [\n"
                  "      <%f, %f>,\n"
                  "      <%f, %f>,\n"
                  "      <%f, %f>,\n"
                  "      <%f, %f>\n"
                  "    ];\n"
                  "  }\n\n\n",
                  vertices[i+3]->v[0], vertices[i+3]->v[1], vertices[i+3]->v[2],
                  vertices[i+2]->v[0], vertices[i+2]->v[1], vertices[i+2]->v[2],
                  vertices[i+1]->v[0], vertices[i+1]->v[1], vertices[i+1]->v[2],
                  vertices[i+0]->v[0], vertices[i+0]->v[1], vertices[i+0]->v[2],
                  vertices[i+3]->tc[0], vertices[i+3]->tc[1],
                  vertices[i+2]->tc[0], vertices[i+2]->tc[1],
                  vertices[i+1]->tc[0], vertices[i+1]->tc[1],
                  vertices[i+0]->tc[0], vertices[i+0]->tc[1]
                  );

  }
  fprintf(file, "}\n");

  fclose(file);
}

