#include "LA.h"
#include "Knuta_machine.h"

int main() {
	LA la;
	la.Symbol_Categories();
	la.Lexem_creator();
	//la.Out_lexem();
	Knuta_machine knut(la.pri,la.ident,la.constants,la.identif,la.colum,la.rows);
	int type;
		type = knut.func();
	knut.out();
	knut.err(type);
	knut.generator();
	if (la.flag)
		la.Errors();
	system("pause");
	return 0;
}