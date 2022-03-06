//	exprtopt.h
//
//	By Robert W. Bryce
//	Started:	May 18, 1996
//	Last Modified:	March 16, 2001


#ifndef __EXPORTOPT_H
#define __EXPORTOPT_H                              

#include "types.h"
#include "hssconfig/config.h"
#include <string>

#if defined(_MSC_VER) && !defined(__INTEL_COMPILER) && !defined(__INTEL_LLVM_COMPILER)
#pragma managed(push, off)
#endif
#ifdef HSS_SHOULD_PRAGMA_PACK
#pragma pack(push, 8)
#endif

// if the string returned is 1 character in length, and is of hex value 1,
// then we are supposed to do column-delimited output


class ExportOptions {
protected:
	std::string Delimiter;			// what we store the filename extension in

    public:
	ExportOptions(const TCHAR *group_name);	// gets what we need to know from the app's INI file

	 void __cdecl SaveToIniFile(const TCHAR *group_name) const;
						// saves info back out to the ini file
	void __cdecl SetExportDelimiter(const TCHAR *delimit);
						// sets the delimiter between columns of data
	BOOL __cdecl ExportColumnAligned() const;	// if we should pad things out with spaces...
	const TCHAR * __cdecl GetExportDelimiter() const;	// gets the delimiter between columns of data

	const TCHAR * __cdecl GetImportFilter() const;	// gets a filter for the file dialog
	const TCHAR * __cdecl GetExportFilter() const;	// gets a filter for the file dialog
	const TCHAR * __cdecl GetExportExt(bool choice) const;	// gets the filename extension
};

#ifdef HSS_SHOULD_PRAGMA_PACK
#pragma pack(pop)
#endif
#if defined(_MSC_VER) && !defined(__INTEL_COMPILER) && !defined(__INTEL_LLVM_COMPILER)
#pragma managed(pop)
#endif

#endif 
