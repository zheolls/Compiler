#include "Derivation.hpp"
Derivation::Derivation(std::string s)
{
	lexcode = s;
	statelist.push_back(characterlist());
	statelist[0].push_back(character(0, 0, 0));
	terminal.push_back("$");
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
	if (token == "blank") {
		(*p)->haveblank = true;
	}
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
	if (is_terminal(token) && !terminal_is_exist(token) && token != "blank") {
		terminal.push_back(token);
	}



	return false;
}

bool Derivation::Scanner()
{
	int i = 0;
	int syn;
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
	strlist s2, s3, temp, temp2;
	bool flag = false;
	while (s.size() > 0) {
		flag = false;
		std::string ss = s[s.size() - 1];
		s.pop_back();
		if (is_terminal(ss) || ss == "blank") {
			s2.push_back(ss);
			return s2;
		}
		else {
			Derivation::derivate* p = &start;
			while (true && p != nullptr) {
				if (p->NAME == ss) {
					p = p->son;
					while (p != nullptr) {
						derivate* q = p;
						while (q != nullptr) {
							temp.push_back(q->NAME);
							q = q->next;
						}
						while (temp.size()) {
							temp2.push_back(temp[temp.size() - 1]);
							temp.pop_back();
						}
						s3 = First(temp2);
						temp2.clear();
						for (int i = 0; i < s3.size(); i++) {
							if (s3[i] == "blank")
								flag = true;
							else
							{
								temp2.push_back(s3[i]);
							}
						}
						p = p->brother;
					}

					break;
				}
				p = p->brother;
			}
			if (!flag)
			{
				s2 = merge(s2, temp2);
				return s2;
			}
			else {
				s2 = merge(s2, temp2);
			}
		}
	}
	if (flag) {
		s2.push_back("blank");
	}
	return strlist();
}

Derivation::strlist Derivation::First(std::string s)
{
	strlist s2;
	s2.push_back(s);
	return First(s2);
}

Derivation::strlist Derivation::Follow(std::string s)
{
	strlist s2;
	if (s == "start") {
		s2 = merge(s2, "$");
	}
	for (derivate* p = &start; p != nullptr; p = p->brother)
		for (derivate* q = p->son; q != nullptr; q = q->brother)
			for (derivate* r = q; r != nullptr; r = r->brother) {
				if (r->NAME == s) {
					if (r->next == nullptr || (r->next != nullptr && has_blank(First(r->next->NAME))))
						s2 = merge(s2, Follow(p->NAME));
					else if (is_terminal(r->next->NAME))
					{
						s2 = merge(s2, r->next->NAME);
					}
					else
					{
						s2 = merge(s2, Follow(r->next->NAME));
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
	return s1;
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
			if (p != nullptr) {
				s = merge(s, p->NAME);
			}
		}
		s = set(s);
		if (s.size() == 0) {
			continue;
		}
		else
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
							GOTOTABLE[i][getterminalpos(s[j])].action = SHIFT;
							GOTOTABLE[i][getterminalpos(s[j])].state = k;
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
			derivate* p = getcharacter(statelist[i][j]);
			strlist sl;
			if (p == nullptr) {
				int num = statelist[i][j].left;
				if (num == 0) {
					ACTIONTABLE[i][getterminalpos("$")].action = ACCEPT;
				}
				continue;
				while (num > 0) {
					p = p->brother;
					sl = Follow(p->NAME);
					num--;
				}
				for (int k = 0; k < sl.size(); k++) {
					ACTIONTABLE[i][getterminalpos(sl[k])].action = REDUCE;
					ACTIONTABLE[i][getterminalpos(sl[k])].c = statelist[i][j];
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
}

void Derivation::LR()
{

}

Derivation::character::character()
{
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
}

Derivation::derivate::derivate()
{
}
