#include <iostream>
#include "lex.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include "symboltable.hpp"
using namespace std;
//从文件读入到string里
string readFileIntoString(char* filename)
{
	ifstream ifile(filename);
	//将文件读入到ostringstream对象buf中
	ostringstream buf;
	char ch;
	while (buf && ifile.get(ch))
		buf.put(ch);
	//返回与流对象buf关联的字符串
	return buf.str();
}

void genetable() {

}

int main() {
	string scode;
	double a = 1.3;
	vector<lex::Token> tokenlist;
	char* fn = (char*)"main.cpp";
	scode = readFileIntoString(fn);
	lex::lex(scode);
	lex::per_process();
	symboltable::SYNBL synbl;
	
	cout << lex::processedCode << endl;
	while (lex::Scanner()) {
		tokenlist.push_back(lex::token);
		cout <<"( "<< lex::token.type<< ", " << lex::token.val<<", " <<lex::token.syn<<")"<< endl;
	}
	cout << tokenlist.size() << endl;
}