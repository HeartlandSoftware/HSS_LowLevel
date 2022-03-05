#pragma once

#if __has_include(<mathimf.h>)
#define HSS_SUPPORTS_MATH_IMF 1
#else
#undef HSS_SUPPORTS_MATH_IMF
#endif

#if (defined(__INTEL_COMPILER) || defined(__INTEL_LLVM_COMPILER))
#define HSS_IS_INTEL_COMPILER 1
#ifdef __INTEL_LLVM_COMPILER
#define HSS_INTEL_IS_CLANG 1
#else
#undef HSS_INTEL_IS_CLANG
#endif
#else
#undef HSS_IS_INTEL_COMPILER
#endif

#ifdef _MSC_VER
#define HSS_IS_WINDOWS 1
#else
#undef HSS_IS_WINDOWS
#endif

#if defined(_MSC_VER) || defined(__GNUC__) || defined(HSS_IS_INTEL_COMPILER)
#define HSS_SHOULD_PRAGMA_PACK_MIN 1
#ifndef __GNUC__
#define HSS_SHOULD_PRAGMA_PACK 1
#endif
#else
#undef HSS_SHOULD_PRAGMA_PACK
#endif

#if (defined(_WIN64) || defined(__LLP64__) || defined(__LP64__))
#define HSS_IS_64_BIT 1
#else
#undef HSS_IS_64_BIT
#endif

#if (defined(_MSC_VER) && !defined(HSS_IS_INTEL_COMPILER))
#define HSS_SUPPORTS_MANAGED 1
#else
#undef HSS_SUPPORTS_MANAGED
#endif

#if (defined(__GNUC__) || defined(HSS_INTEL_IS_CLANG))
#define HSS_STRICT_CPP 1
#else
#undef HSS_STRICT_CPP
#endif
