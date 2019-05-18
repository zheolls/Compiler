#include "Derivation.hpp"
Derivation::Derivation(std::string s)
{
	lexcode = s;
	statelist.push_back(characterlist());
	statelist[0].push_back(character(0, 0, 0));
	terminal.push_back("$");
	statestack.push_back(0);
	firstset = nullptr;
	followset = nullptr;
	GOTOTABLE = nullptr;
	ACTIONTABLE = nullptr;
	ignore = false;
}

void Derivation::per_process()
{

}

Derivation::derivate* Derivation::createnewnode(std::string token)
{
	derivate* newnode = new derivate;
	newnode->NAME = token;
	newnode->brother = nullptr;
	newnode->son = nullptr;
	newnode->next = nullptr;
	if (newnode)
		return newnode;
	else
		return nullptr;
}

bool Derivation::createderivation(std::string token, std::string code, int& i, derivate** p, derivate** q, derivate** r)
{
	if (token == "start") {
		if (code[i + 1] == ':' && code[i + 2] == '\n') {
			derivate* newnode = &start;
			newnode->NAME = token;
			nonterminal.push_back(token);
			newnode->brother = nullptr;
			newnode->son = nullptr;
			(*p) = (*q) = (*r) = newnode;
			i += 2;
			return true;
		}

	}

	if (i < code.length() - 2) {
		if (code[i + 1] == ':' && code[i + 2] == '\n') {
			derivate* newnode = createnewnode(token);
			nonterminal.push_back(token);
			(*p)->brother = newnode;
			(*p) = newnode;
			(*q) = (*p);
			(*r) = (*p);
			i += 2;
			return true;
		}

	}
	if ((*p) == (*q)) {
		derivate* newnode = createnewnode(token);
		(*p)->son = newnode;
		(*q) = newnode;
		if (code[i + 1] == '\n')
		{
			(*r) = (*p);
			i++;
		}
		else
		{
			(*r) = (*q);
		}

	}


	else if ((*r) == (*p))
	{
		derivate* newnode = createnewnode(token);
		(*q)->brother = newnode;
		(*q) = newnode;
		if (code[i + 1] == '\n')
		{
			(*r) = (*p);
			i++;
		}
		else
		{
			(*r) = (*q);
		}

	}
	else if (code[i + 1] == '\n') {
		derivate* newnode = createnewnode(token);
		(*r)->next = newnode;
		(*r) = (*p);
		i++;
	}
	else {
		derivate* newnode = createnewnode(token);
		(*r)->next = newnode;
		(*r) = newnode;
	}
	if (!terminal_is_exist(token) && token != "blank") {
		terminal.push_back(token);
	}



	return false;
}

