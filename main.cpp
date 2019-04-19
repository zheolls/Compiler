#include <iostream>
#include "lex.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include "symboltable.hpp"
using namespace std;
//���ļ����뵽string��
string readFileIntoString(char* filename)
{
	ifstream ifile(filename);
	//���ļ����뵽ostringstream����buf��
	ostringstream buf;
	char ch;
	while (buf && ifile.get(ch))
		buf.put(ch);
	//������������buf�������ַ���
	return buf.str();
}

void genetable() {

}

int main() {
	string scode;
	int a = 1;
	int a = 2;
	double a = 1.3;
	vector<lex::Token> tokenlist;
	char* fn = (char*)"main.cpp";
	scode = readFileIntoString(fn);
	lex::lex(scode);
	lex::per_process();
	symboltable::_SYNBL synbl;
	
	cout << lex::processedCode << endl;
	while (lex::Scanner()) {
		tokenlist.push_back(lex::token);
		cout <<"( "<< lex::token.typeset<< ", " << lex::token.val<<", " <<lex::token.syn<<")"<< endl;
	}
	cout << tokenlist.size() << endl;
}