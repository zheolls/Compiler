#include "Config.hpp"
#include "Error.hpp"
#include <string>
namespace lex {
	int syn;
	int ptr = 0;
	int codeLength;	
	std::string sourceCode;
	std::string processedCode;
	std::string Token;
	void lex(std::string s) {
		sourceCode = s;
		codeLength = sourceCode.length();
	}
	static std::string reserveWord[32] = {
		"auto", "break", "case", "char", "const", "continue",
		"default", "do", "double", "else", "enum", "extern",
		"float", "for", "goto", "if", "int", "long",
		"register", "return", "short", "signed", "sizeof", "static",
		"struct", "switch", "typedef", "union", "unsigned", "void",
		"volatile", "while"
	};
	static std::string operatorWord[36] = {
	   "+","-","*","/","<","<=",">",">=","=","==",
	   "!=",";","(",")","^",",","\"","\'","#","&",
	   "&&","|","||","%","~","<<",">>","[","]","{",
	   "}","\\",".","\?",":","!"
	};
	static std::string identifier = { "" };
	int searchResearve(std::string keyword) {
		for (int i = 0; i < 32; i++) {
			if (keyword == reserveWord[i]) {
				return i+1;
			}
		}
		return _NOT_RESERVE_WORD;
	}
	void setsyn(int i) {
		syn = i;
	}
	bool is_alpha(char s) {
		if ((s >= 'a' && s <= 'z') || (s >= 'A' && s <= 'Z') || s == '_') {
			return true;
		}
		else
			return false;
	}
	bool is_digit(char s) {
		if (s >= '0' && s <= '9')  {
			return true;
		}
		else
			return false;
	}
	bool is_single_operater(char s) {
		std::string temp = "+-*/;()^,\"\'~#%[]{}\\.\?:";
		for (int i = 0; i < temp.length(); i++) {
			if (s == temp[i]) {
				return true;
			}
		}
		return false;
	}
	void per_process() {
		int i = 0;
		while(i<codeLength){
			if (sourceCode[i] == '/' && sourceCode[i + 1] == '/') {
				while (sourceCode[i] != '\n')
					i++;
			}
			if (sourceCode[i] == '/' && sourceCode[i + 1] == '*') {
				i += 2;
				while (sourceCode[i] != '*' || sourceCode[i + 1] != '/') {
					i++;
					if (i == codeLength) {
						throw Error::pairError::noteError_1;
					}
				}
				i += 2;
			}
			if (sourceCode[i] == '\n' || sourceCode[i] == '\t' ||
					sourceCode[i] == '\v' || sourceCode[i] == '\r') {
				i++;
				continue;
			}
			else {
				processedCode.push_back(sourceCode[i]);
			}
			i++;
		}
		codeLength = processedCode.length();
		//processedCode.push_back('\0');
	}
	bool Scanner() {
		std::string code = processedCode;
		Token.clear();
		int i = ptr;
		if (ptr >= codeLength) {
			return false;
		}
		while (code[i] == ' ') {
			i++; 
		}
		if (is_alpha(code[i])||code[i]=='_') {
			Token.push_back(code[i]);
			i++;
			while (is_alpha(code[i]) || is_digit(code[i]) || code[i] == '_')
			{
				Token.push_back(code[i]);
				i++;
			}
			i--;
			//Token.push_back('\0');
			syn = searchResearve(Token);
			if (syn == _NOT_RESERVE_WORD) {
				syn = _IDENTIFIER;
			}
		}
		else if (is_digit(code[i])) {
			i++;
			int dotflag = 0;
			while (is_digit(code[i]) || code[i] == '.')
			{
				if (code[i] == '.' && dotflag) {
					throw Error::formError::numberError_1;
				}
				if (code[i] == '.') {
					dotflag = true;
				}
				Token.push_back(code[i]);
				i++;
			}
			i--;
			//Token.push_back('\0');
			syn = _NUMBER;
		}
		else if (is_single_operater(code[i])) {
			Token.push_back(code[i]);
			//Token.push_back('\0');
			for (int j = 0; j < _OPERATOR_NUM; j++) {
				if (Token == operatorWord[j]) {
					syn = _RESERVE_NUM + 1 + j;
					break;
				}
			}
		}
		else if (code[i] == '<') {
			i++;
			if (code[i] == '=') {
				syn = _OPERATOR_NUM+2;
			}
			else if (code[i] == '<') {
				syn = _OPERATOR_NUM + 22;
			}
			else {
				syn = _OPERATOR_NUM + 1;
				i--;
			}
		}
		else if (code[i] == '>') {
			i++;
			if (code[i] == '=') {
				syn = _OPERATOR_NUM + 4;
			}
			else if (code[i] == '>') {
				syn = _OPERATOR_NUM + 23;
			}
			else {
				syn = _OPERATOR_NUM + 3;
				i--;
			}
		}
		else if (code[i] == '!') {
			i++;
			if (code[i] == '=') {
				syn = _OPERATOR_NUM + 7;
			}
			else {
				syn = _OPERATOR_NUM + 32;
				i--;
			}
		}
		else if (code[i] == '&') {
			i++;
			if (code[i] == '&') {
				syn = _OPERATOR_NUM + 17;
			}
			else {
				syn = _OPERATOR_NUM + 16;
				i--;
			}
		}
		else if (code[i] == '=') {
			i++;
			if (code[i] == '=') {
				syn = _OPERATOR_NUM + 6;
			}
			else {
				syn = _OPERATOR_NUM + 5;
				i--;
			}
		}
		else if (code[i] == '|') {
			i++;
			if (code[i] == '|') {
				syn = _OPERATOR_NUM + 19;
			}
			else {
				syn = _OPERATOR_NUM + 18;
				i--;
			}
		}
		else if (code[i]=='$'){
			syn = _SCANNER_FINISHED_SYN_NUM;
			return false;
		}
		else {
			throw Error::formError::unkownerror_1;
		}
		i++;
		ptr = i;
		return true;
	}
}