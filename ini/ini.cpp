#include <iostream>
#include <ctype.h>
#include <assert.h>
#include "ini.h"

#define isexchr(c)	( ((int)c & 0xff) > 0x7f )
#define isstrch(c)	( isprint(c) || isexchr(c) )
#define PAIR_END	5

namespace _24k{
using namespace std;

int
sw(int c)
{
	if ( isblank(c) )
		return 0;
	if ( c == '\n' )
		return 5;
	if ( c == '=' )
		return 4;
	if ( c == '[' )
		return 2;
	if ( c == ']' )
		return 3;
	if ( isstrch(c) )
		return 1;
	return -1;
}

int dfa[][6]{
//	  0   1   2   3   4   5
//	blank %s '[' ']'  = '\n'
	{ 0, -1,  1, -1, -1,  0}, // 0 blank [
	{ 1,  2, -1, -1, -1, -1}, // 1 blank %s
	{ 3,  2, -1,  4, -1, -1}, // 2 %s blank/']'
	{ 3, -1, -1,  4, -1, -1}, // 3 blank ]
	{ 4, -1, -1, -1, -1,  5}, // 4 blank '\n'

	{ 5,  6,  1, -1, -1,  5}, // 5 blank %s
	{ 7,  6, -1, -1,  8, -1}, // 6 %s blank/'='
	{ 7, -1, -1, -1,  8, -1}, // 7 blank =
	{ 8,  9, -1, -1, -1, -1}, // 8 blank %s
	{10,  9, -1, -1, -1,  5}, // 9 %s blank/'\n'
	{10, -1, -1, -1, -1,  5}, //10 blank '\n'
};

INI::INI(string const& s): file_name(s) {
	int state, line, col, c, c1, ignore;
	Prop prop;
	Pair pair;

	state = 0;
	line = 1;
	col = 0;
	ignore = 0;

	fstream f(s, f.in);
	assert( f.is_open() );

	for (;;) {
		c = f.get();
		c == '\n' ? (++line,ignore=0,col=0) : ++col;
		if ( f.eof() )
			return;
		if ( ignore )
			continue;
		if ( c == '#' ) {
			ignore = 1;
			continue;
		}
		c1 = sw(c);
		if ( c1 < 0 )
			break;
		c1 = dfa[state][c1];
		if ( c1 < 0 )
			break;
		switch (c1) {
		case 2:
			prop.main.push_back(c);
			break;
		case 6:
			pair.name.push_back(c);
			break;
		case 9:
			pair.value.push_back(c);
			break;
		case PAIR_END:
			if (state != PAIR_END-1 && pair.name.size() > 0) {
				data.back().kv.push_back(pair);
				pair.name.clear();
				pair.value.clear();
			}
			else if (prop.main.size() > 0) {
				data.push_back(prop);
				prop.kv.clear();
				prop.main.clear();
			}
		}
		state = c1;
	}
	f.putback(c);
	cout<<"ERROR: Check your input file to use the correct syntax in file=`"
		<<file_name<<"` line=`"<<line<<"` col=`"<<col<<"` near \'";
	for (;;) {
		c = f.get();
		if ( f.eof() || c=='\n' )
			break;
		cout<<(char)c;
	}
	cout<<"\'\n";
	return;
}

void
INI::print()
{
	list<Prop>::iterator it_prop = data.begin();
	list<Pair>::iterator it_pair;

	while (it_prop != data.end()) {
		cout<< '[' << it_prop->main << ']' <<'\n';
		it_pair = it_prop->kv.begin();
		while (it_pair != it_prop->kv.end()) {
			cout<<it_pair->name<<" = "<<it_pair->value<<'\n';
			++it_pair;
		}
		++it_prop;			
		cout<<'\n';
	}
}

} // namespace _24k