bool Derivation::Scanner()
{
	int i = 0;
	derivate* p = &start;
	derivate* q = &start;
	derivate* r = &start;
	std::string code = lexcode;
	std::string token = "";
	while (i < lexcode.length())
	{

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
			createderivation(token, code, i, &p, &q, &r);
			token.clear();
		}
		else if (is_symbol(code[i])) {
			token.push_back(code[i]);
			i++;
			while (is_symbol(code[i]))
			{
				token.push_back(code[i]);
				i++;
			}
			i--;
			createderivation(token, code, i, &p, &q, &r);
			token.clear();
		}


		i++;

	}
	strlist sl;
	for (int i = 0; i < terminal.size(); i++) {
		int flag = true;
		for (int j = 0; j < nonterminal.size(); j++) {
			if (nonterminal[j] == terminal[i]) {
				flag = false;
				break;
			}
		}
		if (flag)
			sl.push_back(terminal[i]);
	}
	terminal = sl;
	firstset = new bool* [nonterminal.size()];
	followset = new bool* [nonterminal.size()];
	for (int i = 0; i < nonterminal.size(); i++) {
		firstset[i] = new bool[terminal.size()];
		followset[i] = new bool[terminal.size()];
		for (int j = 0; j < terminal.size(); j++) {
			firstset[i][j] =false;
			followset[i][j] = false;
		}
		blankset.push_back(false);
	}
	//BLANKTABLE
	while (true) {
		bool flag = false;
		for (derivate* p = &start; p != nullptr; p = p->brother)
			for (derivate* q = p->son; q != nullptr; q = q->brother) {
				bool temp=false;
				if (is_terminal(q->NAME)) {
					continue;
				} 
				else if (q->NAME == "blank") {
					if (!blankset[getnonterminalpos(p->NAME)])
						flag = true;
					blankset[getnonterminalpos(p->NAME)]=true;
				}
				else {
					temp = blankset[getnonterminalpos(q->NAME)];
					for (derivate* r = q; r != nullptr; r = r->next) {
						if (is_terminal(r->NAME))
						{
							break;
						}
						else
						{
							temp = blankset[getnonterminalpos(r->NAME)] && temp;
						}
					}
					if (!blankset[getnonterminalpos(p->NAME)] && temp) {
						blankset[getnonterminalpos(p->NAME)] = true;
						flag = true;
					}
				}
			}
		if (!flag) {
			break;
		}
	}
	//FIRSTSET
	while (true) {
		bool flag = false;
		for (derivate* p = &start; p != nullptr; p = p->brother)
			for (derivate* q = p->son; q != nullptr; q = q->brother) {
				if (is_terminal(q->NAME)) {
					if (!firstset[getnonterminalpos(p->NAME)][getterminalpos(q->NAME)])
					{
						firstset[getnonterminalpos(p->NAME)][getterminalpos(q->NAME)] = true;
						flag = true;
						continue;
					}
				}
				else if (q->NAME == "blank") {
					continue;
				}
				else {
					for (derivate* r = q; r != nullptr; r = r->next) {
						if (is_terminal(r->NAME))
						{
							if (!firstset[getnonterminalpos(p->NAME)][getterminalpos(r->NAME)])
								firstset[getnonterminalpos(p->NAME)][getterminalpos(r->NAME)] = true;

							break;
						}
						else
						{
							for (int i = 0; i < terminal.size(); i++) {
								if (!firstset[getnonterminalpos(p->NAME)][i] &&firstset[getnonterminalpos(r->NAME)][i]) {
									firstset[getnonterminalpos(p->NAME)][i] = true;
									flag = true;
								}
							}
							if (!blankset[getnonterminalpos(r->NAME)]) {
								break;
							}
						}
							
					}
				}
			}
		if (!flag) {
			break;
		}
	}
	followset[getnonterminalpos("start")][getterminalpos("$")] = true;

	//FOLLOWSET
	while (true) {
		bool flag = false;
		for (derivate* p = &start; p != nullptr; p = p->brother)
			for (derivate* q = p->son; q != nullptr; q = q->brother)
				for (derivate* r = q; r != nullptr; r = r->next) {
					bool hb = has_blank(r->next);
					if (q->NAME == "blank" || is_terminal(r->NAME)) {
						continue;
					}
					else if (hb) {
						for (int i = 0; i < terminal.size(); i++) {
							if (followset[getnonterminalpos(p->NAME)][i] && !followset[getnonterminalpos(r->NAME)][i]) {
								followset[getnonterminalpos(r->NAME)][i] = true;
								flag = true;
							}
						}
					}
					strlist s3 = First(r->next);
					for (int i = 0; i < s3.size(); i++) {
						int a1 = getnonterminalpos(r->NAME);
						int a2 = getterminalpos(s3[i]);
						bool *p2= &followset[a1][a2];
						if (!*p2) {
							*p2 = true;
							flag = true;
						}
					}
					

				}
		if (!flag) {
			break;
		}
	}

	return false;
}

bool Derivation::is_symbol(char s)
{
	std::string str = "+-=*/%&()[]{}:,<>;|!~";
	for (int i = 0; i < str.length(); i++) {
		if (str[i] == s)
			return true;
	}
	return false;
}

bool Derivation::is_terminal(std::string s)
{
	for (int i = 0; i < nonterminal.size(); i++) {
		if (nonterminal[i] == s || s == "blank")
			return false;
	}
	return true;
}

