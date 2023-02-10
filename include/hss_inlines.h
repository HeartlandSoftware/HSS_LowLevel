/**
 * hss_inlines.h
 *
 * Copyright 2021-2023 Heartland Software Solutions Inc.
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

#include <stdlib.h>
#include <string>
#include <algorithm>

namespace hss
{
	/// <summary>
	/// Remove null characters from a string.
	/// </summary>
	/// <param name="str">The string to remove null characters from.</param>
	/// <returns>The same string as passed but without \0.</returns>
	inline std::string denull(const std::string& str)
	{
		std::string ret = str;
		return denull(std::move(ret));
	}

	/// <summary>
	/// Remove null characters from a string.
	/// </summary>
	/// <param name="str">The string to remove null characters from.</param>
	/// <returns>The same string as passed but without \0.</returns>
	inline std::string denull(std::string&& str)
	{
		str.erase(std::remove(str.begin(), str.end(), '\0'), str.end());
		return str;
	}

	/// <summary>
	/// Get an environment variable as a string. On Windows uses getenv_s, everywhere else
	/// this just calls std::getenv.
	/// </summary>
	/// <param name="varname">The name of the environment variable to get.</param>
	/// <returns>The value stored in the environment variable.</returns>
	inline std::string getenv(std::string_view varname)
	{
#ifdef _MSC_VER
		size_t size;
		getenv_s(&size, NULL, 0, varname.data());
		if (size == 0)
			return "";
		std::string retval;
		retval.resize(size);
		getenv_s(&size, &retval[0], size, varname.data());
		return denull(std::move(retval));
#else
		const char* s = std::getenv(varname.data());
        if (s)
            return std::string(s);
        return "";
#endif
	}
}
