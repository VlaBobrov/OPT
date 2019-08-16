#include "LA.h"

LA::LA()
{
	file.open("test.txt", ios_base::in);
	//file.open("test2.txt", ios_base::in);
	//file.open("test3.txt", ios_base::in);
}

void LA::Out_lexem()
{
	for (int i = 0; i < pri.size(); i++) {
		cout << rows[i] << '\t' << colum[i] << '\t' << ident[i] << '\t' << pri[i] << endl;
	}
}

void LA::Errors()
{
	for (int i = 0; i < err.size(); i++)
		cout << err[i] << '\t' << "in line " << err_row[i] << ", column" << err_col[i] << '\t' << endl;

}

void LA::Symbol_Categories()
{
	for (int i = 0; i < 255; i++) {
		if ((i > 7) && (i < 33)) SymbolCategories[i] = 0;//whitespace
		else if ((i>47) && (i<58)) SymbolCategories[i] = 1;//digit
		else if ((i>64) && (i<91)) SymbolCategories[i] = 2;//let
		else if ((i == 45) || (i == 61) || (i == 46) || (i == 59)) SymbolCategories[i] = 3;//dm1 ( . ; = - )
		else if (i == 58) SymbolCategories[i] = 4;//dm2 :=
		else if (i == 40) SymbolCategories[i] = 5;//com
		else
			SymbolCategories[i] = 6;//Недопустимые символы
	}
}

void LA::push_lexem(vector<string> vec)
{
	vec.push_back(let);
	ident.push_back(id_const);
	pri.push_back(let);
}


bool LA::Lexem_creator()
{
	temp_file = file.get();
	while (!file.eof()) {
		if (!file.is_open()) {
			cout << "Cant open or create file";
		}
		else {
			temp_char = (char)temp_file;
			switch_on = SymbolCategories[temp_file];
			switch (switch_on)
			{
			case 0:
				if (temp_file == 10) {
					row++;
					col = 1;
				}
				else if ((temp_char == '\t') || (temp_file == 11))
					col += 4;
				else col++;

				temp_file = file.get();
				break;
			case 1:
				colum.push_back(col);
				rows.push_back(row);
				while (SymbolCategories[temp_file] == 1) {
					temp_char = (char)temp_file;
					let += temp_char;
					col++;
					temp_file = file.get();
				}

				if (!(find(constants.begin(), constants.end(), let) != constants.end())) {
					constants.push_back(let);
					ident.push_back(id_const);
					id_const++;
				}
				else {
					int i = 0;
					while (pri[i] != let) { i++; }
					ident.push_back(ident[i]);
				}
			

			
				pri.push_back(let);
				let.clear();
				break;
			case 2:
				colum.push_back(col);
				rows.push_back(row);
				while ((SymbolCategories[temp_file] == 2) || (SymbolCategories[temp_file] == 1)) {
					temp_char = (char)temp_file;
					let += temp_char;
					col++;
					temp_file = file.get();
				}

				if (((let == "PROGRAM") || (let == "CONST") || (let == "BEGIN") || (let == "END"))) {
					if (!(find(key_words.begin(), key_words.end(), let) != key_words.end())) {
						key_words.push_back(let);
						ident.push_back(id_key);
						id_key++;
					}
					else {
						int i = 0;
						while (pri[i] != let) { i++; }
						ident.push_back(ident[i]);
					}
				}
				else if (!(find(identif.begin(), identif.end(), let) != identif.end())) {
					identif.push_back(let);
					ident.push_back(id_ident);
					id_ident++;
				}
				else {
					int i = 0;
					while (pri[i] != let) { i++; }
					ident.push_back(ident[i]);
				}
				pri.push_back(let);
				let.clear();
				break;
			case 3:
				colum.push_back(col);
				rows.push_back(row);
				temp_char = (char)temp_file;
				let += temp_char;
				col++;
				id_dm = temp_file;
				if (!(find(dm.begin(), dm.end(), let) != dm.end())) {
					dm.push_back(let);
					ident.push_back(id_dm);
				}
				else
					ident.push_back(id_dm);

				pri.push_back(let);
				let.clear();
				temp_file = file.get();
				break;
			case 4:
				while (temp_file == 61 || SymbolCategories[temp_file] == 4) {
					temp_char = (char)temp_file;
					let += temp_char;
					col++;
					temp_file = file.get();
				}
				if ((let == ":=") && !(find(key_words.begin(), key_words.end(), let) != key_words.end())) {
					key_words.push_back(let);
					ident.push_back(id_key);
					id_key++;
					colum.push_back(col - 2);
					rows.push_back(row);
					pri.push_back(let);
				}
				else if (let == ":=") {
					int i = 0;
					while (pri[i] != let) { i++; }
					ident.push_back(ident[i]);
					colum.push_back(col - 2);
					rows.push_back(row);
					pri.push_back(let);
				}
				else
				{
					err.push_back("Lexer: Error: Illegal character detected");
					err_row.push_back(row);
					err_col.push_back(col - 1);
					flag = true;
					temp_file = file.get();
				}

				let.clear();
				break;
			case 5:
				temp_char = file.get();
				if ((temp_file == 40) && (temp_char == '*')) {
					if (!(temp_char == ')')) {
						col++;
						temp_char = file.get();

						while (!(((temp_file == 42) && (temp_char == ')')) || ((temp_file == 41) && (temp_char == '*')))) {
							if (!file.eof()) {
								if ((temp_char == '\n') || (temp_file == 10)) {
									row+=2;
									col = 3;
								}
								else if ((temp_char == '\t') || (temp_file == 11)) {
									col += 4;
								}
								else
									col += 2;
								temp_file = file.get();
								if (temp_file != 41) {
									temp_char = file.get();
								}
								if (temp_file == 41) {
									col--;
								}

							}
							else {
								err.push_back("Lex Error : Unexpected end of file(comment not closed)");
								err_row.push_back(row);
								err_col.push_back(col);
								flag = true;
								break;
							}
						}
						row -= 1;
						temp_file = file.get();
					}
				}
				else {
					err.push_back("Lexer: Error: Illegal character detected");
					err_row.push_back(row);
					err_col.push_back(col);
					flag = true;
					temp_file = file.get();
				}
				col++;
				break;
			case 6:
				err.push_back("Lexer: Error: Illegal character detected");
				err_row.push_back(row);
				err_col.push_back(col);
				col++;
				flag = true;
				temp_file = file.get();
				break;
			default:
				break;
			}
		}
	}
	return flag;
}


LA::~LA()
{
	file.close();
}
