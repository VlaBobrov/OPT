#ifndef LA_H
#define LA_H
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
using namespace std;

class LA
{
public:
	LA();
	//OUT
	void Out_lexem();
	void Errors();
	void Symbol_Categories();
	void push_lexem(vector<string>);
	bool Lexem_creator();
	bool flag = false;
	vector<string>key_words, identif, constants, dm, err;
	int id_key = 401, id_ident = 601, id_const = 1001, id_dm;
	vector<int>rows, colum, ident, err_col, err_row;

	vector<string>pri;
	~LA();
protected:
	ifstream file;
	int switch_on, temp_file, col = 1, row = 1;
	int SymbolCategories[255];


	string let;
	char temp_char;

};

#endif