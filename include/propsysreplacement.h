//	propsysreplacement.h
//
//	By Robert W. Bryce
//	Version 1.2
//	Started:	January 17, 2008
//	Last Modified:	November 4, 2018

#pragma once

#if __has_include(<mathimf.h>)
#include <mathimf.h>
#else
#include <cmath>
#endif
#ifdef _MSC_VER
#pragma managed(push, off)
#endif

#ifndef _NO_MFC

#include <afxwin.h>
#include "types.h"
#include <propvarutil.h>

HRESULT VariantToBoolean_(const VARIANT &varIn, BOOL *retval);
HRESULT VariantToInt16_(const VARIANT &varIn, SHORT *retval);
HRESULT VariantToInt32_(const VARIANT &varIn, LONG *retval);
HRESULT VariantToInt64_(const VARIANT &varIn, LONGLONG *retval);
HRESULT VariantToUInt16_(const VARIANT &varIn, USHORT *retval);
HRESULT VariantToUInt32_(const VARIANT &varIn, ULONG *retval);
HRESULT VariantToUInt64_(const VARIANT &varIn, ULONGLONG *retval);
HRESULT VariantToDouble_(const VARIANT &varIn, DOUBLE *pdblRet);

#endif

#if __cplusplus>=201700 || _MSVC_LANG >= 201703
#include <cstdint>
#include <variant>

typedef std::variant<std::monostate, bool, std::int8_t, std::int16_t, std::int32_t, std::int64_t, std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t, float, double> NumericVariant;


bool variantToBoolean(const NumericVariant &varIn, bool *retval);
bool variantToInt8(const NumericVariant &varIn, std::int8_t *retval);
bool variantToInt16(const NumericVariant &varIn, std::int16_t *retval);
bool variantToInt32(const NumericVariant &varIn, std::int32_t *retval);
bool variantToInt64(const NumericVariant &varIn, std::int64_t *retval);
bool variantToUInt8(const NumericVariant &varIn, std::uint8_t *retval);
bool variantToUInt16(const NumericVariant &varIn, std::uint16_t *retval);
bool variantToUInt32(const NumericVariant &varIn, std::uint32_t *retval);
bool variantToUInt64(const NumericVariant &varIn, std::uint64_t *retval);
bool variantToFloat(const NumericVariant &varIn, float *pdblRet);
bool variantToDouble(const NumericVariant &varIn, double *pdblRet);

#elif defined(_NO_MFC)
static_assert(false, "No C++17 support");
#endif

#if !defined(__INTEL_COMPILER) && !defined(__INTEL_LLVM_COMPILER) && defined(_MSC_VER)
#pragma managed(pop)
#endif
