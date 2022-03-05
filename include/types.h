/*	TYPES.H
//
//	Started:	November 11, 1992
//	Last Modified:	October 3, 2019
//	Version 1.2
//	March 18, 2000:		changed to be more "compatible" with MS-Windows rather than ancient
//				Amiga-centric stuff - a bit overdue but 'bout time
//	December 8, 2004:	changed to make it more 64-bit "happy" but shouldn't affect any 32-bit code
//	June 17, 2010:		CUDA-centric modifications
*/

#pragma once

#ifdef __cplusplus
#include <cstdint>
#include <cassert>
#if defined(_NO_MFC) || defined(__GNUC__)
#include "hresult.h"
#endif
#else
#include <stdint.h>
#undef _NODISCARD
#define _NODISCARD
#endif

#include "debug-trap.h"

#define NO_GCC 0
#define GCC_7 70000
#define GCC_8 80000

#ifdef __GNUC__
#define GCC_VERSION (__GNUC__ * 10000 \
					 + __GNUC_MINOR__ * 100 \
					 + __GNUC_PATCHLEVEL__)
#else
#define GCC_VERSION 0
#endif

//<inttypes.h> helpers
#ifdef _MSC_VER
#define SCNdLONG  "d"
#else
#define SCNdLONG  "ld"
#endif

//pragmas
#if defined(__clang__)
#define HSS_PRAGMA_WARNING_PUSH _Pragma("clang diagnostic push")
#define HSS_PRAGMA_WARNING_POP _Pragma("clang diagnostic pop")
#define HSS_PRAGMA_DISABLE_MSVC_WARNING(num)
#define HSS_PRAGMA_SUPPRESS_MSVC_WARNING(num) 
#define HSS_PRAGMA_GCC(val)
#define HSS_PRAGMA_CLANG(val) _Pragma( #val )
#define HSS_PRAGMA_MANAGED_OFF
#define HSS_PRAGMA_MANAGED_ON
#define HSS_PRAGMA_MANAGED_POP
#define HSS_PRAGMA_MANAGED_FORCE_OFF
#define HSS_PRAGMA_MANAGED_FORCE_ON
#elif defined(__GNUC__)
#define HSS_PRAGMA_WARNING_PUSH _Pragma("GCC diagnostic push")
#define HSS_PRAGMA_WARNING_POP _Pragma("GCC diagnostic pop")
#define HSS_PRAGMA_DISABLE_MSVC_WARNING(num) 
#define HSS_PRAGMA_SUPPRESS_MSVC_WARNING(num) 
#define HSS_PRAGMA_GCC(val) _Pragma( #val )
#define HSS_PRAGMA_CLANG(val)
#define HSS_PRAGMA_MANAGED_OFF
#define HSS_PRAGMA_MANAGED_ON
#define HSS_PRAGMA_MANAGED_POP
#define HSS_PRAGMA_MANAGED_FORCE_OFF
#define HSS_PRAGMA_MANAGED_FORCE_ON
#else
#define HSS_PRAGMA_WARNING_PUSH __pragma(warning(push))
#define HSS_PRAGMA_WARNING_POP __pragma(warning(pop))
#define HSS_PRAGMA_DISABLE_MSVC_WARNING(num) __pragma(warning(disable: num))
#define HSS_PRAGMA_SUPPRESS_MSVC_WARNING(num) __pragma(warning(suppress: num))
#define HSS_PRAGMA_GCC(val)
#define HSS_PRAGMA_CLANG(val)
#define HSS_PRAGMA_MANAGED_OFF __pragma(managed(push, off))
#define HSS_PRAGMA_MANAGED_ON  __pragma(managed(push, on))
#define HSS_PRAGMA_MANAGED_POP __pragma(managed(pop))
#define HSS_PRAGMA_MANAGED_FORCE_OFF __pragma(unmanaged)
#define HSS_PRAGMA_MANAGED_FORCE_ON __pragma(managed)
#endif
#if defined(__INTEL_COMPILER) || defined(__INTEL_LLVM_COMPILER)
#define HSS_PRAGMA_DISABLE_INTEL_WARNING(num) __pragma(warning(disable: num))
#else
#define HSS_PRAGMA_DISABLE_INTEL_WARNING(num)
#endif

#ifdef _MSC_VER
	#define __PRETTY_FUNCTION__ __FUNCSIG__
	#define comment_assert(val, comment) assert(val, comment)
	#if defined(NDEBUG) || (!defined(_DEBUG))
		#define weak_assert(val) ((void)0)
	#else
		#define ASSERT_BREAK
		#ifdef __AFX_H__
			#define weak_assert(val) ASSERT(val)
		#else
			#ifdef ASSERT_BREAK
				#define weak_assert(val) do { if (!(val)) { fprintf(stderr, "file %s: line %d (%s): `%s` failed.", __FILE__, __LINE__, __PRETTY_FUNCTION__, #val); psnip_dbg_assert(false); } } while(0)
			#else
				#define weak_assert(val) do { if (!(val)) { fprintf(stderr, "file %s: line %d (%s): `%s` failed.", __FILE__, __LINE__, __PRETTY_FUNCTION__, #val); } } while(0)
			#endif // ASSERT_BREAK
		#endif // __AFX_H__
	#endif // NDEBUG
