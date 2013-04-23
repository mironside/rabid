// mayaFileTranslator.cpp
#include "MayaFileTranslator.h"
#include "mayaImportMesh.h"

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
#include <maya/MPxCommand.h>
#include <maya/MPxNode.h>
#include <maya/MPxFileTranslator.h>



MayaFileTranslator::MayaFileTranslator(void)
{
}

MayaFileTranslator::~MayaFileTranslator(void)
{
}


MStatus	MayaFileTranslator::writer( const MFileObject& file,
				   const MString& options,
				   FileAccessMode mode )
{
	MStatus     	stat;
	MString     	arg;
	MString			fileName;
	MString			boolean;

	if( mode == MPxFileTranslator::kExportActiveAccessMode )
	{
		std::cerr << "As yet this does not support export selected\nExporting all instead\n";
	}

	std::cout << options.asChar() << std::endl;

	return MS::kSuccess;
}

