#include "Knuta_machine.h"
#include "LA.h"


Knuta_machine::Knuta_machine(vector<string> lex, vector<int> temp,vector<string> con, vector<string> i, vector<int> c,vector<int> r)
{
	lex_string = lex;
	id = temp;
	constant = con;
	identifier = i;
	clmn = c;
	rw=r;
}

void Knuta_machine::err(int error_category) {
	switch (error_category)
	{

	case 1:
		cout << "Succesfull SA run" << endl;
		break;
	case -1:
		cout << "Error : Expected '.' after END" << "  row " << rw[i-1] << "  column  " << clmn[i-1] << endl;
		break; 
	case -2:
		cout << "Error: Expected PROGRAM" << "	row	" << rw[i] << "	column	" << clmn[i] << endl;
		break;
	case -4:
		cout << "Error: Expected identifier" << "	row	" << rw[i] << "	column	" << clmn[i] << endl;
		break;
	case -5:
		cout << "Error: Expected ;" << "	row	" << rw[i] << "	column	" << clmn[i] << endl;
		break;
	case -6:
		cout << "Error: Expected BEGIN" << "	row	" << rw[i] << "	column	" << clmn[i] << endl;
		break;
	case -7:
		cout << "Error: Expected END" << "	row	" << rw[i] << "	column	" << clmn[i] << endl;
		break;
	case -9:
		cout << "Error: Expected ':='" << "	row	" << rw[i] << "	column	" << clmn[i] << endl;
		break;
	case -10:
		cout << "Error: Expected constant" << "	row	" << rw[i] << "	column	" << clmn[i] << endl;
		break;
	case -11:
		cout << "Error: Expected '='" << "	row	" << rw[i] << "	column	" << clmn[i] << endl;
		break;
	case -12:
		cout << "Error: UnExpected 'CONST' in <constant-declaration> " << "	row	" << rw[i] << "	column	" << clmn[i] << endl;
		break;
	default:
		break;
	}

}
int Knuta_machine::func()
{
	int sw,errtype = 0;
	bool f = true;
	string s,dot="..";
	if (lex_string[i] == "PROGRAM") {
		sw = 2;
		for (i = 0; i < lex_string.size(); i++) {
				switch (sw)
				{
				case 2:
					rgr.push_back("<signal - program>");
					rgr.push_back("..<program>");
					s = to_string(id[i]);
					rgr.push_back("...."+s+" "+lex_string[i]);
					sw = 4;
					break;
				case 4:
					if (!(find(identifier.begin(), identifier.end(), lex_string[i]) != identifier.end())) {
						errtype = -4;
						return errtype;
					}
					else {
							rgr.push_back("....<procedure-identifier>");
							rgr.push_back("......<identifier>");
							s = to_string(id[i]);
							rgr.push_back("........" + s + " " + lex_string[i]);
					}
					sw = 5;
					break;
				case 5:
					if (lex_string[i] == ";") {
						s = to_string(id[i]);
						rgr.push_back("...." + s + " " + lex_string[i]);
					}
					else {
						errtype = -5;
						return errtype;
					}
					sw = 8;
					break;
				case 7:
					if (lex_string[i] == ".") {
						s = to_string(id[i]);
						rgr.push_back("...." + s + " " + lex_string[i]);
						errtype = 1;
						return errtype;
					}
					break;
				case 8://<block>
					if (lex_string[i] == "BEGIN") {
						rgr.push_back("....<block>");
						rgr.push_back("......<declarations>");
						rgr.push_back("........<constant-declaraton>");
						rgr.push_back("..........<empty>");
						s = to_string(id[i]);
						rgr.push_back("......" + s + " " + lex_string[i]);
						sw = 23;
					}
					else if((lex_string[i]=="CONST")&&(f == true)){
						f = false;
						i--;
						sw = 12;
					}
					else{errtype = -6;
						return errtype;
					}
					break;
				case 9:
					if (lex_string[i] == "BEGIN") {
						s = to_string(id[i]);
						rgr.push_back("......" + s + " " + lex_string[i]);
						sw = 23;
					}
					else if (!(find(identifier.begin(), identifier.end(), lex_string[i]) != identifier.end())) {
						if ((lex_string[i] == "CONST") && (f == false)) {
							errtype = -12;
							return errtype;
						}
						else {
							errtype = -6;
							return errtype;
						}
					}
					else {
						i--;
						sw = 19;
					}
					break;
				case 11:
					if (lex_string[i] == "END") {
						s = to_string(id[i]);
						rgr.push_back("......" + s + " " + lex_string[i]);
						sw = 7;
					}
					else {
						errtype = -7;
						return errtype;
					}
					break;
				case 12:
					if ((lex_string[i] == "CONST")&&(lex_string[i-3]=="PROGRAM")) {
						rgr.push_back("....<block>");
						rgr.push_back("......<declarations>");
						rgr.push_back("........<constant-declaratons>");
						s = to_string(id[i]);
						rgr.push_back(".........." + s + " " + lex_string[i]);
						sw = 19;
					}
					else if (lex_string[i] == "CONST") {
						errtype = -12;
						return errtype;
					}
					else {
						i--;
						sw = 19;
					}
					break;
				case 16:
					if (lex_string[i] == "BEGIN") {
						rgr.push_back(".........." + dot + "<constant-declaraton-list>");
						dot += "..";
						rgr.push_back(".........." + dot + "<empty>");
						i--;
						sw = 9;
						dot = "..";
					}
					else{
						i--;
						sw = 19;
					}
					break;
				case 19:
					if (!(find(identifier.begin(), identifier.end(), lex_string[i]) != identifier.end())) {
						if (!(find(constant.begin(), constant.end(), lex_string[i]) != constant.end())) {
							if (lex_string[i] != "BEGIN") {
								rgr.push_back(".........." + dot + "<constant-declaraton-list>");
								dot += "..";
								rgr.push_back(".........." + dot + "<empty>");
								i--;
								sw = 12;
								dot = "..";
							}
							else if (lex_string[i] == "BEGIN") {
								rgr.push_back(".........." + dot + "<constant-declaraton-list>");
								dot += "..";
								rgr.push_back(".........." + dot + "<empty>");
								i--;
								sw = 9;
								dot = "..";
							}
							else {
								i--;
								sw = 9;
							}
						}
						else {
							errtype = -4;
							return errtype;
						}
					}
					else if((lex_string[i-3]=="PROGRAM")){
						rgr.push_back("....<block>");
						rgr.push_back("......<declarations>");
						rgr.push_back("......" + dot + "<constant-declaration>");
						rgr.push_back("........" + dot + "<constant-identifier>");
						rgr.push_back(".........." + dot + "<identifier>");
						s = to_string(id[i]);
						rgr.push_back("............" + dot + s + " " + lex_string[i]);
						sw = 20;
						
					}
					else if (find(identifier.begin(), identifier.end(), lex_string[i]) != identifier.end()) {
						rgr.push_back("........" + dot + "<constant-declaraton-list>");
						rgr.push_back(".........." + dot + "<constant-declaration>");
						rgr.push_back("............" + dot + "<constant-identifier>");
						rgr.push_back(".............." + dot + "<identifier>");
						s = to_string(id[i]);
						rgr.push_back("................" + dot + s + " " + lex_string[i]);
						sw = 20;
					}
					else{
						i--;
						sw = 9;
					}
					break;
				case 20:
					if (lex_string[i] == "=") {
						s = to_string(id[i]);
						rgr.push_back("............" + dot + s + " " + lex_string[i]);
						sw = 30;
						dot += "....";
					}
					else {
						errtype = -11;
							return errtype;
					}
					break;				
				case 23:
					if (!(find(identifier.begin(), identifier.end(), lex_string[i]) != identifier.end())) {
						if (lex_string[i] == "END") {
							rgr.push_back(dot + "....<statment-list>");
							dot += "..";
							rgr.push_back(dot +"......<empty>");
							i--;
							sw = 11;
							dot = "..";
						}
						else {
							errtype = -4;
							return errtype;
						}
					}
					else {
						if (lex_string[i + 1] == ".") {
							i--;
							sw = 11;
						}
						else {
							rgr.push_back("...." + dot + "<statment-list>");
							rgr.push_back("......" + dot + "<statment>");
							rgr.push_back("........" + dot + "<variable-identifier>");
							rgr.push_back(".........." + dot + "<identifier>");
							s = to_string(id[i]);
							rgr.push_back("............" + dot + s + " " + lex_string[i]);
							sw = 27;
							dot += "..";
						}
					}
					break;
				case 27:
					if (lex_string[i] == ":=") {
						s = to_string(id[i]);
						rgr.push_back("......"+dot + s + " " + lex_string[i]);
						sw = 30;
					}
					else {
						errtype = -9;
						return errtype;
					}
					break;
				case 29:
					if (lex_string[i] == ";") {
						s = to_string(id[i]);
						rgr.push_back("......"+dot + s + " " + lex_string[i]);
						sw = 23;
					}
					else {
						errtype = -5;
						return errtype;
					}
					if (lex_string[i-2]=="=")
					{
						sw = 9;
					}
					break;
				case 30:
					if (!(find(constant.begin(), constant.end(), lex_string[i]) != constant.end())){
						errtype = -10;
						return errtype;
					}
					else {
						if (lex_string[i-1] == "-") {
							rgr.push_back("........" + dot + "<constant>");
							rgr.push_back(".........." + dot + "<unsigned-integer>");
							rgr.push_back("............" + dot + "<digit>");
							s = to_string(id[i]);
							rgr.push_back("............."+dot + s + " -"+lex_string[i]);
							sw = 29;
							dot = "..";
						}
						else {
							rgr.push_back("........" + dot + "<constant>");
							rgr.push_back(".........." + dot + "<unsigned-integer>");
							rgr.push_back("............" + dot + "<digit>");
							s = to_string(id[i]);
							rgr.push_back("............"+dot + s + " " + lex_string[i]);
							sw = 29;
							dot = "..";
						}
					}

					break;
				default:
					break;
				}
			}
		}
	
	else {
		errtype = -2;
		return errtype;
	}
	if (lex_string[i-1] != ".") {
		errtype = -1;
		return errtype;
	}
}

