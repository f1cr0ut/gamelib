#pragma once

#include<vector>
#include<sstream>
#include<global_config.h>

namespace core {


	// WINDOWS:
	// std::wstring is 16bit for windows
	// dst[0] is a big-endian(0xfeff as hex, 65279 as decimal)
	// so you need to get from dst[1]
	void APICALL utf8_to_utf16(const char * src, std::vector<u16> & dst);

	void APICALL utf8_to_utf32(const char * src, std::vector<u32> & dst);

	// get absolute pass of executable file
	void APICALL get_exe_pass(char * dst);

	// change windows thread schedule from interval of 15.6[msec] to msec
	// WARN:
	// this function is too dangerous for windows user.
	// DO NOT USE this function if you have some reason enough to do that
#if defined(OS_WIN)
	void APICALL set_thread_interval(u32 msec);
#endif

	// AES encryption of openssl
	i32 APICALL encrypt(const u8 * saltkey, const u8 * password, const u8 *plaintext, const i32 plaintext_len, u8 *ciphertext);

	// AES decryption of openssl
	i32 APICALL decrypt(const u8 * saltkey, const u8 * password, const u8 *ciphertext, const i32 ciphertext_len, u8 *plaintext);

	// compress data using zlib
	void  APICALL compress(const std::vector<u8> & src, std::vector<u8> & dst);

	// decompress data using zlib
	void  APICALL decompress(const std::vector<u8> & src, std::vector<u8> & dst);

	template<class T>
	std::string APICALL val_to_str(const T & t) {
		std::stringstream ss;
		ss << t;
		return std::move(ss.str());
	}

	// vendor name \n
	// cpu brand \n
	// opengl version \n
	// vendor \n
	// renderer \n
	// shading language version
	std::string APICALL get_system_info();

	// create directory
	bool APICALL mkdir(const char * dir);

	bool APICALL delete_file(const char * file);
	bool APICALL delete_dir(const char * dir);

	// windows : '/' -> '\\'
	// other : '\\' -> '/'
	void APICALL replace_path_slash(char * p);
}
