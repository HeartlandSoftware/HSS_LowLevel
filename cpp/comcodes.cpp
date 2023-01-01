//	ComCodes.cpp
//
//	Started:	March 24, 2006
//	Last Modified:	September 19, 2006
//	By Robert W. Bryce

#include "hresult.h"
#include <errno.h>
#ifdef _MSC_VER
#include <winerror.h>
#endif
#include "comcodes.h"


HRESULT ComError(int _errno_) {
	switch (_errno_) {
		case EACCES:	return ERROR_ACCESS_DENIED | ERROR_SEVERITY_WARNING;
		case EBADF:		return ERROR_INVALID_HANDLE | ERROR_SEVERITY_WARNING;
		case EEXIST:	return ERROR_FILE_EXISTS | ERROR_SEVERITY_WARNING;
		case EINVAL:	return ERROR_INVALID_PARAMETER | ERROR_SEVERITY_WARNING;
		case EMFILE:	return ERROR_TOO_MANY_OPEN_FILES | ERROR_SEVERITY_WARNING;
		case ENOENT:	return ERROR_FILE_NOT_FOUND | ERROR_SEVERITY_WARNING;
		case ENOMEM:	return E_OUTOFMEMORY;
		case ENOSPC:	return ERROR_HANDLE_DISK_FULL | ERROR_SEVERITY_WARNING;
	}
	return ERROR_SEVERITY_WARNING;			// failure but not exactly sure why...
}
