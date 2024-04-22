#include "games101.h"

#include "Lesson01/L1.h"
#include "Lesson00/L0.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif


int main(int argc, const char** argv)
{
	lesson00(argc, argv);
	return 0;
}
