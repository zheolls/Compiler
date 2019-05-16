#include "lex.hpp"

lex::lex(std::string s)
{
	sourceCode = s;
	codeLength = sourceCode.length();
	syn = -1;
	ptr = 0;
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
	for (int i = 0; i < temp.length(); i++) {
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
	newnode->brother=nullptr;
	newnode->son=nullptr;
	newnode->next=nullptr;
	if (newnode)
		return newnode;
	else
		return nullptr;
}

bool Derivation::createderivation(std::string token,std::string code,int & i, derivate** p, derivate** q, derivate** r)
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
		(*q)->brother=newnode;
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
		(*r)->next=newnode;
		(*r) = newnode;
	}
	if (is_terminal(token) && !terminal_is_exist(token) && token!="blank") {
		terminal.push_back(token);
	}



	return false;
}

bool Derivation::Scanner()
{
	int i = 0;
	int syn;
	derivate *p=&start;
	derivate *q=&start;
	derivate *r=&start;
	std::string code = lexcode;
	std::string token = "";
	while (i<lexcode.length())
	{

		while (code[i] == ' ') {
			i++;
		}
		if (lexobject.is_alpha(code[i]) || code[i] == '_') {
			token.push_back(code[i]);
			i++;
			while (lexobject.is_alpha(code[i]) || lexobject.is_digit(code[i]) || code[i] == '_')
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
	std::string str="+-=*/%&()[]{}:,<>;|!~";
	for (int i = 0; i < str.length(); i++) {
		if (str[i] == s) 
			return true;
	}
	return false;
}

bool Derivation::is_terminal(std::string s)
{
	for (int i = 0; i < nonterminal.size(); i++) {
		if (nonterminal[i] == s || s=="blank")
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
		str +='\n';
	}
	return str;
}

Derivation::derivate* Derivation::getcharacter(Derivation::character c)
{
	int i = c.left;
	int j = c.right;
	int k = c.pos;
	if (i == 0) {
		if (j == 0 && k == 0) return &start;
		else return nullptr;
	}
	else {
		derivate* p = &start;
		for (int ii = 0; ii < i-1; ii++) {
			p = p->brother;
		}
		if (p == nullptr) return nullptr;
		else
		{
			p = p->son;
		}
		for (int ii = 0; ii < j ; ii++) {
			p = p->brother;
		}
		if (p == nullptr) return nullptr;

		for (int ii = 0; ii < k ; ii++) {
			p = p->next;
		}
	}
	return nullptr;
}

bool Derivation::terminal_is_exist(std::string s)
{
	for (int i = 0; i < terminal.size(); i++) {
		if (s == terminal[i])
			return true;
	}
	return false;
}

bool Derivation::closure(Derivation::characterlist &I)
{
	int persize;
	while (true) {
		persize = I.size();
		for (int i = 0; i < I.size(); i++) {
			if (is_terminal(getcharacter(I[i])->NAME) || getcharacter(I[i]) == nullptr) continue;
			int left=0;
			for (int j = 0; j < nonterminal.size(); j++) {
				if (nonterminal[j] == getcharacter(I[i])->NAME)
					left = j;
					break;
			}
			int j = 0;
			while (true) {
				derivate* p = getcharacter(character(left, j, 0));
				if (p != nullptr) {
					if (p->NAME == "blank")
						I.push_back(character(left, j, 1));
					else
					{
						I.push_back(character(left, j, 0));
					}
					j++;
				}
				else
				{
					break;
				}
			}
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
	strlist s2,s3,temp,temp2;
	bool flag=false;
	while(s.size()>0){
		flag = false;
		std::string ss = s[s.size() - 1];
		s.pop_back();
		if (is_terminal(ss) || ss=="blank") {
			s2.push_back(ss);
			return s2;
		}
		else {
			Derivation::derivate *p=&start;
			while (true && p!=nullptr) {
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
			for (derivate* r = q; r != nullptr; r = r->brother){
				if (r->NAME == s){
					if (r->next == nullptr || (r->next != nullptr && has_blank(First(r->next->NAME))))
						s2 = merge(s2, Follow(p->NAME));
					else if (is_terminal(r->next->NAME))
					{
						s2=merge(s2, r->next->NAME);
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
			if (s1[i] == s2[j])
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
	return merge(s1,s3);
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
	return strlist();
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
	for (int i = 0; i < statelist.size(); i++) {
		//SHIFT AND GOTO
		strlist s;
		setsize = statelist.size();
		for (int j = 0; j < statelist[i].size(); j++) {
			derivate* p = getcharacter(statelist[i][j]);
			if (p!= nullptr) {
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
					if (p->NAME == s[j]) {
						character c = character(statelist[i][k].left, statelist[i][k].right, statelist[i][k].pos + 1);
						cl.push_back(c);
					}
				}
				bool flag = true;
				closure(cl);
				for (int k = 0; k < statelist.size(); k++) {
					if (clequal(cl, statelist[k])) {
						if (is_terminal(s[i])) {
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
					if (is_terminal(s[i])) {
						ACTIONTABLE[i][getterminalpos(s[j])].action = SHIFT;
						ACTIONTABLE[i][getterminalpos(s[j])].state = statelist.size();
					}
					else
					{
						GOTOTABLE[i][getterminalpos(s[j])].action = SHIFT;
						GOTOTABLE[i][getterminalpos(s[j])].state = statelist.size();
					}
					ACTIONTABLE[statelist.size()] = p;
					GOTOTABLE[statelist.size()] = q;
					statelist.push_back(cl);
				}
			}

		}
		
		//REDUCE
		for (int j = 0; j < statelist[i].size(); j++) {
			derivate *p= getcharacter(statelist[i][j]);
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
	
	ACTIONTABLE	 = new SLRtabletuple* [_MAX_STATE_NUMBER];
	GOTOTABLE = new SLRtabletuple * [_MAX_STATE_NUMBER];
	SLRtabletuple* p = new SLRtabletuple [terminal.size()];
	SLRtabletuple* q = new SLRtabletuple [nonterminal.size()];
	ACTIONTABLE[0] = p;
	GOTOTABLE[0] = q;
	Genestatetable();
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