std::string Derivation::visitderivate()
{
	std::string str = "";
	for (derivate* i = &start; i != nullptr; i = (*i).brother) {
		str += "alpha:" + (*i).NAME + "  beta: ";
		for (derivate* j = (*i).son; j != nullptr; j = (*j).brother) {
			str += (*j).NAME + ' ';
		}
		str += '\n';
	}
	return str;
}

Derivation::derivate* Derivation::getcharacter(Derivation::character c)
{
	int i = c.left;
	int j = c.right;
	int k = c.pos;
	derivate* p = &start;

	if (i == 0) {
		if (j == 0 && k == 0) return &start;
		else return nullptr;
	}
	else {
		for (int ii = 0; ii < i - 1; ii++) {
			if (p == nullptr)
				return nullptr;
			p = p->brother;
		}
		if (p == nullptr) return nullptr;
		else
		{
			p = p->son;
		}
		for (int ii = 0; ii < j; ii++) {
			if (p == nullptr)
				return nullptr;
			p = p->brother;
		}
		if (p == nullptr) return nullptr;

		for (int ii = 0; ii < k; ii++) {
			if (p == nullptr)
				return nullptr;
			p = p->next;
		}
	}
	return p;
}

bool Derivation::terminal_is_exist(std::string s)
{
	for (int i = 0; i < terminal.size(); i++) {
		if (s == terminal[i])
			return true;
	}
	return false;
}

bool Derivation::closure(Derivation::characterlist& I)
{
	int persize;
	while (true) {
		persize = I.size();
		for (int i = 0; i < I.size(); i++) {
			if (getcharacter(I[i]) == nullptr) continue;
			else if (is_terminal(getcharacter(I[i])->NAME)) continue;
			else
			{
				int j = 0;
				int left = getnonterminalpos(getcharacter(I[i])->NAME);
				while (true) {
					derivate* p = getcharacter(character(left + 1, j, 0));

					if (p != nullptr) {
						if (p->NAME == "blank") {
							if (!character_is_existed(I, character(left + 1, j, 1)))
								I.push_back(character(left + 1, j, 1));
						}
						else
						{
							if (!character_is_existed(I, character(left + 1, j, 0)))
								I.push_back(character(left + 1, j, 0));
						}
						j++;
					}
					else
					{
						break;
					}
				}
			}
			int j = 0;

		}
		//if (I.size() == persize) {
		//	break;
		//}
		break;
	}
	return false;
}

bool Derivation::has_blank(strlist s)
{
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == "blank")
			return true;
	}
	return false;
}

bool Derivation::has_blank(derivate* p)
{
	bool flag = true;
	for (derivate* q = p; q != nullptr; q = q->next ) {
		if (is_terminal(q->NAME))
		{
			return false;
		}
		else
			flag = flag && blankset[getnonterminalpos(q->NAME)];
	}
	return flag;
}

bool Derivation::clequal(characterlist c1, characterlist c2)
{
	if (c1.size() != c2.size())
		return false;
	for (int i = 0; i < c1.size(); i++) {
		int flag = false;
		for (int j = 0; j < c2.size(); j++) {
			if (c1[i].left == c2[j].left && c1[i].right == c2[j].right && c1[i].pos == c2[j].pos) {
				flag = true;
				break;
			}
		}
		if (!flag) {
			return false;
		}
	}
	return true;
}

Derivation::strlist Derivation::First(strlist s)
{
	strlist s2;
	
	bool flag = true;
	for (int i = 0; i < s.size(); i++) {
		if (is_terminal(s[i]))
		{
			s2 = merge(s2, s[i]);
			flag = false;
			break;
		}
		else
		{
			for (int j = 0; j < terminal.size(); j++)
			{
				if (firstset[getnonterminalpos(s[i])][j])
				{
					s2 = merge(s2, terminal[j]);
				}
				
			}
			if (!blankset[getnonterminalpos(s[i])])
				flag = false;
		}
		
	}
	if (flag) {
		s2 = merge(s2, "blank");
	}

	return set(s2);
}

Derivation::strlist Derivation::First(std::string s)
{
	strlist s2;
	s2.push_back(s);
	return First(s2);
}

