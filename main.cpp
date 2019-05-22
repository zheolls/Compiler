#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include "parser.hpp"
#include  <direct.h>  

std::string readFileIntoString(char* filename)
{
	std::ifstream ifile(filename);
	std::ostringstream buf;
	char ch;
	while (buf && ifile.get(ch))
		buf.put(ch);
	return buf.str();
}

void  writeStringIntoFile(char* filename,std::string s)
{
	char   buffer[100];
	
	std::fstream fs;
	std::ostringstream buf;
	fs.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);
	fs.close();
	std::ofstream ofs;
	ofs.open(filename, std::fstream::out | std::fstream::app);
	ofs << s;
	ofs.close();
}


int main(int argc, char* argv[]) 
{
	std::string helpinfo = "\
-help \n\
-i input\n\
-o output\n\
-st symbol table\n\
-sm state machine\n\
-a analysis stack\n\
-t token";
	std::string scode;
	std::vector<lex::Token> tokenlist;
	char* fn = (char*)"default.txt";
	char* fno = (char*)"output.txt";
	int flag = 0;
	char* fn2;
	std::string str;



	symboltable st = symboltable();
	try {
		fn2 = (char*)"grammar.txt";
		str = readFileIntoString(fn2);
	}
	catch (std::exception& e) {
		std::cout << "文法加载错误,检查同目录下grammar.txt文件" << e.what();
		return 0;
	}
	std::string directory;

	parser  Parser(str);
	Parser.SetSymboltable(st);
	try {
		Parser.Scanner();
		
	}
	catch (std::exception& e) {
		std::cout << "文法格式错误" << e.what();
		return 0;
	}
	Parser.Geneactiontable();
	if (argc < 1)
		std::cout<< helpinfo <<std::endl;
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			switch (argv[i][1])
			{
			case 'i':
				fn = argv[i + 1];
				break;
			case 'o': {
				
				const size_t last_slash_idx = ((std::string)(argv[0])).rfind('\\');
				if (std::string::npos != last_slash_idx)
				{
					directory = ((std::string)(fn)).substr(0, last_slash_idx);
				}
				fno = argv[i + 1];
			}
				break;
			case 't':
				flag = 4;
				break;
			case 'a':
				flag=3;
				break;
			case 's':
				if (argv[i][2] == 't') {
					flag = 1;
				}
				else if (argv[i][2] == 'm')
				{
					flag = 2;
				}
				break;
			default:
				std::cout << helpinfo << std::endl;
				return 0;
				break;
			}
		}
		else
		{
			if (argc == 2) {
				fn = argv[1];
			}
			continue;
		}
	}
	try
	{
		scode = readFileIntoString(fn);

	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
		return 0;
	}
	bool haserror=false;
	lex lex(scode);
	std::string s;
	
	while (true) {
		try {
			if (!lex.Scanner()) {
				st.Scanner(lex.token);
				Parser.LR(lex.token);
				break;
			}

			st.Scanner(lex.token);
			Parser.LR(lex.token);
			tokenlist.push_back(lex.token);
		}
		catch (std::string e) {
			haserror = true;
			std::cout << "(" << lex.line << "," << lex.pos << ") " << e << std::endl;;
		}
	}
	if (haserror) {
		return 0;
	}

	switch (flag)
	{
	case 0:
		s+= Parser.GenerateCode();
		std::cout << s;

		break;
	case 1:
		s = st.printsymbol();
		break;
	case 2:
		s = Parser.printstate();
		break;
	case 3:
		Parser.as >> s;
		break;
	case 4:
		s += "TOKENLIST:\n";
		s += "VALUE\tSYN\tTYPE";
		for (auto i = tokenlist.begin(); i < tokenlist.end(); i++) {
			s += i->val + '\t' + std::to_string(i->syn) + '\t' + i->typeset + '\n';
		}
		break;
	default:
		break;
	}

	std::string sss = s;
	try
	{
		writeStringIntoFile(fno, s);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}
	return 0;
}