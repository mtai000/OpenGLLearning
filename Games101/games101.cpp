#include "games101.h"
#include "Lesson0/L0.h"
#include "Lesson1/L1.h"

#define LESSON_1

#ifdef LESSON_0
int main()
{
	Lesson0::run();
	return 0;
}
#endif

#ifdef LESSON_1
int main(int argc, const char** argv)
{
	Lesson1::run(argc, argv);
	return 0;
}
#endif
