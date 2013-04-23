// mayaFileTranslator.h

//-----------------------------------------------------------------------
///	\file    	MayaFileTranslator.h
///	\date   	24/4/2004   14:55
///	\author		Rob Bateman
///	\brief      Defines the main File translator class. This contains 
///				the read and write methods for the file translator.
/// \note       
///
//-----------------------------------------------------------------------

#ifndef __MayaFileTranslator__H__
#define __MayaFileTranslator__H__

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

#include <maya/MFileIO.h>
#include <maya/MFileObject.h>
#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MString.h>
#include <maya/MPxFileTranslator.h>


class MayaFileTranslator :
	public MPxFileTranslator
{
public:

	MayaFileTranslator(void);
	~MayaFileTranslator(void);
	MStatus		writer( const MFileObject& file,
						const MString& optionsString,
						FileAccessMode mode );

  bool         haveWriteMethod()  const { return true; }
  MString      defaultExtension() const { return ""; }
  static void* creator()                { return new MayaFileTranslator; }

private:

};

#endif



