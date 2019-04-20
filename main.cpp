#include <iostream>
#include <fstream>
#include "lex.hpp"
#include <sstream>
#include <vector>
#include <stdlib.h>
#include "symboltable.hpp"
//从文件读入到string里
std::string readFileIntoString(char* filename)
{
	std::ifstream ifile(filename);
	//将文件读入到ostringstream对象buf中
	std::ostringstream buf;
	char ch;
	while (buf && ifile.get(ch))
		buf.put(ch);
	//返回与流对象buf关联的字符串
	return buf.str();
}


int main() {
	std::string scode;
	std::vector<lex::Token> tokenlist;
	char* fn = (char*)"main.cpp";
	scode = readFileIntoString(fn);
	lex lex(scode);
	lex.per_process();
	while (const int i = 0) {

	}
	std::cout << lex.processedCode << std::endl;
	while (lex.Scanner()) {
		tokenlist.push_back(lex.token);
		std::cout <<"( "<< lex.token.typeset<< ", " << lex.token.val<<", " <<lex.token.syn<<")"<< std::endl;
	}
	std::cout << tokenlist.size() << std::endl;
}