#include "symboltable.hpp"
#include<vector>
#include<iostream>
class parser {
private:
	enum ACTIONTYPE
	{
		ACCEPT, SHIFT, REDUCE, ERROR, SYNC
	};
	struct derivate {
		std::string NAME;
		derivate* brother;
		derivate* son;
		derivate* next;
		derivate();
	};
	struct character
	{
		int left;
		int right;
		int pos;
		character();
		character(int i, int j, int k);
	};
	struct SLRtabletuple {
		parser::ACTIONTYPE action;
		int state;
		parser::character c;
		SLRtabletuple();
	};
	typedef std::vector<int> leballist;

	struct INSTR
	{
		std::string left;
		int right;
		INSTR(std::string a, int b);
	};
	std::vector<INSTR> instrlist;
	union VAL
	{
		int intval;
		float fval;
		double dval;
		bool bval;
	};
	struct attribute {
		std::string NAME; // word 
		int state;  //state
		symboltable::typeset type; //Type
		int width;  //Varible Length
		int offset;
		int instr;
		std::string op;
		leballist nextlist,truelist,falselist;
		int addr;
		VAL value;
		attribute();
	};
	std::vector<attribute> statestack;
	SLRtabletuple** ACTIONTABLE;
	SLRtabletuple** GOTOTABLE;
	bool **firstset;
	bool** followset;
	std::vector<bool> blankset;
	bool ignore;
	typedef std::vector<character> characterlist;
	std::vector<characterlist> statelist;
	int tempval;
	void* Temp(int type);
	int instrpos;
	void gen(std::string s,int instr);
	void gen(std::string s);
	void backpatch(leballist list,int instr);
	leballist merge(leballist l1, leballist l2);
	std::vector<std::string> nonterminal, terminal;

	symboltable* st; //
	typedef std::vector<std::string> strlist;
	std::string lexcode;
	derivate start;
	int top;
	derivate* getcharacter(character c);
	derivate* createnewnode(std::string token);
	strlist First(strlist s);
	strlist First(std::string s);
	strlist First(derivate* p);
	strlist Follow(std::string s);
	strlist merge(strlist s1, strlist s2);
	strlist merge(strlist s1, std::string s2);
	strlist set(strlist s);
	bool is_alpha(char s);
	bool is_digit(char s);
	bool terminal_is_exist(std::string s);
	bool closure(characterlist& I);
	bool has_blank(strlist s);
	bool has_blank(derivate* p);
	bool clequal(characterlist c1, characterlist c2);
	bool character_is_existed(characterlist cl, character c);
	bool is_symbol(char s);
	bool createderivation(std::string token, std::string code, int& i, derivate** p, derivate** q, derivate** r);
	bool is_terminal(std::string s);
	int GOTO(int state, std::string s);
	int getterminalpos(std::string s);
	int getnonterminalpos(std::string s);
	int  getlrpos(lex::Token token);
	void loadtokeblist(lex& lexobject);
	void per_process();
	void Genestatetable();
	void SDTaction();

public:
	std::vector<symboltable::ExToken> extokenstate;
	symboltable::ExToken extoken;
	parser(std::string s);
	bool Scanner();
	std::string  visitderivate();
	void printstate();
	void GenerateCode();
	void Geneactiontable();
	bool LR(lex::Token& token);
	void SetSymboltable(symboltable &_st);
};