/*	TSTRING.CPP
//
//	Started:	January 27, 2013
//	Last Modified:	January 27, 2013
//	Version 1.0
//
//	Purpose: to simply extract unicode away from std::string vs. std::wstring
*/

#include "intel_check.h"
#include "tstring.h"

#include <cstdio>
#include <cstdarg>
#include <cstdlib>

#ifdef UNICODE

std::string toStdString(const tstring &toConvert) {
	std::string converted(toConvert.begin(), toConvert.end());
	return converted;
}


std::string toStdString(const TCHAR *toConvert) {
	return toStdString(tstring(toConvert));
}


tstring toTString(const std::string &toConvert) {
	tstring converted(toConvert.begin(), toConvert.end());
	return converted;
}


tstring toTString(const char *toConvert) {
	return toTString(std::string(toConvert));
}


#else

std::string toStdString(const tstring &toConvert) {
	return toConvert;
}


std::string toStdString(const TCHAR *toConvert) {
	return std::string(toConvert);
}


tstring toTString(const std::string &toConvert) {
	return toConvert;
}


tstring toTString(const char *toConvert) {
	return tstring(toConvert);
}

#endif
