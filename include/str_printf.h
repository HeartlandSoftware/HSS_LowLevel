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
