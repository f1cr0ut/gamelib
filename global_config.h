/*
64bit compile only.
*/
#pragma once

// gcc >= 5.4
#if defined(__GNUC__)
	#if __GNUC__ < 5
		#error compiler is too old
	#elif __GNUC_MINOR__ < 4
		#error compiler is too old
	#endif
	#define COMPILER_GCC 1
#endif

// msvc >= vc2015 update3
#if defined(_MSC_FULL_VER)
	#if _MSC_FULL_VER < 190024210
		#error compiler is too old
	#endif
	#define COMPILER_MSVC 1
	#define OS_WIN 1
#endif

// linux os
#if defined(__linux__)
	#define OS_LINUX 1
#endif

// mac osx os
#if defined(__APPLE__)
	#define OS_MACOSX 1

	#include <TargetConditionals.h>
	// ios os
	#if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
		#define OS_IOS 1
	#endif 
#endif

// android os
#if defined(ANDROID)
	#define OS_ANDROID 1
#endif

// mingw compile
#if defined(__MINGW64__)
	#define COMPILE_ENV_MINGW 1
#endif

// cygwin compile
#if defined(__CYGWIN__)
	#define COMPILE_ENV_CYGWIN 1
#endif

// clang compile
#if defined(__clang__)
	#define COMPILE_ENV_CLANG 1
#endif

// api for shared library
#if ! defined(APICALL)
	#if defined(OS_WIN)
		#define APICALL __declspec(dllexport)
	#else
		#define APICALL
	#endif
#else
	#error APICALL is already defined
#endif

// build mode
#if defined(_DEBUG) || defined(DEBUG)
	#define DEBUG_BUILD 1
#elif defined(STAGING)
	#define STAGING_BUILD 1
#elif defined(RELEASE)
	#define RELEASE_BUILD 1
#else
	#error set build mode
#endif

// debug print
#if defined(DEBUG_BUILD)
	#define DBGPRINT(a) std::cout << (a) << std::endl
#else
	#define DBGPRINT(a)
#endif

#include<cstdint>

typedef uint8_t u8;
typedef int8_t i8;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint32_t u32;
typedef int32_t i32;
typedef uint64_t u64;
typedef int64_t i64;
typedef float f32;
typedef double f64;

#define DISALLOW_COPY_AND_ASSIGN(a) \
	a(const a&) = delete;\
	a(const a&&) = delete;\
	a & operator=(const a&) = delete;\
	a & operator=(const a&&) = delete

// less than 64 characters
#define GAME_VERSION_STRING "0.1.0"