#include "games101.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

#ifdef _WIN32
	#define LESSONSTR const wchar_t*
	LESSONSTR lesson = L"lesson01.so";
#else
	#define LESSONSTR const char*
	LESSONSTR lesson = "libLesson00.so";
#endif

#define LOGERROR(bErr,msg) if(bErr) {std::cerr << msg << std::endl;return 1;}

int run(int argc, const char** argv)
{
#ifdef  _WIN32
	HINSTANCE hDll = LoadLibrary(lesson);
	LOGERROR(hDll == NULL, "Fail to load dll");
	typedef void (*func)(int argc, const char** argv);

	func myFunction = (func)GetProcAddress(hDll, "run");
	LOGERROR(myFunction == NULL, "Fail to get function");
	myFunction(argc, argv);
	FreeLibrary(hDll);
#else
	void myFunction(int, const char**);
	void* handle = dlopen(lesson, RTLD_LAZY);
	if (!handle) {
		fprintf(stderr, "%s\n", dlerror());
		return 1;
	}
	dlerror();
	*(void**)(&myFunction) = dlsym(handle, "run");
	char* error;
	if ((error = dlerror()) != NULL)
	{
		fprintf(stderr, "%s\n", error);
		dlclose(handle);
		return 1;
	}
	myFunction(argc, argv);
	if (dlclose(handle) != 0)
	{
		fprintf(stderr, "Error closing library\n");
		return 1;
	}
#endif //  _WIN32

	return 0;
}

int main(int argc, const char** argv)
{
	run(argc, argv);
	return 0;
}
