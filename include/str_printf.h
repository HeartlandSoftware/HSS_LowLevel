/**
 * str_printf.h
 *
 * Copyright 2017-2023 Heartland Software Solutions Inc.
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

#include <string>

std::string strprintf(const char *fmt, ...);

#if defined(__cpp_variadic_templates) && __has_include("boost/format.hpp")
#include <boost/format.hpp>



namespace hss
{
	template<template<typename...> class T, typename U>
	struct is_specialization_of : std::false_type { };

	template<template<typename...> class T, typename... Ts>
	struct is_specialization_of<T, T<Ts...>> : std::true_type { };

	template<typename... T>
	constexpr
	std::enable_if_t<(false || ... || hss::is_specialization_of<std::basic_string, T>::value), std::string>
	strprintf(const char* fmt, T&&... args)
	{
		return (boost::format(fmt) % ... % std::forward<T>(args)).str();
	}

	template<typename... T>
	constexpr
	std::enable_if_t<(true && ... && !hss::is_specialization_of<std::basic_string, T>::value), std::string>
	strprintf(const char* fmt, T&&... args)
	{
		return ::strprintf(fmt, std::forward<T>(args)...);
	}
}
#endif
