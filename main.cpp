#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include "lex.hpp"
#include "symboltable.hpp"
#include "Derivation.hpp"
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
	Derivation derivate = Derivation(str);
	derivate.Scanner();
	//std::cout<<derivate.visitderivate();
	derivate.Geneactiontable();
	//derivate.printstate();
	lex lex(scode);
	std::string s;
	symboltable st=symboltable();
	//std::cout << lex.processedCode<<std::endl;
	while (true) {
		try {
			if (!lex.Scanner()) {
				derivate.LR(lex.token);
				break;
			}
			derivate.LR(lex.token);
			//tokenlist.push_back(lex.token);
		}
		catch (std::string e) {
			std::cout << "(" << lex.line << "," << lex.pos << ") " << e << std::endl;;
		}
	}


	//std::cout << st.printsymbol();
	//std::cin >> scode;

}