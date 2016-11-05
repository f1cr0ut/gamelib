// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "cpu.h"
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <algorithm>

#include <stddef.h>  // For size_t.
// Put this in the declarations for a class to be uncopyable.
#define DISALLOW_COPY(TypeName) \
  TypeName(const TypeName&) = delete
// Put this in the declarations for a class to be unassignable.
#define DISALLOW_ASSIGN(TypeName) \
  void operator=(const TypeName&) = delete
// A macro to disallow all the implicit constructors, namely the
// default constructor, copy constructor and operator= functions.
//
// This should be used in the private: declarations for a class
// that wants to prevent anyone from instantiating it. This is
// especially useful for classes containing only static methods.
#define DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
  TypeName() = delete;                           \
  DISALLOW_COPY_AND_ASSIGN(TypeName)
// The arraysize(arr) macro returns the # of elements in an array arr.  The
// expression is a compile-time constant, and therefore can be used in defining
// new arrays, for example.  If you use arraysize on a pointer by mistake, you
// will get a compile-time error.  For the technical details, refer to
// http://blogs.msdn.com/b/the1/archive/2004/05/07/128242.aspx.
// This template function declaration is used in defining arraysize.
// Note that the function doesn't need an implementation, as we only
// use its type.
template <typename T, size_t N> char(&ArraySizeHelper(T(&array)[N]))[N];
#define arraysize(array) (sizeof(ArraySizeHelper(array)))
// Used to explicitly mark the return value of a function as unused. If you are
// really sure you don't want to do anything with the return value of a function
// that has been marked WARN_UNUSED_RESULT, wrap it with this. Example:
//
//   std::unique_ptr<MyType> my_var = ...;
//   if (TakeOwnership(my_var.get()) == SUCCESS)
//     ignore_result(my_var.release());
//
template<typename T>
inline void ignore_result(const T&) {
}
// The following enum should be used only as a constructor argument to indicate
// that the variable has static storage class, and that the constructor should
// do nothing to its state.  It indicates to the reader that it is legal to
// declare a static instance of the class, provided the constructor is given
// the base::LINKER_INITIALIZED argument.  Normally, it is unsafe to declare a
// static variable that has a constructor or a destructor because invocation
// order is undefined.  However, IF the type can be initialized by filling with
// zeroes (which the loader does for static variables), AND the destructor also
// does nothing to the storage, AND there are no virtual methods, then a
// constructor declared as
//       explicit MyClass(base::LinkerInitialized x) {}
// and invoked as
//       static MyClass my_variable_name(base::LINKER_INITIALIZED);
	enum LinkerInitialized { LINKER_INITIALIZED };
	// Use these to declare and define a static local variable (static T;) so that
	// it is leaked so that its destructors are not called at exit. If you need
	// thread-safe initialization, use base/lazy_instance.h instead.
#define CR_DEFINE_STATIC_LOCAL(type, name, arguments) \
  static type& name = *new type arguments

	// A set of macros to use for platform detection.
#if defined(__native_client__)
	// __native_client__ must be first, so that other OS_ defines are not set.
#define OS_NACL 1
	// OS_NACL comes in two sandboxing technology flavors, SFI or Non-SFI.
	// PNaCl toolchain defines __native_client_nonsfi__ macro in Non-SFI build
	// mode, while it does not in SFI build mode.
#if defined(__native_client_nonsfi__)
#define OS_NACL_NONSFI
#else
#define OS_NACL_SFI
#endif
#elif defined(ANDROID)
#define OS_ANDROID 1
#elif defined(__APPLE__)
	// only include TargetConditions after testing ANDROID as some android builds
	// on mac don't have this header available and it's not needed unless the target
	// is really mac/ios.
#include <TargetConditionals.h>
#define OS_MACOSX 1
#if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
#define OS_IOS 1
#endif  // defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
#elif defined(__linux__)
#define OS_LINUX 1
	// include a system header to pull in features.h for glibc/uclibc macros.
#include <unistd.h>
#if defined(__GLIBC__) && !defined(__UCLIBC__)
	// we really are using glibc, not uClibc pretending to be glibc
#define LIBC_GLIBC 1
#endif
#elif defined(_WIN32)
#define OS_WIN 1
#define TOOLKIT_VIEWS 1
#elif defined(__FreeBSD__)
#define OS_FREEBSD 1
#elif defined(__NetBSD__)
#define OS_NETBSD 1
#elif defined(__OpenBSD__)
#define OS_OPENBSD 1
#elif defined(__sun)
#define OS_SOLARIS 1
#elif defined(__QNXNTO__)
#define OS_QNX 1
#else
#error Please add support for your platform in build/build_config.h
#endif
#if defined(USE_OPENSSL_CERTS) && defined(USE_NSS_CERTS)
#error Cannot use both OpenSSL and NSS for certificates
#endif
	// For access to standard BSD features, use OS_BSD instead of a
	// more specific macro.