#else // _MSC_VER
	#define comment_assert(val, comment) assert(((void)(comment), (val)))
	#ifdef _DEBUG
		#ifdef ASSERT_BREAK
			#define weak_assert(val) do { if (!(val)) { fprintf(stderr, "file %s: line %d (%s): `%s` failed.", __FILE__, __LINE__, __PRETTY_FUNCTION__, #val); psnip_dbg_assert(false); } } while(0)
		#else
			#define weak_assert(val) do { if (!(val)) { fprintf(stderr, "file %s: line %d (%s): `%s` failed.", __FILE__, __LINE__, __PRETTY_FUNCTION__, #val); } } while(0)
		#endif // ASSERT_BREAK
	#else
		#define weak_assert(val) ((void)0)
	#endif // _DEBUG
#endif //_MSC_VER

#if defined(__CUDACC__) || (!defined(_MSC_VER))

#if (!(defined(__CUDACC__)) && (defined(_DEBUG)) && (defined(_MSC_VER)))
typedef signed char		BYTE;	// defined in SDK
#endif

#ifndef BASETYPES
typedef int8_t			CHAR;
typedef int16_t			SHORT;
typedef int32_t			BOOL;
typedef int32_t			LONG;
typedef uint8_t			UCHAR; // UBYTE;
typedef uint16_t		USHORT;
typedef uint32_t		ULONG;
#endif

#ifndef FALSE
#define FALSE           0
#endif

#ifndef TRUE
#define TRUE            1
#endif

#ifdef __GNUC__
typedef uint8_t			BYTE;
typedef uint16_t		WORD;
typedef uint32_t		DWORD;
typedef uint64_t		ULONG_PTR;
typedef ULONG_PTR		DWORD_PTR, *PDWORD_PTR;
typedef unsigned int	UINT;
typedef uint32_t        COLORREF;

#define AFX_CDECL 
#endif

typedef int64_t			LONGLONG;
typedef uint64_t		ULONGLONG;

typedef char			*STRPTR;
typedef void			*APTR;

typedef double			DOUBLE;
typedef float			FLOAT;

#ifndef _INC_TCHAR
#ifdef UNICODE
#ifndef _TCHAR_DEFINED
typedef WCHAR TCHAR, *PTCHAR;
typedef WCHAR TBYTE , *PTBYTE ;
#define _TCHAR_DEFINED
#endif
#else
#ifndef _TCHAR_DEFINED
typedef char TCHAR, *PTCHAR;
typedef unsigned char TBYTE , *PTBYTE ;
#define _TCHAR_DEFINED
#endif
#endif
#endif

#else /* _MSC_VER | __CUDACC__ */

#ifndef _INC_TCHAR
#include <tchar.h>
#endif

#endif

typedef void *APTR;

#ifndef __INLINE
#define __INLINE __inline
#endif

#ifdef __CUDACC__
#define DEVICE __device__
#define SHARED __shared__
#else
#define DEVICE
#define SHARED
#endif

#ifdef __GNUC__
#define __FASTCALL
#define __CRTDECL
#define __cdecl
#define TYPENAME
#define FRIEND_TYPENAME class
#elif defined(__INTEL_LLVM_COMPILER)
#define __FASTCALL __stdcall
#define TYPENAME
#define FRIEND_TYPENAME class
#else
#define __FASTCALL __stdcall
#define TYPENAME typename
#define FRIEND_TYPENAME typename
#endif
//#endif

#ifndef VOLATILE
#define VOLATILE volatile
#endif

#ifndef __CUDACC__
#define _3RDBYTEOFLONG(a)	(((a) >> 24)&0xff)
#define _2NDBYTEOFLONG(a)	(((a) >> 16)&0xff)
#define _1STBYTEOFLONG(a)	(((a) >> 8)&0xff)
#define _0THBYTEOFLONG(a)	((a)&0xff)

#define _7THBYTEOFLONGLONG(a)	(((a) >> 56)&0xff)
#define _6THBYTEOFLONGLONG(a)	(((a) >> 48)&0xff)
#define _5THBYTEOFLONGLONG(a)	(((a) >> 40)&0xff)
#define _4THBYTEOFLONGLONG(a)	(((a) >> 32)&0xff)
#define _3RDBYTEOFLONGLONG(a)	(((a) >> 24)&0xff)
#define _2NDBYTEOFLONGLONG(a)	(((a) >> 16)&0xff)
#define _1STBYTEOFLONGLONG(a)	(((a) >> 8)&0xff)
#define _0THBYTEOFLONGLONG(a)	((a)&0xff)

