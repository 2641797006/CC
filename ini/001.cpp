#include <iostream>
#include <string>
#include "ini.h"
using namespace std;
using namespace _24k;

int
main()
{
	string fname = "24.txt";
	INI ini(fname);

	ini.print();
}

