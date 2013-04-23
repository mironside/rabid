// mayaExportCommand.h

#ifndef __MayaExportCommand__H__
#define __MayaExportCommand__H__

#ifdef WIN32
	#ifndef NT_PLUGIN
		#define NT_PLUGIN
	#endif

	#ifdef VISUALC_6
		
		// disable VC6 template name warning
		#pragma warning(disable:4786)

		// scope for loops under an if statement for visual C 6
		#ifndef FOR__
			#define for if(1)
		#endif
			
	#endif
#endif

#include <maya/MPxCommand.h>


class MArgList;

class MayaExportCommand : public MPxCommand
{
public:
	MayaExportCommand(void);
	virtual ~MayaExportCommand(void);	
	virtual MStatus	doIt(const MArgList&);
	virtual bool    isUndoable() const {	return false; }
	static  void*   creator();
  static MSyntax newSyntax();
};

#endif