#if defined(OS_FREEBSD) || defined(OS_NETBSD) || defined(OS_OPENBSD)
#define OS_BSD 1
#endif
	// For access to standard POSIXish features, use OS_POSIX instead of a
	// more specific macro.
#if defined(OS_MACOSX) || defined(OS_LINUX) || defined(OS_FREEBSD) ||     \
    defined(OS_NETBSD) || defined(OS_OPENBSD) || defined(OS_SOLARIS) ||   \
    defined(OS_ANDROID) || defined(OS_OPENBSD) || defined(OS_SOLARIS) ||  \
    defined(OS_ANDROID) || defined(OS_NACL) || defined(OS_QNX)
#define OS_POSIX 1
#endif
	// Use tcmalloc
#if (defined(OS_WIN) || defined(OS_LINUX) || defined(OS_ANDROID)) && \
    !defined(NO_TCMALLOC)
#define USE_TCMALLOC 1
#endif
	// Compiler detection.
#if defined(__GNUC__)
#define COMPILER_GCC 1
#elif defined(_MSC_VER)
#define COMPILER_MSVC 1
#else
#error Please add support for your compiler in build/build_config.h
#endif
	// Processor architecture detection.  For more info on what's defined, see:
	//   http://msdn.microsoft.com/en-us/library/b0084kay.aspx
	//   http://www.agner.org/optimize/calling_conventions.pdf
	//   or with gcc, run: "echo | gcc -E -dM -"
#if defined(_M_X64) || defined(__x86_64__)
#define ARCH_CPU_X86_FAMILY 1
#define ARCH_CPU_X86_64 1
#define ARCH_CPU_64_BITS 1
#define ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(_M_IX86) || defined(__i386__)
#define ARCH_CPU_X86_FAMILY 1
#define ARCH_CPU_X86 1
#define ARCH_CPU_32_BITS 1
#define ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(__ARMEL__)
#define ARCH_CPU_ARM_FAMILY 1
#define ARCH_CPU_ARMEL 1
#define ARCH_CPU_32_BITS 1
#define ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(__aarch64__)
#define ARCH_CPU_ARM_FAMILY 1
#define ARCH_CPU_ARM64 1
#define ARCH_CPU_64_BITS 1
#define ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(__pnacl__)
#define ARCH_CPU_32_BITS 1
#define ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(__MIPSEL__)
#if defined(__LP64__)
#define ARCH_CPU_MIPS_FAMILY 1
#define ARCH_CPU_MIPS64EL 1
#define ARCH_CPU_64_BITS 1
#define ARCH_CPU_LITTLE_ENDIAN 1
#else
#define ARCH_CPU_MIPS_FAMILY 1
#define ARCH_CPU_MIPSEL 1
#define ARCH_CPU_32_BITS 1
#define ARCH_CPU_LITTLE_ENDIAN 1
#endif
#else
#error Please add support for your architecture in build/build_config.h
#endif
	// Type detection for wchar_t.
#if defined(OS_WIN)
#define WCHAR_T_IS_UTF16
#elif defined(OS_POSIX) && defined(COMPILER_GCC) && \
    defined(__WCHAR_MAX__) && \
    (__WCHAR_MAX__ == 0x7fffffff || __WCHAR_MAX__ == 0xffffffff)
#define WCHAR_T_IS_UTF32
#elif defined(OS_POSIX) && defined(COMPILER_GCC) && \
    defined(__WCHAR_MAX__) && \
    (__WCHAR_MAX__ == 0x7fff || __WCHAR_MAX__ == 0xffff)
	// On Posix, we'll detect short wchar_t, but projects aren't guaranteed to
	// compile in this mode (in particular, Chrome doesn't). This is intended for
	// other projects using base who manage their own dependencies and make sure
	// short wchar works for them.
#define WCHAR_T_IS_UTF16
#else
#error Please add support for your compiler in build/build_config.h
#endif
#if defined(OS_ANDROID)
	// The compiler thinks std::string::const_iterator and "const char*" are
	// equivalent types.
#define STD_STRING_ITERATOR_IS_CHAR_POINTER
	// The compiler thinks base::string16::const_iterator and "char16*" are
	// equivalent types.
#define BASE_STRING16_ITERATOR_IS_CHAR16_POINTER
#endif

