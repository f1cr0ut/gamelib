#include<unicode/unistr.h>
#include<unicode/ucnv.h>
#include<unicode/normlzr.h>
#include<openssl/evp.h>
#include<openssl/aes.h>
#include <zlib.h>
#include<GL/glew.h>
#include<global_config.h>

#if defined(OS_WIN)
	#include<Windows.h>
	#include <direct.h>
#elif defined(OS_MACOSX) 
	#include <mach-o/dyld.h>
#endif

#if defined(OS_MACOSX) || defined(OS_LINUX)
	#include <sys/stat.h>
	#include <sys/types.h>
#endif

#include"../system/cpu.h"
#include"util.h"

using namespace core;

void core::utf8_to_utf16(const char * src, std::vector<u16> & dst) {
	if( ! src) {
		return;
	}
	icu::UnicodeString s(src, "utf8");
	int32_t len = s.extract(0, s.length(), NULL, "utf16");
	dst.resize(len + 1);
	s.extract(0, s.length(), (char*)&dst[0], "utf16");
}

void core::utf8_to_utf32(const char * src, std::vector<u32> & dst) {
	if( ! src) {
		return;
	}
	icu::UnicodeString s(src, "utf8");
	int32_t len = s.extract(0, s.length(), NULL, "utf32");
	dst.resize(len + 1);
	s.extract(0, s.length(), (char*)&dst[0], "utf32");
}

void core::get_exe_pass(char * dst) {
	if( ! dst) {
		return;
	}
#if defined(OS_WIN)
	char Path[MAX_PATH + 1];
	if (0 != GetModuleFileName(NULL, Path, MAX_PATH)) {
		char drive[MAX_PATH + 1], dir[MAX_PATH + 1], fname[MAX_PATH + 1], ext[MAX_PATH + 1];
		_splitpath(Path, drive, dir, fname, ext);
		sprintf(dst, "%s%s", drive, dir);
	}
#elif defined(OS_LINUX)
	auto len = readlink("/proc/self/exe", dst, strlen(dst) - 1);
	if (len != -1) {
		dst[len] = '\0';
	}
#elif defined(OS_MACOSX) || defined(OS_IOS)
	uint32_t size = strlen(dst);
	_NSGetExecutablePath(dst, &size);
#else
	#error get_exe_pass cannot use your environment
#endif
}


#if defined(OS_WIN)

#include <mutex>
#include<Windows.h>

namespace {

	std::mutex mtx;
	bool is_ready = false;
	static u32 at_exit_msec = 1;

	void onexit() {
		timeEndPeriod(at_exit_msec);
	}
}

void core::set_thread_interval(u32 msec) {
	std::unique_lock<std::mutex> lock(mtx);
	timeBeginPeriod(msec);
	at_exit_msec = msec;
	atexit(onexit);
}

#endif

i32 core::encrypt(const u8 * saltkey, const u8 * password, const u8 *plaintext, const i32 plaintext_len, u8 *ciphertext) {
	if( ! saltkey || ! password || ! plaintext || plaintext_len <= 0 || ! ciphertext) {
		return -1;
	}
	EVP_CIPHER_CTX *ctx;
	i32 len;
	i32 ciphertext_len;
	if (!(ctx = EVP_CIPHER_CTX_new())) {
		return -1;
	}
	u8 key[EVP_MAX_KEY_LENGTH], iv[EVP_MAX_IV_LENGTH];
	const EVP_CIPHER* ci = EVP_aes_256_cbc();
	EVP_BytesToKey(ci, EVP_md5(), saltkey, password, (int)strlen((const char *)password), 1, key, iv);
	if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
		return -1;
	}
	if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) {
		return -1;
	}
	ciphertext_len = len;
	if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) {
		return -1;
	}
	ciphertext_len += len;
	EVP_CIPHER_CTX_free(ctx);
	return ciphertext_len;
}

