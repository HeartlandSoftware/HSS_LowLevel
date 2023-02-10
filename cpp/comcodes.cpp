/**
 * comcodes.cpp
 *
 * Copyright 2006-2023 Heartland Software Solutions Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the license at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the LIcense is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
