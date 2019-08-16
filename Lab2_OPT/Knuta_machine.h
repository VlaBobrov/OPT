#ifndef KNUTA_MACHINE
#define KNUTA_MACHINE
#include "LA.h"
#include <map>

class Knuta_machine : public LA
{
public:
	int i = 0;
	Knuta_machine(vector<string> , vector<int> ,vector<string> , vector<string> , vector<int> , vector<int> );
	void err(int );
	int func();
	void out();
	bool check(vector<string> ,string);
	void generator();
	vector<int> id,clmn,rw;
	vector<string> lex_string,constant,identifier;
	vector<string>rgr,gen;
	~Knuta_machine();

};


#endif

