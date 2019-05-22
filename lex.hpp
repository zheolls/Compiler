#pragma once
#include "Config.hpp"
#include "Error.hpp"

class lex {
private:
	int syn;
	
	std::string sourceCode;
	std::string processedCode;
	int ptr;
	int codeLength;
	const std::string reserveWord[_RESERVE_NUM] = {
	"auto", "break", "case", "char", "const", "continue",
	"default", "do", "double", "else", "enum", "extern",
	"float", "for", "goto", "if","include", "int", "long","namesapce",
	"false", "return", "short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union", "unsigned","using", "void",
	"true", "while"
	};
	const std::string operatorWord[_OPERATOR_NUM] = {
	   "+","++","+=","-","--","-=","->","*","*=","/","/=","<","<=","<<",">",">=",">>","=","==","!",
	   "!=","&","&&","|","||","%","%=","::","~",".","^"
	};
	const std::string delimiterWord[_DELIMETER_NUM] = {
		";","(",")","\"","\'","[","]","{","}","\\","\?",":","#",","
	};
	const std::string identifier = { "" };
	int searchResearve(std::string keyword);
	bool is_alpha(char s);
	bool is_digit(char s);
	bool is_single_operater(char s);
	bool is_delimiter(char s);
	bool is_legal(char s);
	void per_process();
public:
	struct Token : std::string {
		std::string val;
		std::string typeset;
		std::string attr;
		int addr;
		void* TYPE;
		int syn;
	}token;
	int line;
	int pos;
	lex(std::string s);
	lex();
	bool Scanner();
};
