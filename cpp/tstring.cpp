/**
 * tstring.h
 *
 * Copyright 2013-2023 Heartland Software Solutions Inc.
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