#if defined(ARCH_CPU_ARM_FAMILY) && (defined(OS_ANDROID) || defined(OS_LINUX))
#include "base/files/file_util.h"
#include "base/lazy_instance.h"
#endif
#if defined(ARCH_CPU_X86_FAMILY)
#if defined(_MSC_VER)
#include <intrin.h>
#include <immintrin.h>  // For _xgetbv()
#endif
#endif

using namespace core;

CPU::CPU() : signature_(0),
	type_(0),
	family_(0),
	model_(0),
	stepping_(0),
	ext_model_(0),
	ext_family_(0),
	has_mmx_(false),
	has_sse_(false),
	has_sse2_(false),
	has_sse3_(false),
	has_ssse3_(false),
	has_sse41_(false),
	has_sse42_(false),
	has_avx_(false),
	has_avx2_(false),
	has_aesni_(false),
	has_non_stop_time_stamp_counter_(false),
	cpu_vendor_("unknown") {
	Initialize();
}
namespace {
#if defined(ARCH_CPU_X86_FAMILY)
#ifndef _MSC_VER
#if defined(__pic__) && defined(__i386__)
	void __cpuid(int cpu_info[4], int info_type) {
		__asm__ volatile (
			"mov %%ebx, %%edi\n"
			"cpuid\n"
			"xchg %%edi, %%ebx\n"
			: "=a"(cpu_info[0]), "=D"(cpu_info[1]), "=c"(cpu_info[2]), "=d"(cpu_info[3])
			: "a"(info_type)
			);
	}
#else
	void __cpuid(int cpu_info[4], int info_type) {
		__asm__ volatile (
			"cpuid\n"
			: "=a"(cpu_info[0]), "=b"(cpu_info[1]), "=c"(cpu_info[2]), "=d"(cpu_info[3])
			: "a"(info_type)
			);
	}
#endif
	// _xgetbv returns the value of an Intel Extended Control Register (XCR).
	// Currently only XCR0 is defined by Intel so |xcr| should always be zero.
	uint64_t _xgetbv(uint32_t xcr) {
		uint32_t eax, edx;
		__asm__ volatile (
			"xgetbv" : "=a"(eax), "=d"(edx) : "c"(xcr));
		return (static_cast<uint64_t>(edx) << 32) | eax;
	}
#endif  // !_MSC_VER
#endif  // ARCH_CPU_X86_FAMILY
#if defined(ARCH_CPU_ARM_FAMILY) && (defined(OS_ANDROID) || defined(OS_LINUX))
	class LazyCpuInfoValue {
	public:
		LazyCpuInfoValue() {
			// This function finds the value from /proc/cpuinfo under the key "model
			// name" or "Processor". "model name" is used in Linux 3.8 and later (3.7
			// and later for arm64) and is shown once per CPU. "Processor" is used in
			// earler versions and is shown only once at the top of /proc/cpuinfo
			// regardless of the number CPUs.
			const char kModelNamePrefix[] = "model name\t: ";
			const char kProcessorPrefix[] = "Processor\t: ";
			std::string contents;
			ReadFileToString(FilePath("/proc/cpuinfo"), &contents);
			DCHECK(!contents.empty());
			if (contents.empty()) {
				return;
			}
			std::istringstream iss(contents);
			std::string line;
			while (std::getline(iss, line)) {
				if (brand_.empty() &&
					(line.compare(0, strlen(kModelNamePrefix), kModelNamePrefix) == 0 ||
						line.compare(0, strlen(kProcessorPrefix), kProcessorPrefix) == 0)) {
					brand_.assign(line.substr(strlen(kModelNamePrefix)));
				}
			}
		}
		const std::string& brand() const { return brand_; }
	private:
		std::string brand_;
		DISALLOW_COPY_AND_ASSIGN(LazyCpuInfoValue);
	};
	base::LazyInstance<LazyCpuInfoValue>::Leaky g_lazy_cpuinfo =
		LAZY_INSTANCE_INITIALIZER;
#endif  // defined(ARCH_CPU_ARM_FAMILY) && (defined(OS_ANDROID) ||
	// defined(OS_LINUX))
}  // anonymous namespace
void CPU::Initialize() {
#if defined(ARCH_CPU_X86_FAMILY)
	int cpu_info[4] = { -1 };
	char cpu_string[48];
	// __cpuid with an InfoType argument of 0 returns the number of
	// valid Ids in CPUInfo[0] and the CPU identification string in
	// the other three array elements. The CPU identification string is
	// not in linear order. The code below arranges the information
	// in a human readable form. The human readable order is CPUInfo[1] |
	// CPUInfo[3] | CPUInfo[2]. CPUInfo[2] and CPUInfo[3] are swapped
	// before using memcpy to copy these three array elements to cpu_string.
	__cpuid(cpu_info, 0);
	int num_ids = cpu_info[0];
	std::swap(cpu_info[2], cpu_info[3]);
	memcpy(cpu_string, &cpu_info[1], 3 * sizeof(cpu_info[1]));
	cpu_vendor_.assign(cpu_string, 3 * sizeof(cpu_info[1]));
	// Interpret CPU feature information.
	if (num_ids > 0) {
		int cpu_info7[4] = { 0 };
		__cpuid(cpu_info, 1);
		if (num_ids >= 7) {
			__cpuid(cpu_info7, 7);
		}
		signature_ = cpu_info[0];
		stepping_ = cpu_info[0] & 0xf;
		model_ = ((cpu_info[0] >> 4) & 0xf) + ((cpu_info[0] >> 12) & 0xf0);
		family_ = (cpu_info[0] >> 8) & 0xf;
		type_ = (cpu_info[0] >> 12) & 0x3;
		ext_model_ = (cpu_info[0] >> 16) & 0xf;
		ext_family_ = (cpu_info[0] >> 20) & 0xff;
		has_mmx_ = (cpu_info[3] & 0x00800000) != 0;
		has_sse_ = (cpu_info[3] & 0x02000000) != 0;
		has_sse2_ = (cpu_info[3] & 0x04000000) != 0;
		has_sse3_ = (cpu_info[2] & 0x00000001) != 0;
		has_ssse3_ = (cpu_info[2] & 0x00000200) != 0;
		has_sse41_ = (cpu_info[2] & 0x00080000) != 0;
		has_sse42_ = (cpu_info[2] & 0x00100000) != 0;
		// AVX instructions will generate an illegal instruction exception unless
		//   a) they are supported by the CPU,
		//   b) XSAVE is supported by the CPU and
		//   c) XSAVE is enabled by the kernel.
		// See http://software.intel.com/en-us/blogs/2011/04/14/is-avx-enabled
		//
		// In addition, we have observed some crashes with the xgetbv instruction
		// even after following Intel's example code. (See crbug.com/375968.)
		// Because of that, we also test the XSAVE bit because its description in
		// the CPUID documentation suggests that it signals xgetbv support.
		has_avx_ =
			(cpu_info[2] & 0x10000000) != 0 &&
			(cpu_info[2] & 0x04000000) != 0 /* XSAVE */ &&
			(cpu_info[2] & 0x08000000) != 0 /* OSXSAVE */ &&
			(_xgetbv(0) & 6) == 6 /* XSAVE enabled by kernel */;
		has_aesni_ = (cpu_info[2] & 0x02000000) != 0;
		has_avx2_ = has_avx_ && (cpu_info7[1] & 0x00000020) != 0;
	}
	// Get the brand string of the cpu.
	__cpuid(cpu_info, 0x80000000);
	const int parameter_end = 0x80000004;
	int max_parameter = cpu_info[0];
	if (cpu_info[0] >= parameter_end) {
		char* cpu_string_ptr = cpu_string;
		for (int parameter = 0x80000002; parameter <= parameter_end &&
			cpu_string_ptr < &cpu_string[sizeof(cpu_string)]; parameter++) {
			__cpuid(cpu_info, parameter);
			memcpy(cpu_string_ptr, cpu_info, sizeof(cpu_info));
			cpu_string_ptr += sizeof(cpu_info);
		}
		cpu_brand_.assign(cpu_string, cpu_string_ptr - cpu_string);
	}
	const int parameter_containing_non_stop_time_stamp_counter = 0x80000007;
	if (max_parameter >= parameter_containing_non_stop_time_stamp_counter) {
		__cpuid(cpu_info, parameter_containing_non_stop_time_stamp_counter);
		has_non_stop_time_stamp_counter_ = (cpu_info[3] & (1 << 8)) != 0;
	}
#elif defined(ARCH_CPU_ARM_FAMILY) && (defined(OS_ANDROID) || defined(OS_LINUX))
	cpu_brand_.assign(g_lazy_cpuinfo.Get().brand());
#endif
}
CPU::IntelMicroArchitecture CPU::GetIntelMicroArchitecture() const {
	if (has_avx2()) return AVX2;
	if (has_avx()) return AVX;
	if (has_sse42()) return SSE42;
	if (has_sse41()) return SSE41;
	if (has_ssse3()) return SSSE3;
	if (has_sse3()) return SSE3;
	if (has_sse2()) return SSE2;
	if (has_sse()) return SSE;
	return PENTIUM;
}