#define _3BYTELONG(a)		((a)&0x00ffffff)
#endif /* __CUDACC__ */

#if defined(__INTEL_COMPILER) || defined(__INTEL_LLVM_COMPILER)
//#define ROB_USE_INTEL_HEAP_MANAGEMENT  // no statistically significant change in performance - RWB, 2015-08-04 using the Grand Cache FGM
#ifdef ROB_USE_INTEL_HEAP_MANAGEMENT
#include "tbb/tbbmalloc_proxy.h"
#endif

#endif

#ifdef __GNUC__
#define MAKEWORD(a, b)      ((WORD)(((BYTE)(((DWORD_PTR)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD_PTR)(b)) & 0xff))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)(((DWORD_PTR)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD_PTR)(b)) & 0xffff))) << 16))
#define LOWORD(l)           ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define HIWORD(l)           ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
#define LOBYTE(w)           ((BYTE)(((DWORD_PTR)(w)) & 0xff))
#define HIBYTE(w)           ((BYTE)((((DWORD_PTR)(w)) >> 8) & 0xff))
#define GetRValue(rgb)      (LOBYTE(rgb))
#define GetGValue(rgb)      (LOBYTE(((WORD)(rgb)) >> 8))
#define GetBValue(rgb)      (LOBYTE((rgb)>>16))
#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))

#define _In_z_
#define _In_
#define _Out_

typedef const TCHAR *LPCTSTR;
#define _tcslen         strlen
#define _tcscmp         strcmp
#define _tcsicmp        stricmp
#define _tcscpy_s       strcpy
#define _tcstok_s       strtok_r
#define _tcsdup         strdup
#define _tcsncpy        strncpy
#define _tcsncpy_s      strncpy
#define _tcsncmp        strncmp
#define _tcsnccmp       strncmp
#define _strdup         strdup
#define _tcstod         strtod
#define _tcstol         strtol
#define _tstof          atof
#define stricmp         strcasecmp
#define _fputts         fputs
#define _ftprintf       fprintf
#define _fgetts         fgets
#define _stscanf_s      sscanf
#ifdef __STDC_LIB_EXT1__
#define _tfopen_s       fopen_s
#define _tcscat_s       strcat_s
#else
#define _tfopen_s(pFile, filename, mode) ((*(pFile))=fopen((filename), (mode)))==NULL
#define _tcscat_s(dst, count, src)   strcat(dst, src)
#endif

#ifndef _T
#define _T(t)			t
#endif

#define __assume(x)

//template<typename C>inline int strcpy_s(C*d, unsigned long dmax, const C*s) { if (dmax <= 1 || !d) { if (!d || !dmax)return 22; *d = C(0); return 0; }for (C*de = d + dmax - 1; (d != de || (*d = C(0))) && (*d = *s); ++d, ++s); return 0; }

#endif

#if defined(_MSC_VER)
#define ALIGNED_(x) __declspec(align(x))
#elif defined(__GNUC__)
#define ALIGNED_(x) __attribute__ ((aligned(x)))
#endif

#define _ALIGNED_TYPE(t, x) typedef t ALIGNED_(x)

#ifdef __GNUC__
#define InterlockedIncrement(x) __sync_add_and_fetch((x), 1)
#define InterlockedDecrement(x) __sync_sub_and_fetch((x), 1)
#define InterlockedExchangeAdd(x, i) __sync_add_and_fetch((x), (i))
#define InterlockedExchangeSubtract(x, i) __sync_sub_and_fetch((x), (i))
//64bit equivalents
#define InterlockedIncrement64(x) __sync_add_and_fetch((x), 1)
#define InterlockedDecrement64(x) __sync_sub_and_fetch((x), 1)
#define InterlockedExchangeAdd64(x, i) __sync_add_and_fetch((x), (i))
#define InterlockedExchangeSubtract64(x, i) __sync_sub_and_fetch((x), (i))
#define InterlockedAdd64(a, b) __sync_add_and_fetch((a), (b))
#endif

#ifndef MAX_PATH
#define MAX_PATH	PATH_MAX
#endif

typedef void *HANDLE;

#ifdef __GNUC__
#define _byteswap_uint64 __builtin_bswap64
#define _byteswap_ulong __builtin_bswap32
#endif

#ifndef min
#ifdef __cplusplus
template <typename T> const T min(const T& a, const T& b) { return (b < a) ? b : a; }
#else
#define min(a, b) ((b) < (a) ? (b) : (a))
#endif
#endif

#ifndef max
#ifdef __cplusplus
template <typename T> const T max(const T& a, const T& b) { return (b > a) ? b : a; }
#else
#define max(a, b) ((b) > (a) ? (b) : (a))
#endif
#endif

