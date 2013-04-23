// mayaExportCommand.cpp
#include "mayaExportCommand.h"

#include <maya/MArgList.h>
#include <maya/MBoundingBox.h>
#include <maya/MDagPath.h>
#include <maya/MDagPathArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MFileObject.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatMatrix.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MGlobal.h>
#include <maya/MImage.h>
#include <maya/MIntArray.h>
#include <maya/MMatrix.h>
#include <maya/MObject.h>
#include <maya/MObjectArray.h>
#include <maya/MPlugArray.h>
#include <maya/MPointArray.h>
#include <maya/MStatus.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MSyntax.h>

#include "mayaImportMesh.h"

MayaExportCommand::MayaExportCommand()
{
}

MayaExportCommand::~MayaExportCommand()
{
}

MStatus	MayaExportCommand::doIt( const MArgList& args )
{
	MStatus stat;

	std::string CombinedArgs;

  MStringArray optionList;
  args.asString(0).split(' ', optionList);

  for(unsigned int i = 0; i < args.length(); ++i )
  {
    if(optionList.length() >= 1)
    {
      char filename[256];
      strcpy(filename, optionList[0].asChar());
      
      MayaImportMesh* mesh = new MayaImportMesh();
      mesh->Import();
      mesh->Write(filename);
    }
  }

	return MS::kSuccess;

}

void* MayaExportCommand::creator()
{
	return new MayaExportCommand;
}


MSyntax MayaExportCommand::newSyntax()
{

    MSyntax syntax;

    return syntax;
}
