#include "lex.hpp"

lex::lex(std::string s)
{
	sourceCode = s;
	codeLength = sourceCode.length();
	syn = -1;
	ptr = 0;
	line = 1;
	pos = 1;
	//per_process();
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

bool lex::is_legal(char s)
{
	std::string ss = "\t\r\n\v+1*/!%^&()<>,?:[]{=}\+-\|.; ";
	for (int i = 0; i < ss.length(); i++) {
		if (s == ss[i]) {
			return true;
		}
	}
	if (is_alpha(s) || is_delimiter(s) || is_digit(s))
		return true;
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
			processedCode.push_back(' ');
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
	token.addr = nullptr;

	std::string code = sourceCode;;
	token.clear();
	if (ptr >= codeLength) {
		token.val = "FINISHED";
		token.syn = 98;
		return false;
	}
	while (code[ptr] == ' ') {
		if (ptr >= codeLength-1)
			return false;
		ptr++; pos++;
	}
	while (true) {
		if (sourceCode[ptr] == '/' && sourceCode[ptr + 1] == '*') {
			ptr += 2;
			pos += 2;
			while (sourceCode[ptr] != '*' || sourceCode[ptr + 1] != '/') {
				ptr++;
				pos++;
				if (sourceCode[ptr] == '\n') {
					line++;
					pos = 1;
				}
				if (ptr == codeLength) {
					ptr++;
					throw Error::pairError::noteError_1;
				}
			}
			ptr += 2;
			pos += 2;
		}
		else if (sourceCode[ptr] == '/' && sourceCode[ptr + 1] == '/') {
			while (sourceCode[ptr] != '\n') {
				ptr++;
			}
			line++;
			pos = 1;
			ptr++;
		}

		else if (sourceCode[ptr] == '\n' || sourceCode[ptr] == '\t' ||
			sourceCode[ptr] == '\v' || sourceCode[ptr] == '\r') {
			pos++;
			if (sourceCode[ptr] == '\n') {
				line++;
				pos = 1;
			}
			ptr++;
		}
		else
		{
			break;
		}
	}





	
	if (is_alpha(code[ptr]) || code[ptr] == '_') {
		token.push_back(code[ptr]);
		ptr++;
		while (is_alpha(code[ptr]) || is_digit(code[ptr]) || code[ptr] == '_')
		{
			token.push_back(code[ptr]);
			ptr++;
			pos++;
		}
		if (!is_legal(code[ptr])) {
			ptr++;
			pos++;
			throw Error::formError::identiferError_1;
		}
		ptr--;
		pos--;
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
	else if (code[ptr] == ':' && code[ptr + 1] == ':') {
		syn = _RESERVE_NUM + 28;
		ptr++;
		pos++;
	}
	else if (is_digit(code[ptr])) {
		token.push_back(code[ptr]);
		ptr++; pos++;
		int dotflag = 0;
		while (is_digit(code[ptr]) || code[ptr] == '.')
		{	
			if (code[ptr] == '.' && dotflag) {
				ptr++; pos++;
				throw Error::formError::numberError_1;
			}
			if (code[ptr] == '.') {
				dotflag = true;
			}
			token.push_back(code[ptr]);
			ptr++; pos++;
		}
		if (is_alpha(code[ptr])) {
			ptr++; pos++;
			throw Error::formError::numberError_1;
		}
		ptr--; pos--;
		syn = _NUMBER;
		token.val = token;
		token.typeset = "CONSTANT";
	}
	else if (is_delimiter(code[ptr])) {
		token.push_back(code[ptr]);
		for (int j = 0; j < _DELIMETER_NUM; j++) {
			if (token == delimiterWord[j]) {
				syn = _OPERATOR_NUM + _RESERVE_NUM + 1 + j;
				break;
			}
		}
		token.typeset = "DELIMITER";
		token.val = token;
	}
	else if (code[ptr] == '+') {
		ptr++; pos++;
		if (code[ptr] == '=') {
			syn = _RESERVE_NUM + 3;
		}
		else if (code[ptr] == '+') {
			syn = _RESERVE_NUM + 2;
		}
		else {
			syn = _RESERVE_NUM + 1;
			ptr--; pos--;
		}
	}
	else if (code[ptr] == '-') {
		ptr++; pos++;
		if (code[ptr] == '-') {
			syn = _RESERVE_NUM + 5;
		}
		else if (code[ptr] == '>') {
			syn = _RESERVE_NUM + 7;
		}
		else if (code[ptr] == '=') {
			syn = _RESERVE_NUM + 6;
		}
		else {
			syn = _RESERVE_NUM + 4;
			ptr--; pos--;
		}
	}
	else if (code[ptr] == '*') {
		ptr++; pos++;
		if (code[ptr] == '=') {
			syn = _RESERVE_NUM + 9;
		}
		else {
			syn = _RESERVE_NUM + 8;
			ptr--; pos--;
		}
	}
	else if (code[ptr] == '/') {
		ptr++; pos++;
		if (code[ptr] == '=') {
			syn = _RESERVE_NUM + 11;
		}
		else {
			syn = _RESERVE_NUM + 10;
			ptr--; pos--;
		}
	}
	else if (code[ptr] == '<') {
		ptr++; pos++;
		if (code[ptr] == '=') {
			syn = _RESERVE_NUM + 13;
		}
		else if (code[ptr] == '<') {
			syn = _RESERVE_NUM + 14;
		}
		else {
			syn = _RESERVE_NUM + 12;
			ptr--; pos--;
		}
	}
	else if (code[ptr] == '>') {
		ptr++; pos++;
		if (code[ptr] == '=') {
			syn = _RESERVE_NUM + 16;
		}
		else if (code[ptr] == '>') {
			syn = _RESERVE_NUM + 17;
		}
		else {
			syn = _RESERVE_NUM + 15;
			ptr--; pos--;
		}
	}
	else if (code[ptr] == '=') {
		ptr++; pos++;
		if (code[ptr] == '=') {
			syn = _RESERVE_NUM + 19;
		}
		else {
			syn = _RESERVE_NUM + 18;
			ptr--; pos--;
		}
	}
	else if (code[ptr] == '!') {
		ptr++; pos++;
		if (code[ptr] == '=') {
			syn = _RESERVE_NUM + 21;
		}
		else {
			syn = _RESERVE_NUM + 20;
			ptr--; pos--;
		}
	}
	else if (code[ptr] == '&') {
		ptr++; pos++;
		if (code[ptr] == '&') {
			syn = _RESERVE_NUM + 23;
		}
		else {
			syn = _RESERVE_NUM + 22;
			ptr--; pos--;
		}
	}
	else if (code[ptr] == '|') {
		ptr++; pos++;
		if (code[ptr] == '|') {
			syn = _RESERVE_NUM + 25;
		}
		else {
			syn = _RESERVE_NUM + 24;
			ptr--; pos--;
		}
	}
	else if (code[ptr] == '%') {
		ptr++; pos++;
		if (code[ptr] == '=') {
			syn = _RESERVE_NUM + 27;
		}
		else {
			syn = _RESERVE_NUM + 26;
			ptr--; pos--;
		}
	}
	else if (code[ptr] == '~') {
		syn = _RESERVE_NUM + 29;
	}
	else if (code[ptr] == '.') {
		syn = _RESERVE_NUM + 30;
	}
	else if (code[ptr] == '^') {
		syn = _RESERVE_NUM + 31;
		ptr++; pos++;
	}
	else if (code[ptr] == '$') {
		syn = _SCANNER_FINISHED_SYN_NUM;
		return false;
	}
	else {
		ptr++;
		pos++;
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
	ptr++; pos++;
	return true;
}