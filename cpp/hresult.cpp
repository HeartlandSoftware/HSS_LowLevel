#include "intel_check.h"
#include "hresult.h"

#include <map>
#include <stdexcept>

namespace ResultCodes
{
	static const std::map<HRESULT, std::string> errorCodes = {
		{ OK, "Success" },
		{ GENERIC_FAILURE, "Unspecified error" },
		{ NOT_SUPPORTED, "The request is not supported." },
		{ INVALID_TIME, "The specified time is invalid." },
		{ INVALID_STATE, "The group or resource is not in the correct state to perform the requested operation." },
		{ INVALID_POINTER, "Invalid pointer" },
		{ UNEXPECTED, "Catastrophic failure" },
		{ FILE_NOT_FOUND, "The system cannot find the file specified." },
		{ TOO_MANY_FILES_OPEN, "The system cannot open the file." },
		{ ACCESS_DENIED, "Access is denied." },
		{ INVALID_HANDLE, "The handle is invalid." },
		{ INVALID_DATA, "The data is invalid." },
		{ READ_FAULT, "The system cannot read from the specified device." },
		{ DISK_FULL, "The disk is full." },
		{ BAD_FILE_TYPE, "The file type being saved or retrieved has been blocked." },
		{ NO__DATA, "The pipe is being closed." },
		{ INVALID_INDEX, "Invalid index." },
		{ NOT_ENOUGH_MEMORY, "Not enough memory resources are available to process this command." },
		{ OUT_OF_MEMORY, "Ran out of memory." },
		{ INVALID_PARAMETER, "The parameter is incorrect." },
		{ INVALID_ARG, "One or more arguments are invalid." },
		{ NOT_IMPLEMENTED, "Not implemented." },
		{ NO_INTERFACE, "No such interface supported" }
	};

	std::string ErrorDescription(HRESULT error)
	{
		try
		{
			return errorCodes.at(error & 0xffff);
		}
		catch (std::out_of_range& e)
		{
			return "Unknown error.";
		}
	}
}