Derivation::strlist Derivation::First(derivate *p)
{
	strlist s2;
	for (p ; p != nullptr; p = p->next) {
		s2.push_back(p->NAME);
	}
	return First(s2);
}

Derivation::strlist Derivation::Follow(std::string s)
{
	strlist s2;
	if (is_terminal(s)) {
		return strlist();
	}
	else
	{
		for (int i = 0; i < terminal.size(); i++) {
			if (followset[getnonterminalpos(s)][i]) {
				s2.push_back(terminal[i]);
			}
		}
	}
	return s2;
}

Derivation::strlist Derivation::merge(strlist s1, strlist s2)
{
	strlist s3;
	for (int i = 0; i < s1.size(); i++) {

	}
	int size = s2.size();
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < s1.size(); j++) {
			if (s1[j] == s2[i])
				break;
		}
		s1.push_back(s2[i]);
	}
	return set(s1);
}

Derivation::strlist Derivation::merge(strlist s1, std::string s2)
{
	strlist s3;
	s3.push_back(s2);
	return merge(s1, s3);
}

Derivation::strlist Derivation::set(strlist s)
{
	strlist s2;
	for (int i = 0; i < s.size(); i++) {
		int flag = true;
		for (int j = 0; j < s2.size(); j++) {
			if (s[i] == s2[j])
				flag = false;
			break;
		}
		if (flag) {
			s2.push_back(s[i]);
		}
	}
	return s2;
}

bool Derivation::is_alpha(char s)
{
	if ((s >= 'a' && s <= 'z') || (s >= 'A' && s <= 'Z') || s == '_') {
		return true;
	}
	else
		return false;
}

bool Derivation::is_digit(char s)
{
	if (s >= '0' && s <= '9') {
		return true;
	}
	else
		return false;
}

bool Derivation::character_is_existed(characterlist cl, character c)
{
	for (int i = 0; i < cl.size(); i++) {
		if (cl[i].left == c.left && cl[i].pos == c.pos && cl[i].right == c.right)
			return true;
	}
	return false;
}

int Derivation::GOTO(int state, std::string s)
{

	return 0;
}

int Derivation::getterminalpos(std::string s)
{
	for (int i = 0; i < terminal.size(); i++) {
		if (s == terminal[i]) {
			return i;
		}
	}
	return -1;
}

int Derivation::getnonterminalpos(std::string s)
{
	for (int i = 0; i < nonterminal.size(); i++) {
		if (s == nonterminal[i]) {
			return i;
		}
	}
	return -1;
}