void Knuta_machine::out()
{
	for (int i = 0; i < rgr.size(); i++) {
		cout << '\t' << rgr[i] << endl;
	}
}


bool Knuta_machine::check(vector<string> temp,string s)
{
	for (int i = 0; i < temp.size(); i++) {
		if (temp[i] == s) { 
			return false; }
	}
	return true;
}


void Knuta_machine::generator()
{
	ofstream f("code.txt");
	string temp;
	string programIdent;
	vector<string> constIdent, varIdent;
	int j = 0, c = 0, v = 0;
	bool flag_for_task = false;
	for (int i = 0; i < rgr.size(); i++) {
		if (rgr[i] == "<signal - program>") {
			temp += "=================CODE===================\n";
			programIdent = lex_string[1];
			temp += ";PROGRAM " + programIdent;
			temp += "\n\nDATA SEGMENT\n";
			f << temp;
			cout << temp;
		}
		else if (rgr[i]=="........<constant-declaraton>") {
			if (rgr[i + 1] == "..........<empty>") {
				temp = "\tDEFAULT	mov,55\n"; j=-1;
			}
			f << temp;
			cout << temp;
		}
		else if (rgr[i] == "..........<constant-declaraton-list>") {
			if (rgr[i + 1] == "..........<empty>") {
				temp = "\tNOP\n";
			}
			else {
				if (c == 0){
					constIdent.push_back(lex_string[4 + j]);
					if (check(constIdent, programIdent)) {
						temp = "\t"+lex_string[4 + j] + "\tmov," + lex_string[6 + j] + "\n";

						c++;
					}else{
						temp = "Error:Can't be same name with ProgIdent\n";
						f << temp;
						cout << temp<<"	row	" << rw[4+j] << "	column	" << clmn[4+j] << endl;;
						j += 4;
						break;
					}

				}
				else if (c != 0) {
					constIdent.push_back(lex_string[4 + j]);
					if (check(constIdent, programIdent)) {
						constIdent.erase(constIdent.begin() + c);
						if (check(constIdent, lex_string[4 + j])) {
							temp = "\t" + lex_string[4 + j] + "\tmov," + lex_string[6 + j] + "\n";
							constIdent.push_back(lex_string[4 + j]);
							c++;
						}
						else {
							temp = "Error:ConstIdent can't be changed\n";
							f << temp;
							cout << temp<< "	row	" << rw[4+j] << "	column	" << clmn[4+j] << endl;
							j += 4;
							break;
						}
					}
					else {
						temp = "Error:Can't be same name with ProgIdent\n";
						f << temp;
						cout << temp<< "	row	" << rw[4 + j] << "	column	" << clmn[4 + j] << endl;
						j += 4;
						break;
					}
				}
				else {
					temp = "Error:Can't be same name with ProgIdent and ConstIdent \n";
					f << temp;
					cout << temp<< "	row	" << rw[4 + j] << "	column	" << clmn[4 + j] << endl;
					j += 4;
					break;
				}
			}
			f << temp;
			cout << temp;
			j += 4;
		}
		else if (rgr[i] == "......<statment-list>") {
			if (rgr[i + 1] == "..........<empty>") {
				temp = "\tNOP\n";
			}
			else {
				varIdent.push_back(lex_string[5 + j]);
				 if ((check(varIdent, programIdent))&& (check(constIdent,varIdent[v]))) {
					temp = "\t"+lex_string[5 + j] + "\tmov," + lex_string[7 + j] + "\n";
					v++;
					
				}
				else {
					if (flag_for_task == false) {
						temp = "\t" + lex_string[5 + j] + "\tmov," + lex_string[7 + j] + "\n";
						v++;
						flag_for_task = true;
					}
					else {
						temp = "Error:Can't be changed Const more than one time\n";
						f << temp;
						cout << temp << "	row	" << rw[5 + j] << "	column	" << clmn[5 + j] << endl;
						j += 4;
						break;
					}
				}
			}
				f << temp;
				cout << temp;
				j += 4;
		
		}
		else if ((rgr[i] == "......403 BEGIN")|| (rgr[i] == "......402 BEGIN")) {
			temp = "DATA ENDS\n\nCODE SEGMENT\n\tASSUME CS:CODE, DS:DATA";
			temp += "\nBEGIN:\n";
			temp += "\tPUSH EBP\n";
			temp += "\tMOV EBP, ESP\n";
			temp += "\tXOR EAX, EAX\n";
			temp += "\tMOV ESP, EBP\n";
			temp += "\tPOP EBP\n\n";
			f << temp;
			cout << temp;

		}
		
		else if ((rgr[i] == "......405 END")||(rgr[i] == "......404 END")) {
			temp = "\n\tRET 0\n";
			temp += "\tMOV AX, 4c00h\n\tINT 21h\nCODE ENDS\n\tEND BEGIN\n\n";
			f << temp;
			cout << temp;
		}
	}
	f.close();
}



Knuta_machine::~Knuta_machine()
{
	
}
