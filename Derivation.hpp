#include "lex.hpp"
#include<vector>
#include<iostream>
class Derivation {
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
		Derivation::ACTIONTYPE action;
		int state;
		Derivation::character c;
		SLRtabletuple();
	};
	SLRtabletuple** ACTIONTABLE;
	SLRtabletuple** GOTOTABLE;
	bool **firstset;
	bool** followset;
	std::vector<bool> blankset;
	typedef std::vector<character> characterlist;
	typedef std::vector<std::string> strlist;
	std::vector<std::string> nonterminal, terminal;
	std::vector<characterlist> statelist;
	std::string lexcode;
	derivate start;
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
	bool clequal(characterlist c1, characterlist c2);
	bool character_is_existed(characterlist cl, character c);
	bool is_symbol(char s);
	bool createderivation(std::string token, std::string code, int& i, derivate** p, derivate** q, derivate** r);
	bool is_terminal(std::string s);
	int GOTO(int state, std::string s);
	int getterminalpos(std::string s);
	int getnonterminalpos(std::string s);
	void Genestatetable();
public:
	Derivation(std::string s);
	void per_process();
	std::string  visitderivate();
	bool Scanner();
	void Geneactiontable();
	void printstate();
	bool LR(lex &lexobject);
	bool LR(strlist sl);
	void loadtokeblist(lex &lexobject);
};