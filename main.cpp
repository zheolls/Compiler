#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include "parser.hpp"
int a = 1;

std::string readFileIntoString(char* filename)
{
	std::ifstream ifile(filename);
	std::ostringstream buf;
	char ch;
	while (buf && ifile.get(ch))
		buf.put(ch);
	return buf.str();
}


int main() {
	std::string scode;
	std::vector<lex::Token> tokenlist;
	char* fn = (char*)"test.txt";
	scode = readFileIntoString(fn);
	char* fn2 = (char*)"derivation.txt";
	std::string str = readFileIntoString(fn2);


	symboltable st = symboltable();

	parser derivate = parser(str);
	derivate.SetSymboltable(st);
	derivate.Scanner();
	//std::cout<<derivate.visitderivate();
	derivate.Geneactiontable();
	//derivate.printstate();
	lex lex(scode);
	std::string s;
	//std::cout << lex.processedCode<<std::endl;
	while (true) {
		//try {
			if (!lex.Scanner()) {
				st.Scanner(lex.token);

				derivate.LR(lex.token);
				break;
			}

			st.Scanner(lex.token);
			derivate.LR(lex.token);
			//tokenlist.push_back(lex.token);
		//}
		//catch (std::string e) {
		//	std::cout << "(" << lex.line << "," << lex.pos << ") " << e << std::endl;;
		//}
	}
	derivate.GenerateCode();


	//std::cout << st.printsymbol();
	//std::cin >> scode;

}