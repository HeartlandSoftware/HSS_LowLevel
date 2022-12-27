#pragma once

#ifndef _HRESULT_DEFINED

#include <cstdint>
#include <limits.h>
#include <string>

typedef long HRESULT;

namespace ResultCodes
{
	static_assert(sizeof(HRESULT) >= 4, "HRESULT not big enough");

	/**
	 * The method has failed but the application state will be intact.
	 */
	static constexpr HRESULT ERROR_WARNING = 1L << ((CHAR_BIT * sizeof(HRESULT)) - 1);
	static constexpr HRESULT ERROR_FATAL_BIT = (1L << ((CHAR_BIT * sizeof(HRESULT)) - 2));
	/**
	 * The method has failed in a way that has broken the application state. Close immediately.
	 */
	static constexpr HRESULT ERROR_FATAL = ERROR_WARNING | ERROR_FATAL_BIT;

	//unused levels
	static constexpr HRESULT MIDDLE_SEVENTY = 0x70000L;

	//no error
	static constexpr HRESULT OK = 0L;
	static constexpr HRESULT NO_ERROR_FAIL = 1L;

	//unknown errors
	static constexpr HRESULT GENERIC_FAILURE = 0x4005L;
	static constexpr HRESULT NOT_SUPPORTED = 0x32L;
	static constexpr HRESULT INVALID_TIME = 0x76DL;
	static constexpr HRESULT INVALID_STATE = 0x139FL;
	static constexpr HRESULT INVALID_POINTER = 0xFA3L;
	static constexpr HRESULT UNEXPECTED = 0xFFFFL;

	//file errors
	static constexpr HRESULT FILE_NOT_FOUND = 0x2L;
	static constexpr HRESULT TOO_MANY_FILES_OPEN = 0x4L;
	static constexpr HRESULT ACCESS_DENIED = 0x5L;
	static constexpr HRESULT INVALID_HANDLE = 0x6L;
	static constexpr HRESULT BAD_FORMAT = 0xBL;
	static constexpr HRESULT INVALID_DATA = 0xDL;
    static constexpr HRESULT INVALID_ACCESS = 0xCL;
	static constexpr HRESULT READ_FAULT = 0x1EL;
	static constexpr HRESULT DISK_FULL = 0x27L;
	static constexpr HRESULT FILE_EXISTS = 0x50;
	static constexpr HRESULT BAD_FILE_TYPE = 0xDEL;
	static constexpr HRESULT NO__DATA = 0xE8L;		// not sure why the double underscore works, but a single underscore doesn't
	static constexpr HRESULT INVALID_INDEX = 0x585L;
	
	//memory errors
	static constexpr HRESULT NOT_ENOUGH_MEMORY = 0x8L;
	static constexpr HRESULT OUT_OF_MEMORY = 0xEL;

	//usage errors
	static constexpr HRESULT INVALID_PARAMETER = 0x57L;
	static constexpr HRESULT INVALID_ARG = 0x111A9L;
	static constexpr HRESULT NOT_IMPLEMENTED = 0xFA1L;
	static constexpr HRESULT NO_INTERFACE = 0xFA2L;
	static constexpr HRESULT INVALID_PASSWORD = 86L;
	static constexpr HRESULT ERROR_INVALID_DATATYPE = 1804L;

    //MSI errors
    static constexpr HRESULT UNKNOWN_PROPERTY = 0x648L;

	static constexpr HRESULT ERROR_NETWORK_UNREACHABLE = 1231L;

    static constexpr HRESULT NO_ACCESS = 0x3E6L;

	/**
	 * Get an English description of the error.
	 */
	std::string ErrorDescription(HRESULT error);

	/**
	 * Build a return code.
	 * @param severity If non-zero indicates the severity of the error that occured (ResultCodes::ERROR_FATAL or ResultCodes::ERROR_WARNING).
	 * @param code The error code.
	 * @returns A result code that indicates success/failure.
	 */
	constexpr HRESULT ReturnValue(HRESULT severity, HRESULT code) { return severity | code; }

	/**
	 * Build a return code with the middle bits turned on.
	 * @param severity If non-zero indicates the severity of the error that occured (ResultCodes::ERROR_FATAL or ResultCodes::ERROR_WARNING).
	 * @param code The error code.
	 * @returns A result code that indicates success/failure.
	 */
	constexpr HRESULT ReturnSeventyCode(HRESULT severity, HRESULT code) { return severity | MIDDLE_SEVENTY | code; }

