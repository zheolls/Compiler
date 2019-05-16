#include "Config.hpp"
#include "Error.hpp"
#include <string>
#include <vector>
#include <iostream>

class lex {
private:
	int syn;
	int ptr;
	int codeLength;
public:
	std::string sourceCode;
	std::string processedCode;
	struct Token : std::string {
		std::string val;
		std::string typeset;
		std::string attr;
		int syn;
	}token;
	lex(std::string s);
	lex();
	const std::string reserveWord[_RESERVE_NUM] = {
		"auto", "break", "case", "char", "const", "continue",
		"default", "do", "double", "else", "enum", "extern",
		"float", "for", "goto", "if","include", "int", "long","namesapce",
		"register", "return", "short", "signed", "sizeof", "static",
		"struct", "switch", "typedef", "union", "unsigned","using", "void",
		"volatile", "while"
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
	void per_process();
	bool Scanner();
};
class Derivation {
private:
	std::string lexcode;
	lex lexobject;
	std::vector<std::string> nonterminal,terminal;
public:
	struct derivate {
		std::string NAME;
		derivate* brother;
		derivate* son;
		derivate* next;
		bool haveblank;
	};

	struct character
	{
		int left;
		int right;
		int pos;
		character();
		character(int i, int j, int k);
	};
	enum ACTIONTYPE
	{
		ACCEPT, SHIFT, REDUCE, ERROR, SYNC
	};
	struct SLRtabletuple {
		Derivation::ACTIONTYPE action;
		int state;
		Derivation::character c;
		SLRtabletuple();
	};

	SLRtabletuple **ACTIONTABLE;
	SLRtabletuple** GOTOTABLE;
	derivate start;
	typedef std::vector<character> characterlist;
	typedef std::vector<std::string> strlist;
	std::vector<characterlist> statelist;
	Derivation(std::string s);
	void per_process();
	derivate* createnewnode(std::string token);
	bool createderivation(std::string token, std::string code, int& i, derivate** p, derivate** q, derivate** r);
	bool Scanner();
	bool is_symbol(char s);
	bool is_terminal(std::string s);
	std::string  visitderivate();
	derivate* getcharacter(character c);
	bool terminal_is_exist(std::string s);
	bool closure(characterlist &I);
	bool has_blank(strlist s);
	bool clequal(characterlist c1, characterlist c2);
	strlist First(strlist s);
	strlist First(std::string s);
	strlist Follow(std::string s);
	strlist merge(strlist s1,strlist s2);
	strlist merge(strlist s1, std::string s2);
	strlist set(strlist s);
	int GOTO(int state, std::string s);
	int getterminalpos(std::string s);
	int getnonterminalpos(std::string s);
	void Genestatetable();
	void Geneactiontable();
};