void Derivation::Genestatetable()
{
	int setsize;
	closure(statelist[0]);
	for (int i = 0; i < statelist.size(); i++) {
		//SHIFT AND GOTO
		strlist s;
		setsize = statelist.size();
		for (int j = 0; j < statelist[i].size(); j++) {
			derivate* p = getcharacter(statelist[i][j]);
			if (p != nullptr&& p->NAME!="blank") {
				s = merge(s, p->NAME);
			}
		}
		s = set(s);
		if (s.size() > 0) 
		{
			for (int j = 0; j < s.size(); j++) {
				characterlist cl;
				for (int k = 0; k < statelist[i].size(); k++) {
					derivate* p = getcharacter(statelist[i][k]);
					if (p == nullptr) {
						continue;
					}
					else if (p->NAME == s[j]) {
						character c = character(statelist[i][k].left, statelist[i][k].right, statelist[i][k].pos + 1);
						cl.push_back(c);
					}
				}
				bool flag = true;
				closure(cl);
				for (int k = 0; k < statelist.size(); k++) {
					if (clequal(cl, statelist[k])) {
						if (is_terminal(s[j])) {
							ACTIONTABLE[i][getterminalpos(s[j])].action = SHIFT;
							ACTIONTABLE[i][getterminalpos(s[j])].state = k;
						}
						else
						{
							GOTOTABLE[i][getnonterminalpos(s[j])].action = SHIFT;
							GOTOTABLE[i][getnonterminalpos(s[j])].state = k;
						}
						flag = false;
						break;
					}
				}
				if (flag) {
					SLRtabletuple* p = new SLRtabletuple[terminal.size()];
					SLRtabletuple* q = new SLRtabletuple[nonterminal.size()];
					if (is_terminal(s[j])) {
						ACTIONTABLE[i][getterminalpos(s[j])].action = SHIFT;
						ACTIONTABLE[i][getterminalpos(s[j])].state = statelist.size();
					}
					else
					{
						GOTOTABLE[i][getnonterminalpos(s[j])].action = SHIFT;
						GOTOTABLE[i][getnonterminalpos(s[j])].state = statelist.size();
					}
					ACTIONTABLE[statelist.size()] = p;
					GOTOTABLE[statelist.size()] = q;
					statelist.push_back(cl);
				}
			}

		}
		//REDUCE
		for (int j = 0; j < statelist[i].size(); j++) {
			character c = statelist[i][j];
			derivate* p = getcharacter(c);
			strlist sl;
			if (p == nullptr) {
				int num = statelist[i][j].left;
				if (num == 0) {
					ACTIONTABLE[i][getterminalpos("$")].action = ACCEPT;
					continue;
				}
				c.pos--;
				int flag = false;
				if (getcharacter(c)->NAME == "blank") {
					statelist[i][j].pos--;
					flag = true;
				}
				c.pos++;
				sl = Follow(nonterminal[c.left-1]);
				for (int k = 0; k < sl.size(); k++) {
					if (sl[k] == "else" && (nonterminal[c.left-1]== "G" ||
						getcharacter(character(c.left, c.right, 0))->NAME == "if"))
						continue;
					ACTIONTABLE[i][getterminalpos(sl[k])].action = REDUCE;
					ACTIONTABLE[i][getterminalpos(sl[k])].c = statelist[i][j];
				}
				if (flag){
					statelist[i][j].pos++;
				}

			}
		}
	}
}

void Derivation::Geneactiontable()
{

	ACTIONTABLE = new SLRtabletuple * [_MAX_STATE_NUMBER];
	GOTOTABLE = new SLRtabletuple * [_MAX_STATE_NUMBER];
	SLRtabletuple* p = new SLRtabletuple[terminal.size()];
	SLRtabletuple* q = new SLRtabletuple[nonterminal.size()];
	ACTIONTABLE[0] = p;
	GOTOTABLE[0] = q;
	Genestatetable();
}

void Derivation::printstate()
{
	for (int i = 0; i < statelist.size(); i++) {
		std::cout << "state:" << i;
		for (int j = 0; j < statelist[i].size(); j++) {
			std::cout << " (" << statelist[i][j].left << "," << statelist[i][j].right << "," << statelist[i][j].pos << ")";
		}
		std::cout << std::endl;
	}

	for (int i = 0; i < terminal.size(); i++) {
		std::cout << terminal[i] << "  ";
	}
	std::cout << std::endl;
	for (int i = 0; i < nonterminal.size(); i++) {
		std::cout << nonterminal[i] << "  ";
	}
	std::cout << std::endl;

	for (int i = 0; i < statelist.size(); i++) {
		std::cout << "state:" << i<<" ";
		for (int j = 0; j < terminal.size(); j++) {
			if (ACTIONTABLE[i][j].action == SHIFT) {
				std::cout << "S";
				std::cout << ACTIONTABLE[i][j].state << " ";

			}
			else if (ACTIONTABLE[i][j].action == REDUCE) {
				std::cout << "R";
				std::cout << ACTIONTABLE[i][j].c.left<<","<< ACTIONTABLE[i][j].c.right << " ";
			}
			else if (ACTIONTABLE[i][j].action == ACCEPT) {
				std ::cout << "ACC";
			}
			else
			{
				std::cout << "E ";
			}
		}
		std::cout << "GOTO  ";
		for (int j = 0; j < nonterminal.size(); j++) {
			std::cout << GOTOTABLE[i][j].state << " ";
		}
		std::cout << std::endl;
	}

	for (int i = 0; i < nonterminal.size(); i++) {
		std::cout << nonterminal[i] << ": ";
		for (int j = 0; j < terminal.size(); j++) {
			std::cout << followset[i][j]<< " ";
		}
		std::cout << std::endl;
	}
	for (int i = 0; i < nonterminal.size(); i++) {
		std::cout << nonterminal[i] << ": ";
		for (int j = 0; j < terminal.size(); j++) {
			std::cout << firstset[i][j] << " ";
		}
		std::cout << std::endl;
	}
	for (int i = 0; i < nonterminal.size(); i++) {
		std::cout << nonterminal[i] << ": ";
		std::cout << blankset[i] << std::endl;
	}
	//for (int i = 0; i < statelist.size(); i++) {
	//	std::cout << nonterminal[i] << ": ";
	//	for (int j = 0; j < nonterminal.size(); j++) {
	//		std::cout << GOTOTABLE[i][j].state << " ";
	//	}
	//	std::cout << std::endl;
	
}

