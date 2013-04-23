// mayaMain.cpp

//some maya crap
#define _BOOL

#include "mayaExportCommand.h"
#include "mayaFileTranslator.h"
#include <maya/MStatus.h>
#include <maya/MObject.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <maya/MSyntax.h>

#ifdef WIN32
	#define MLL_EXPORT __declspec(dllexport) 
#else
	#define MLL_EXPORT
#endif

/// specifies a script to use for the user interface options box
char* g_OptionScript = "";

/// a set of default options for the exporter
char* g_DefaultOptions = "blah";


MLL_EXPORT MStatus initializePlugin( MObject obj )
{
  MFnPlugin plugin(obj, "Christopher Olsen", "1.0", "Any");
  
  MStatus status;

  status  = plugin.registerCommand("RabidExport",
                                   MayaExportCommand::creator,
                                   MayaExportCommand::newSyntax);

  if(!status)
  {
    MessageBox(0, "OH SNAP!", "snap", MB_OK);
		status.perror("MayaExportCommand::registerCommand");
		return status;
	}

  return status;
}



MLL_EXPORT MStatus uninitializePlugin( MObject obj )
{
	MFnPlugin plugin( obj );
	MStatus status ;

  status = plugin.deregisterCommand("RabidExport");
	if(!status)
  {
		status.perror("MayaExportCommand::deregisterCommand");
		return status;
	}

  return status;
}

