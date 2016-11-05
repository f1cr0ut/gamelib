#include<boot.h>
#include<Windows.h>
#include<string>

#if defined(DEBUG_BUILD)

int main(int argc, char ** argv) 

#elif defined(RELEASE_BUILD)

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)

#endif

{
	if(SetDllDirectory("res\\dll") == 0) {
		MessageBox(NULL, "failed to find dll directory", "error", 0);
		return EXIT_FAILURE;
	}

#if defined(DEBUG_BUILD)

	auto handle = LoadLibrary("gamed.dll");

#elif defined(RELEASE_BUILD)

	auto argc = __argc;
	auto argv = __argv;
	auto handle = LoadLibrary("game++.dll");

#endif

	int ret = EXIT_FAILURE;
	if(handle != NULL) {
		auto p = (int(*)(int, char**))GetProcAddress(handle, "boot");
		if(p) {
			ret = p(argc, argv);
		}
		else {
			MessageBox(NULL, "failed to load dll function", "error", 0);
		}
	}
	else {
		MessageBox(NULL, "failed to load game dll", "error", 0);
	}
	FreeLibrary(handle);
	return ret;
}