bool Derivation::LR(lex::Token &token)
{
	if (token.addr == nullptr&& (token.syn==99 || token.syn==100)) {

		throw Error::stateError::identifer_not_defined;
	}
	bool CONTINUE=true;
	//std::cout << token.val<<" ";

	//Error recovery
	if (ignore) {
		int pos = getlrpos(token);
		if (pos == _CANT_FIND_TERMINAL) {
			return false;
		}
		for (int i = 0; i < nonterminal.size(); i++) {
			if (followset[i][pos]) {
				statestack.push_back(GOTOTABLE[statestack[statestack.size() - 1]][i].state);
				ignore = false;
				break;
			}
			
		}
	}

	//LR analysis
	while (CONTINUE) {
		//for (int i = 0; i < statestack.size(); i++) {
		//	std::cout << statestack[i] << " ";
		//}
		//std::cout << std::endl;
		std::string a = token.val;
		int pos = getlrpos(token);
		if (pos == _CANT_FIND_TERMINAL) {
			throw Error::SyntexError::syntexerror_1;
		}
		int s = statestack[statestack.size() - 1];
		SLRtabletuple slr = ACTIONTABLE[s][getlrpos(token)];
		if (slr.action == SHIFT) {
			statestack.push_back(slr.state);
			CONTINUE = false;
		}
		else if (slr.action == REDUCE)
		{
			for (int j = 0; j < slr.c.pos; j++) {
				statestack.pop_back();
			}
			s = statestack[statestack.size() - 1];
			statestack.push_back(GOTOTABLE[s][slr.c.left - 1].state);
		}
		else if (slr.action == ACCEPT)
			return true;
		else //Error
		{
			while (true) {
				s = statestack[statestack.size()-1];
				for (int i = 0; i < nonterminal.size(); i++) {
					if (GOTOTABLE[s][i].action == SHIFT) {
						ignore = true;
						break;
					}
				}
				if (ignore) {
					break;
				}
				else
				{
					statestack.pop_back();
				}

			}


			throw Error::SyntexError::syntexerror;
		}
	}
	return false;
}

int Derivation::getlrpos(lex::Token token)
{
	std::string s = token.val;
	if (token.syn == _IDENTIFIER) {
		return getterminalpos("id");
	}
	else if (token.syn == _NUMBER)
	{
		return getterminalpos("int10");
	}
	else if (token.syn == _SCANNER_FINISHED) {
		return getterminalpos("$");
	}
	else
	{
		for (int i = 0; i < terminal.size(); i++) {
			if (s == terminal[i]) {
				return i;
			}
		}
	}

	return _CANT_FIND_TERMINAL;
}

void Derivation::loadtokeblist(lex& lexobject)
{

}	  

Derivation::character::character()
{
	left = 0;
	right = 0;
	pos = 0;
}

Derivation::character::character(int i, int j, int k)
{
	left = i;
	right = j;
	pos = k;
}

Derivation::SLRtabletuple::SLRtabletuple()
{
	action = ERROR;
	state = 0;
}

Derivation::derivate::derivate()
{
	NAME = "";
	brother = nullptr;
	son = nullptr;
	next = nullptr;

}