	/**
	 * Did the method return success.
	 */
	constexpr bool MethodSuccess(HRESULT result) { return ((result) >= 0); }

	/**
	 * Did the method fail.
	 */
	constexpr bool MethodFailed(HRESULT result) { return ((result) < 0); }

	/**
	 * Did the method fail fataly.
	 */
	constexpr bool MethodFatal(HRESULT result) { return ((result) < 0) && (result | ERROR_FATAL_BIT); }
}

template <class T> bool SUCCEEDED(T x) {
	static_assert(std::is_same<HRESULT, T>::value, "SUCCEEDED isn't passed HRESULT");
	return ResultCodes::MethodSuccess(x);
}
template <class T> bool FAILED(T x) {
	static_assert(std::is_same<HRESULT, T>::value, "FAILED isn't passed HRESULT");
	return ResultCodes::MethodFailed(x);
}

//macros for backwards compatibility
#ifndef _SKIP_HRESULT_OLD
#define S_OK ResultCodes::OK
#define S_FALSE ResultCodes::NO_ERROR_FAIL
#define E_FAIL ResultCodes::ReturnValue(ResultCodes::GENERIC_FAILURE, ResultCodes::ERROR_WARNING)
#define ERROR_SEVERITY_WARNING ResultCodes::ERROR_WARNING
#define ERROR_SEVERITY_ERROR ResultCodes::ERROR_FATAL
#define ERROR_FILE_NOT_FOUND ResultCodes::FILE_NOT_FOUND
#define ERROR_FILE_EXISTS ResultCodes::FILE_EXISTS
#define ERROR_TOO_MANY_OPEN_FILES ResultCodes::TOO_MANY_FILES_OPEN
#define ERROR_ACCESS_DENIED ResultCodes::ACCESS_DENIED
#define ERROR_INVALID_HANDLE ResultCodes::INVALID_HANDLE
#define ERROR_NOT_ENOUGH_MEMORY ResultCodes::NOT_ENOUGH_MEMORY
#define ERROR_INVALID_DATA ResultCodes::INVALID_DATA
#define ERROR_INVALID_ACCESS ResultCodes::INVALID_ACCESS
#define ERROR_READ_FAULT ResultCodes::READ_FAULT
#define ERROR_HANDLE_DISK_FULL ResultCodes::DISK_FULL
#define ERROR_NOT_SUPPORTED ResultCodes::NOT_SUPPORTED
#define ERROR_INVALID_PARAMETER ResultCodes::INVALID_PARAMETER
#define ERROR_BAD_FILE_TYPE ResultCodes::BAD_FILE_TYPE
#define ERROR_NO_DATA ResultCodes::NO__DATA
#define ERROR_INVALID_INDEX ResultCodes::INVALID_INDEX
#define ERROR_INVALID_TIME ResultCodes::INVALID_TIME
#define ERROR_INVALID_STATE ResultCodes::INVALID_STATE
#define ERROR_INVALID_PASSWORD ResultCodes::INVALID_PASSWORD
#define ERROR_UNKNOWN_PROPERTY ResultCodes::UNKNOWN_PROPERTY
#define ERROR_NOACCESS ResultCodes::NO_ACCESS
#define E_INVALIDARG ResultCodes::ReturnValue(ResultCodes::ERROR_WARNING, ResultCodes::INVALID_ARG)
#define E_NOTIMPL ResultCodes::ReturnValue(ResultCodes::ERROR_WARNING, ResultCodes::NOT_IMPLEMENTED)
#define E_NOINTERFACE ResultCodes::ReturnValue(ResultCodes::ERROR_WARNING, ResultCodes::NO_INTERFACE)
#define E_POINTER ResultCodes::ReturnValue(ResultCodes::ERROR_WARNING, ResultCodes::INVALID_POINTER)
#define E_ACCESSDENIED ResultCodes::ReturnSeventyCode(ResultCodes::ERROR_WARNING, ResultCodes::ACCESS_DENIED)
#define E_OUTOFMEMORY ResultCodes::ReturnSeventyCode(ResultCodes::ERROR_WARNING, ResultCodes::OUT_OF_MEMORY)
#define E_UNEXPECTED ResultCodes::ReturnValue(ResultCodes::ERROR_WARNING, ResultCodes::UNEXPECTED)
#define ERROR_NETWORK_UNREACHABLE ResultCodes::ERROR_NETWORK_UNREACHABLE
#endif

//don't redefine
#define _HRESULT_DEFINED
#define _WINERROR_

#endif