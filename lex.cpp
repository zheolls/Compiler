#include "lex.hpp"

lex::lex(std::string s)
{
	sourceCode = s;
	codeLength = sourceCode.length();
	syn = -1;
	ptr = 0;
	per_process();
}

lex::lex()
{
	syn = 0;
	ptr = 0;
	codeLength = 0;
}

int lex::searchResearve(std::string keyword)
{
	for (int i = 0; i < _RESERVE_NUM; i++) {
		if (keyword == reserveWord[i]) {
			return i + 1;
		}
	}
	return _NOT_RESERVE_WORD;
}

bool lex::is_alpha(char s)
{
	if ((s >= 'a' && s <= 'z') || (s >= 'A' && s <= 'Z') || s == '_') {
		return true;
	}
	else
		return false;
}

bool lex::is_digit(char s)
{
	if (s >= '0' && s <= '9') {
		return true;
	}
	else
		return false;
}

bool lex::is_single_operater(char s)
{
	std::string temp = "^~.";
	for (int i = 0; i < temp.length(); i++) {
		if (s == temp[i]) {
			return true;
		}
	}
	return false;
}

bool lex::is_delimiter(char s)
{
	std::string temp = ";()\"\'[]{}\\\?:#,";
	for (int i = 0; i < (int)temp.length(); i++) {
		if (s == temp[i]) {
			return true;
		}

	}
	return false;
}

void lex::per_process()
{
	int i = 0;
	while (i < codeLength) {
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

bool lex::Scanner()
{
	std::string code = processedCode;
	token.clear();
	int i = ptr;
	if (ptr >= codeLength) {
		return false;
	}
	while (code[i] == ' ') {
		i++;
	}
	if (is_alpha(code[i]) || code[i] == '_') {
		token.push_back(code[i]);
		i++;
		while (is_alpha(code[i]) || is_digit(code[i]) || code[i] == '_')
		{
			token.push_back(code[i]);
			i++;
		}
		i--;
		syn = searchResearve(token);
		if (syn == _NOT_RESERVE_WORD) {
			syn = _IDENTIFIER;
			token.typeset = "IDENTIFIER";
			token.val = token;
		}
		else {
			token.typeset = "RESERVE";
			token.val = token;
		}
	}
	else if (code[i] == ':' && code[i + 1] == ':') {
		syn = _RESERVE_NUM + 28;
		i++;
	}
	else if (is_digit(code[i])) {
		token.push_back(code[i]);
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
			token.push_back(code[i]);
			i++;
		}
		i--;
		syn = _NUMBER;
		token.val = token;
		token.typeset = "CONSTANT";
	}
	else if (is_delimiter(code[i])) {
		token.push_back(code[i]);
		for (int j = 0; j < _DELIMETER_NUM; j++) {
			if (token == delimiterWord[j]) {
				syn = _OPERATOR_NUM + _RESERVE_NUM + 1 + j;
				break;
			}
		}
		token.typeset = "DELIMITER";
		token.val = token;
	}
	else if (code[i] == '+') {
		i++;
		if (code[i] == '=') {
			syn = _RESERVE_NUM + 3;
		}
		else if (code[i] == '+') {
			syn = _RESERVE_NUM + 2;
		}
		else {
			syn = _RESERVE_NUM + 1;
			i--;
		}
	}
	else if (code[i] == '-') {
		i++;
		if (code[i] == '-') {
			syn = _RESERVE_NUM + 5;
		}
		else if (code[i] == '>') {
			syn = _RESERVE_NUM + 7;
		}
		else if (code[i] == '=') {
			syn = _RESERVE_NUM + 6;
		}
		else {
			syn = _RESERVE_NUM + 4;
			i--;
		}
	}
	else if (code[i] == '*') {
		i++;
		if (code[i] == '=') {
			syn = _RESERVE_NUM + 9;
		}
		else {
			syn = _RESERVE_NUM + 8;
			i--;
		}
	}
	else if (code[i] == '/') {
		i++;
		if (code[i] == '=') {
			syn = _RESERVE_NUM + 11;
		}
		else {
			syn = _RESERVE_NUM + 10;
			i--;
		}
	}
	else if (code[i] == '<') {
		i++;
		if (code[i] == '=') {
			syn = _RESERVE_NUM + 13;
		}
		else if (code[i] == '<') {
			syn = _RESERVE_NUM + 14;
		}
		else {
			syn = _RESERVE_NUM + 12;
			i--;
		}
	}
	else if (code[i] == '>') {
		i++;
		if (code[i] == '=') {
			syn = _RESERVE_NUM + 16;
		}
		else if (code[i] == '>') {
			syn = _RESERVE_NUM + 17;
		}
		else {
			syn = _RESERVE_NUM + 15;
			i--;
		}
	}
	else if (code[i] == '=') {
		i++;
		if (code[i] == '=') {
			syn = _RESERVE_NUM + 19;
		}
		else {
			syn = _RESERVE_NUM + 18;
			i--;
		}
	}
	else if (code[i] == '!') {
		i++;
		if (code[i] == '=') {
			syn = _RESERVE_NUM + 21;
		}
		else {
			syn = _RESERVE_NUM + 20;
			i--;
		}
	}
	else if (code[i] == '&') {
		i++;
		if (code[i] == '&') {
			syn = _RESERVE_NUM + 23;
		}
		else {
			syn = _RESERVE_NUM + 22;
			i--;
		}
	}
	else if (code[i] == '|') {
		i++;
		if (code[i] == '|') {
			syn = _RESERVE_NUM + 25;
		}
		else {
			syn = _RESERVE_NUM + 24;
			i--;
		}
	}
	else if (code[i] == '%') {
		i++;
		if (code[i] == '=') {
			syn = _RESERVE_NUM + 27;
		}
		else {
			syn = _RESERVE_NUM + 26;
			i--;
		}
	}
	else if (code[i] == '~') {
		syn = _RESERVE_NUM + 29;
	}
	else if (code[i] == '.') {
		syn = _RESERVE_NUM + 30;
	}
	else if (code[i] == '^') {
		syn = _RESERVE_NUM + 31;
		i++;
	}
	else if (code[i] == '$') {
		syn = _SCANNER_FINISHED_SYN_NUM;
		return false;
	}
	else {
		throw Error::formError::unkownerror_1;
	}
	if (syn > _RESERVE_NUM && syn <= _OPERATOR_NUM + _RESERVE_NUM) {
		token.typeset = "OPERATOR";
		token.val = operatorWord[syn - _RESERVE_NUM - 1];
	}
	//else if (syn >= _RESERVE_NUM + _OPERATOR_NUM && syn <= _OPERATOR_NUM + _RESERVE_NUM + _DELIMETER_NUM) {
	//	token.typeset = delimiterWord[syn - _RESERVE_NUM - _OPERATOR_NUM - 1];
	//	token.val = "__";
	//}

	token.syn = syn;
	i++;
	ptr = i;
	return true;
}