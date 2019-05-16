#include <iostream>
#include <fstream>
#include "lex.hpp"
#include <sstream>
#include <vector>
#include <stdlib.h>
#include "symboltable.hpp"
#include "Derivation.hpp"
int a=1;

//���ļ����뵽string��
std::string readFileIntoString(char* filename)
{
	std::ifstream ifile(filename);
	//���ļ����뵽ostringstream����buf��
	std::ostringstream buf;
	char ch;
	while (buf && ifile.get(ch))
		buf.put(ch);
	//������������buf�������ַ���
	return buf.str();
}


int main() {
	std::string scode;
	std::vector<lex::Token> tokenlist;
	char* fn = (char*)"main.cpp";
	scode = readFileIntoString(fn);
	char* fn2 = (char*)"lex.md";
	std::string str = readFileIntoString(fn2);
	Derivation derivate= Derivation(str);
	derivate.Scanner();
	//std::cout<<derivate.visitderivate();
	derivate.Geneactiontable();
	derivate.printstate();
	std::cin >> scode;
	/*lex lex(scode);
	std::string s;
	lex.searchResearve(s = "dasd");	
	lex.per_process();
	
	for (int i=0,int j=1; i < 10;i++,i-- )
	std::cout << lex.processedCode << std::endl;
	while (lex.Scanner()) {
		tokenlist.push_back(lex.token);
		std::cout <<"( "<< lex.token.typeset<< ", " << lex.token.val<<", " <<lex.token.syn<<")"<< std::endl;
	}
	std::cout << tokenlist.size() << std::endl;*/
}