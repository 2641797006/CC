#ifndef _INI_H_
#define _INI_H_

#include <fstream>
#include <string>
#include <list>

namespace _24k{
using namespace std;

class Pair{
  public:
	string name;
	string value;
};

class Prop{
  public:
	string main;
	list<Pair> kv;
};

class INI{
  public:
	INI(string const&);
//	~INI();

	void print();

	string file_name;
	list<Prop> data;
	list<Prop>::iterator it_prop;
	list<Pair>::iterator it_pair;
};

} // namespace _24k

#endif // _INI_H_