i32 core::decrypt(const u8 * saltkey, const u8 * password, const u8 *ciphertext, const i32 ciphertext_len, u8 *plaintext) {
	if( ! saltkey || ! password || ! plaintext || ciphertext_len <= 0 || ! ciphertext) {
		return -1;
	}
	EVP_CIPHER_CTX *ctx;
	i32 len;
	i32 plaintext_len;
	if (!(ctx = EVP_CIPHER_CTX_new())) {
		return -1;
	}
	u8 key[EVP_MAX_KEY_LENGTH], iv[EVP_MAX_IV_LENGTH];
	const EVP_CIPHER* ci = EVP_aes_256_cbc();
	EVP_BytesToKey(ci, EVP_md5(), saltkey, password, (int)strlen((const char *)password), 1, key, iv);
	if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
		return -1;
	}
	if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)) {
		return -1;
	}
	plaintext_len = len;

	if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) {
		return -1;
	}
	plaintext_len += len;
	EVP_CIPHER_CTX_free(ctx);
	return plaintext_len;
}

void core::compress(const std::vector<u8> & src, std::vector<u8> & dst) {
	z_stream strm;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = (uInt)src.size();
	strm.next_in = (Bytef*)&src[0];
	strm.avail_out = (uInt)dst.size();
	strm.next_out = (Bytef*)&dst[0];
	deflateInit(&strm, Z_BEST_COMPRESSION);
	deflate(&strm, Z_FINISH);
	deflateEnd(&strm);
}

void core::decompress(const std::vector<u8> & src, std::vector<u8> & dst) {
	z_stream strm;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = (uInt)src.size();
	strm.next_in = (Bytef*)&src[0];
	strm.avail_out = (uInt)dst.size();
	strm.next_out = (Bytef*)&dst[0];
	inflateInit(&strm);
	inflate(&strm, Z_FINISH);
	inflateEnd(&strm);
}

std::string core::get_system_info() {
	std::string tmp, res;
	core::CPU cp;
	tmp += cp.vendor_name();
	tmp += ", ";
	tmp += cp.cpu_brand();
	tmp += ", ";
	tmp += (char*)glGetString(GL_VERSION);
	tmp += ", ";
	tmp += (char*)glGetString(GL_VENDOR);
	tmp += ", ";
	tmp += (char*)glGetString(GL_RENDERER);
	tmp += ", ";
	tmp += (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
#if defined(OS_WIN)
	HKEY hKeyResult;
	DWORD dwDisposition;
	auto lResult = RegCreateKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKeyResult, &dwDisposition);
	if(lResult == ERROR_SUCCESS) {
		DWORD dwType = REG_SZ;
		DWORD dwData = 0;
		RegQueryValueEx(hKeyResult, "BuildLabEx", NULL, &dwType, NULL, &dwData);
		std::vector<u8> v(dwData);
		if( ! v.empty()) {
			RegQueryValueEx(hKeyResult, "BuildLabEx", NULL, &dwType, &v[0], &dwData);
			tmp += ", ";
			tmp += (char*)&v[0];
		}
	}
	RegCloseKey(hKeyResult);
#else
	#error get_system_info is not available
#endif
	for(auto & i : tmp) {
		if(i != 0) {
			res.push_back(i);
		}
	}
	return std::move(res);
}

bool core::mkdir(const char * dir) {
	if( ! dir) {
		return false;
	}
#if defined(OS_WIN)
	auto ret = _mkdir(dir) == 0;
	SetFileAttributes(dir, FILE_ATTRIBUTE_NORMAL);
	return ret;
#elif defined(OS_LINUX) || defined(OS_MACOSX)
	struct stat st;
	if (stat(dir, &st) != 0) {
		return mkdir(dir, 0775);
	}
#endif
}

bool core::delete_file(const char * file) {
	if( ! file) {
		return false;
	}
#if defined(OS_WIN)
	return DeleteFile(file);
#elif defined(OS_LINUX) || defined(OS_MACOSX)
	return std::remove(file);
#endif
}

bool core::delete_dir(const char * dir) {
	if( ! dir) {
		return false;
	}
#if defined(OS_WIN)
	return RemoveDirectory(dir);
#elif defined(OS_LINUX) || defined(OS_MACOSX)
	return std::remove(dir);
#endif
}

void core::replace_path_slash(char * p) {
	for (int i = 0; p[i] != 0; ++i) {
#if defined(OS_WIN)
		if(p[i] == '/') {
			p[i] = '\\';
		}
#else
		if(p[i] == '\\') {
			p[i] = '/';
		}
#endif
	}
}