/**
 * WinReplacement.h
 *
 * Copyright 2018-2023 Heartland Software Solutions Inc.
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

#pragma once

#include "types.h"
#include <chrono>
#include <string>

#ifdef _MSC_VER
#ifdef _NO_MFC
#include <windows.h>
#else
#include <afx.h>
#endif
#else

#include <cstdint>
#include <iostream>
#include "filesystem.hpp"

#ifndef VARIANT_TRUE
typedef short VARIANT_BOOL;

#define VARIANT_TRUE ((VARIANT_BOOL)-1)
#define VARIANT_FALSE ((VARIANT_BOOL)0)
#endif

#define THREAD_PRIORITY_LOWEST       -2
#define THREAD_PRIORITY_BELOW_NORMAL -1
#define THREAD_PRIORITY_NORMAL        0
#define THREAD_PRIORITY_HIGHEST       1
#define THREAD_PRIORITY_ABOVE_NORMAL  2


#define __debugbreak raise(SIGTRAP)


inline std::uint32_t GetTickCount()
{
	using namespace std::chrono;
	return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

inline bool GetDiskFreeSpace(std::string* rootPathName, unsigned long* sectorsPerCluster,
	unsigned long* bytesPerSector, unsigned long* freeClusters, unsigned long* totalNumberOfClusters)
{
	std::string toFind;
	if (rootPathName == nullptr || rootPathName->length() == 0)
		toFind = "C:\\";
	else
		toFind = *rootPathName;
	fs::space_info info = fs::space(toFind);
	if (totalNumberOfClusters != nullptr)
		*totalNumberOfClusters = info.capacity / 4096;
	if (freeClusters != nullptr)
		*freeClusters = info.free / 4096;
	if (bytesPerSector != nullptr)
		*bytesPerSector = 512;
	if (sectorsPerCluster != nullptr)
		*sectorsPerCluster = 8;
	return true;
}
#endif
