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
	derivate.printstate();
	lex lex(scode);
	std::string s;
	symboltable st=symboltable();
	while (lex.Scanner()) {
		st.Scanner(lex.token);
		tokenlist.push_back(lex.token);
	}
	std::cin >> scode;

}