#include "c4_1.h"
#include "c4_2.h"
#include "c4_3.h"
#include "c5_1.h"
#include "c6_1.h"
#include "c2.h"


int main(int argc, char * argv[]) {
	ChapterBase* m = new Chapter2();
	string  para;
	if (argc > 1)
		para = argv[1];
	else
		para = "6_1";
	if (para == "2")
		m = new Chapter2();
	else if (para == "4_1")
		m = new Chapter4_1();
	else if (para == "4_2")
		m = new Chapter4_2();
	else if (para == "4_3")
		m = new Chapter4_3();
	else if (para == "5_1")
		m = new Chapter5_1();
	else if (para == "6_1")
		m = new Chapter6_1();
	cout << para << endl;
	cout << m << endl;
	m->run();
	
	return 0;
}
