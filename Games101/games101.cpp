#include "games101.h"
#include <Windows.h>


int run00()
{
	HINSTANCE hDll = LoadLibrary(L"lesson00.dll");
	if (hDll == NULL)
	{
		std::cerr << "Fail to load dll" << std::endl;
		return 1;
	}
	typedef void (*func)();
	func myFunction = (func)GetProcAddress(hDll, "lesson00");
	if (myFunction == NULL)
	{
		std::cerr << "Fail to get function" << std::endl;
		return 1;
	}

	myFunction();

	FreeLibrary(hDll);
}
int run01(int argc, const char** argv)
{
	HINSTANCE hDll = LoadLibrary(L"lesson01.dll");
	if (hDll == NULL)
	{
		std::cerr << "Fail to load dll" << std::endl;
		return 1;
	}
	typedef void (*func)(int argc, const char** argv);
	func myFunction = (func)GetProcAddress(hDll, "lesson01");
	if (myFunction == NULL)
	{
		std::cerr << "Fail to get function" << std::endl;
		return 1;
	}

	myFunction(argc, argv);

	FreeLibrary(hDll);
	return 0;
}


int main(int argc, const char** argv)
{
	run01(argc, argv);
	return 0;
}
