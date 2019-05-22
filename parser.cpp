#include "parser.hpp"
parser::attribute::attribute()
{
	offset = 0;
	addr = 0;
	width = 0;
	instr = 0;
	type = symboltable::VOID;
	state = 0;
	op = "";
	type = symboltable::typeset::VOID;
}

parser::parser(std::string s)
{
	lexcode = s;
	statelist.push_back(characterlist());
	statelist[0].push_back(character(0, 0, 0));
	terminal.push_back("$");
	attribute firststate;
	firststate.state = 0;
	statestack.push_back(firststate);
	firstset = nullptr;
	followset = nullptr;
	GOTOTABLE = nullptr;
	ACTIONTABLE = nullptr;
	ignore = false;
	extokenstate.push_back(* new symboltable::ExToken);
	top = 0; //State Stack Top
	tempval = 0;
	instrpos = -1;

}


parser::derivate* parser::createnewnode(std::string token)
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

bool parser::createderivation(std::string token, std::string code, int& i, derivate** p, derivate** q, derivate** r)
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
//Scanner derivation and create table
bool parser::Scanner()
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

	//SDT action
	auto iter = terminal.begin();
	while (iter != terminal.end()) {
		if ((*iter)[0] == 'M') {
			bool flag = true;
			for (auto i = 2; i < iter->length(); ++i) {
				if (iter->at(i) == '_') {
					nonterminal.push_back(*iter);
					p->brother = createnewnode(*iter);
					p = p->brother;
					p->son = createnewnode("blank");
					iter = terminal.erase(iter);

					flag = false;
					break;
				}
			}
			if (flag) {
				iter++;
			}
		}
		else
		{
			iter++;
		}
	}

	//Initial Sheet
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

bool parser::is_symbol(char s)
{
	std::string str = "+-=*/%&()[]{}:,<>;|!~";
	for (int i = 0; i < str.length(); i++) {
		if (str[i] == s)
			return true;
	}
	return false;
}

bool parser::is_terminal(std::string s)
{
	for (int i = 0; i < nonterminal.size(); i++) {
		if (nonterminal[i] == s || s == "blank")
			return false;
	}
	return true;
}

std::string parser::visitderivate()
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

void * parser::Temp(int type)
{
	switch (type)
	{
	case symboltable::typeset::VOID: {
		void* p = new char;
		return p;
		break; }
	case symboltable::typeset::INT: {
		int* p = new int;
		return p;
		break; }
	case symboltable::typeset::FLOAT: {
		float* p = new float;
		return p;
		break; }
	default:
		break;
	}

	tempval++;
	void* p= new char;
	return  p;
}

parser::INSTR::INSTR(std::string a, int b)
{
	left = a;
	right = b;
}

void parser::gen(std::string s, int instr)
{
	instrlist.push_back(parser::INSTR(s, instr));
}

void parser::gen(std::string s)
{
	instrlist.push_back(parser::INSTR(s,-1));
}

void parser::backpatch(leballist list, int instr)
{
	for (int i = 0; i < list.size(); i++) {
		if (instrlist.at(list[i]).right==-1)
			instrlist.at(list[i]).right = instr;
	}
}

parser::leballist parser::merge(leballist l1, leballist l2)
{
	int a = l1.size();
	int b = l2.size();
	for (int i = 0; i < a; i++) {
		int flag = false;
		for (int j = 0; j < b; j++) {
			if (l1[i] == l2[i])
			{
				flag = true;
				break;
			}
		}
		if (!flag) {
			l2.push_back(l1[i]);
		}
	}
	return l2;
}

parser::derivate* parser::getcharacter(parser::character c)
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

bool parser::terminal_is_exist(std::string s)
{
	for (int i = 0; i < terminal.size(); i++) {
		if (s == terminal[i])
			return true;
	}
	return false;
}

bool parser::closure(parser::characterlist& I)
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

bool parser::has_blank(strlist s)
{
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == "blank")
			return true;
	}
	return false;
}

bool parser::has_blank(derivate* p)
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

bool parser::clequal(characterlist c1, characterlist c2)
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

parser::strlist parser::First(strlist s)
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

parser::strlist parser::First(std::string s)
{
	strlist s2;
	s2.push_back(s);
	return First(s2);
}

parser::strlist parser::First(derivate *p)
{
	strlist s2;
	for (p ; p != nullptr; p = p->next) {
		s2.push_back(p->NAME);
	}
	return First(s2);
}

parser::strlist parser::Follow(std::string s)
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

parser::strlist parser::merge(strlist s1, strlist s2)
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

parser::strlist parser::merge(strlist s1, std::string s2)
{
	strlist s3;
	s3.push_back(s2);
	return merge(s1, s3);
}

parser::strlist parser::set(strlist s)
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

bool parser::is_alpha(char s)
{
	if ((s >= 'a' && s <= 'z') || (s >= 'A' && s <= 'Z') || s == '_') {
		return true;
	}
	else
		return false;
}

bool parser::is_digit(char s)
{
	if (s >= '0' && s <= '9') {
		return true;
	}
	else
		return false;
}

bool parser::character_is_existed(characterlist cl, character c)
{
	for (int i = 0; i < cl.size(); i++) {
		if (cl[i].left == c.left && cl[i].pos == c.pos && cl[i].right == c.right)
			return true;
	}
	return false;
}

int parser::GOTO(int state, std::string s)
{

	return 0;
}

int parser::getterminalpos(std::string s)
{
	for (int i = 0; i < terminal.size(); i++) {
		if (s == terminal[i]) {
			return i;
		}
	}
	return -1;
}

int parser::getnonterminalpos(std::string s)
{
	for (int i = 0; i < nonterminal.size(); i++) {
		if (s == nonterminal[i]) {
			return i;
		}
	}
	return -1;
}

void parser::Genestatetable()
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
					if (sl[k] == "else" && (
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

void parser::Geneactiontable()
{

	ACTIONTABLE = new SLRtabletuple * [_MAX_STATE_NUMBER];
	GOTOTABLE = new SLRtabletuple * [_MAX_STATE_NUMBER];
	SLRtabletuple* p = new SLRtabletuple[terminal.size()];
	SLRtabletuple* q = new SLRtabletuple[nonterminal.size()];
	ACTIONTABLE[0] = p;
	GOTOTABLE[0] = q;
	Genestatetable();
}

std::string parser::printstate()
{
	std::stringstream s;
	s << "STATELIST:" << '\n';
	for (int i = 0; i < statelist.size(); i++) {
		s << "state:" << i;
		for (int j = 0; j < statelist[i].size(); j++) {
			s << " (" << statelist[i][j].left << "," << statelist[i][j].right << "," << statelist[i][j].pos << ")";
		}
		s << std::endl;
	}
	s << std::endl<<"TERMINAL:\n";
	for (int i = 0; i < terminal.size(); i++) {
		s << terminal[i] << "  ";
	}
	s << std::endl;
	s << std::endl << "NONTERMINAL:\n";
	for (int i = 0; i < nonterminal.size(); i++) {
		s << nonterminal[i] << "  ";
	}
	s << std::endl;
	s << std::endl << "ACTIONTABLE:\n";
	for (int i = 0; i < statelist.size(); i++) {
		s << "state:" << i<<" ";
		for (int j = 0; j < terminal.size(); j++) {
			if (ACTIONTABLE[i][j].action == SHIFT) {
				s << "S";
				s << ACTIONTABLE[i][j].state << " ";

			}
			else if (ACTIONTABLE[i][j].action == REDUCE) {
				s << "R";
				s << ACTIONTABLE[i][j].c.left<<","<< ACTIONTABLE[i][j].c.right << " ";
			}
			else if (ACTIONTABLE[i][j].action == ACCEPT) {
				std ::cout << "ACC";
			}
			else
			{
				s << "E ";
			}
		}
		s << "GOTO  ";
		for (int j = 0; j < nonterminal.size(); j++) {
			s << GOTOTABLE[i][j].state << " ";
		}
		s << std::endl;
	}
	s << std::endl << "FOLLOWSET:\n";
	for (int i = 0; i < nonterminal.size(); i++) {
		s << nonterminal[i] << ": ";
		for (int j = 0; j < terminal.size(); j++) {
			s << followset[i][j]<< " ";
		}
		s << std::endl;
	}
	s << std::endl << "FIRSTSET:\n";
	for (int i = 0; i < nonterminal.size(); i++) {
		s << nonterminal[i] << ": ";
		for (int j = 0; j < terminal.size(); j++) {
			s << firstset[i][j] << " ";
		}
		s << std::endl;
	}
	s << std::endl << "BLANKSET:\n";
	for (int i = 0; i < nonterminal.size(); i++) {
		s << nonterminal[i] << ": ";
		s << blankset[i] << std::endl;
	}

	std::string ss;
	s >> ss;
	return ss;
}

std::string parser::GenerateCode()
{
	std::stringstream s;

	for (int i = 0; i < instrlist.size(); i++) {
		if (instrlist[i].right == -1) {
			s << i << ":" << instrlist[i].left  << std::endl;
		}
		else
		{
			s << i << ":" << instrlist[i].left +" "+ std::to_string(instrlist[i].right) << std::endl;
		}
		
	}
	std::string ss=s.str();
	return ss;
}

void parser::SDTaction()
{
	symboltable::ExToken *extoken = &extokenstate[extokenstate.size() - 1];
	int choose_0=0, choose_1=0;

	std::string CHOOSE = statestack[statestack.size() - 1].NAME;

	if (CHOOSE[0]=='M'&& !is_terminal(CHOOSE)) {
		char temp1[6], temp2[6];
		int i = 1;
		while (CHOOSE[i] != '_') {
			temp1[i-1] = CHOOSE[i];
			i++;
		}
		i++;
		int j = 0;
		for (i; i < CHOOSE.length(); i++) {
			temp2[j] = CHOOSE[i];
			j++;
		}
		choose_0 = std::atoi(temp1);
		choose_1 = std::atoi(temp2);
	}
	else if (CHOOSE=="G")
	{
		choose_0 = 7;
		choose_0 = 3;
	}
	else
	{
		return;
	}


	switch (choose_0)
	{
	case 0:
		switch (choose_1)
		{
		case 0:
			instrpos++;
			backpatch(statestack[top - 1].nextlist, instrpos);
			break;
		case 1:
			statestack[top].instr = ++instrpos;
			backpatch(statestack[top - 1].nextlist, statestack[top].instr);
			break;
		default:
			break;
		}
		break;
	case 1: 
		switch (choose_1)
		{
		case 0:
			statestack[top].offset = 0;
			break;
		default:
			break;
		}
		break;
	case 2:
		switch (choose_1)
		{
		case 0:
			extoken->NAME = statestack[top-2].NAME;
			extoken->TVAL = (symboltable::typeset) statestack[top - 3].type;
			extoken->CAT = symboltable::VAL;
			extoken->width = statestack[top - 3].width;
			extoken->addr = statestack[top - 2].addr;
			st->SetType(*extoken);
			break;
		default:
			break;
		}
		break;
	case 3:  //M2
		switch (choose_1)
		{
		case 0:
			extoken->TVAL = statestack[top - 1].type;
			extoken->width = statestack[top - 1].width;
			break;
		case 1:
			statestack[top - 3].type = statestack[top - 1].type;
			statestack[top - 3].width = statestack[top - 1].width;

			break;
		case 2:
			extoken->TVAL = symboltable::STRUCT;

			break;
		case 3:
			statestack[top - 3].type = statestack[top - 1].type;
			statestack[top - 3].width = statestack[top - 1].width;

			break;
		default:
			break;
		}
		break;
	case 4:  //M3
		switch (choose_1)
		{
		case 0:
			statestack[top - 1].type=symboltable::INT;
			statestack[top - 1].width = 4;
			break;
		case 1:
			statestack[top - 1].type = symboltable::FLOAT;
			statestack[top - 1].width = 8;
			break;
		case 2:
			statestack[top - 1].type = symboltable::BOOL;
			statestack[top - 1].width = 1;
			break;
		case 3:
			statestack[top - 1].type = symboltable::DOUBLE;
			statestack[top - 1].width = 16;
			break;
		default:
			break;
		}
		break;
	case 5:
		switch (choose_1)
		{
		case 0:
			statestack[top - 4].type = symboltable::ARRAY;
			extoken->UP.insert(extoken->UP.begin(), statestack[top - 3].value.intval);
			statestack[top - 4].width = statestack[top - 3].value.intval * statestack[top - 1].width;
			break;
		case 1:
			statestack[top].type = extoken->TVAL;
			statestack[top].width = extoken->width;
			extoken->ARRTVAL = extoken->TVAL;
			break;
		default:
			break;
		}
		break;
	case 6:
		switch (choose_1)
		{
		case 0: {
			instrpos++;
			int posval = st->get(statestack[top - 4].NAME);
			if (posval == -1) {
				throw Error::stateError::identifer_not_defined;
			}
			statestack[top - 4].addr = posval;
			gen(st->get(statestack[top - 4].addr) + " = " + st->get(statestack[top - 2].addr)); 
		}
			break;
		case 1:
			statestack[top].instr = instrpos+1;
			break;
		case 2:
			backpatch(statestack[top-4].truelist, statestack[top-2].instr);
			statestack[top - 6].nextlist = merge(statestack[top-4].falselist, statestack[top-1].nextlist);
			break;
		case 3:
			backpatch(statestack[top - 3].nextlist, statestack[top - 2].instr);
			statestack[top - 3].nextlist, statestack[top - 1].nextlist;
			break;
		case 4:
			statestack[top - 4].nextlist = merge(statestack[top - 4].nextlist, statestack[top - 2].nextlist);
			st->pp = st->pp->prev;

			break;
		case 5:
			statestack[top].instr = ++instrpos;
			gen("goto ", statestack[top - 6].instr);
			backpatch(statestack[top - 1].nextlist, statestack[top - 6].instr);
			backpatch(statestack[top - 4].truelist, statestack[top - 2].instr);
			statestack[top - 7].nextlist = statestack[top - 4].falselist;
			break;
		case 6:
			statestack[top].instr = ++instrpos ;
			statestack[top].nextlist.clear();
			statestack[top].nextlist.push_back(instrpos);
			gen("goto ");
			break;
		case 7:
			backpatch(statestack[top - 8].truelist, statestack[top -6].instr);
			backpatch(statestack[top - 8].falselist, statestack[top - 2].instr);
			statestack[top - 10].nextlist = merge(statestack[top - 1].nextlist, merge(statestack[top - 4].nextlist, statestack[top - 5].nextlist));
			break;
		case 8:
			backpatch(statestack[top - 3].nextlist, statestack[top - 2].instr);
			statestack[top - 3].nextlist = merge(statestack[top - 1].nextlist, statestack[top - 3].nextlist);
			break;
		default:
			break;
		}
		break;
	case 7:
		switch (choose_1)
		{
		case 0:
			statestack[top].instr = instrpos+1;
			statestack[top].nextlist.push_back(instrpos);
			gen("goto ");
			break;
		case 1:
			statestack[top].instr = ++instrpos;
			break;
		case 2:
			backpatch(statestack[top-4].falselist, statestack[top - 2].instr);
			statestack[top - 4].nextlist = merge(statestack[top - 4].nextlist, statestack[top - 1].nextlist);
			break;
		case 3:
			statestack[top].falselist.clear();
			statestack[top].truelist.clear();
			statestack[top].nextlist.clear();

			//statestack[top].instr = ++instrpos;
			break;
		default:
			break;
		}
		break;
	case 8:
		switch (choose_1)
		{
		case 0:
			statestack[top].instr = instrpos + 1;
			break;
		case 1:
			backpatch(statestack[top - 4].falselist, statestack[top - 2].instr);
			statestack[top - 4].truelist = merge(statestack[top - 4].truelist, statestack[top - 1].truelist);
			statestack[top - 4].falselist = statestack[top - 1].falselist;
			break;
		case 2:
			break;
		case 3:
			backpatch(statestack[top - 4].truelist, statestack[top - 2].instr);
			statestack[top - 4].truelist = statestack[top - 1].truelist;
			statestack[top - 4].falselist = merge(statestack[top - 4].falselist, statestack[top - 1].falselist);
			break;
		case 4:
			statestack[top - 2].truelist = statestack[top - 1].falselist;
			statestack[top - 2].falselist = statestack[top - 1].truelist;
			break;
		case 5:
			statestack[top - 3].truelist.clear();
			statestack[top - 3].falselist.clear();
			statestack[top - 3].truelist.push_back(++instrpos);
			statestack[top - 3].falselist.push_back(++instrpos);
			gen("if " + st->get(statestack[top - 3].addr) +" "+ statestack[top - 2].op+ " "+ st->get(statestack[top - 1].addr) + " goto ");
			gen("goto ");
			break;
		case 6:
			statestack[top - 1].truelist.clear();
			statestack[top - 1].falselist.clear();
			statestack[top - 1 ].truelist.push_back(++instrpos);
			gen("goto ");
			break;
		case 7:
			statestack[top - 1].truelist.clear();
			statestack[top - 1].falselist.clear();
			statestack[top - 1].falselist.push_back(++instrpos);
			gen("goto ");
			break;
		case 8:
			statestack[top - 3].falselist = statestack[top - 2].falselist;
			statestack[top - 3].truelist = statestack[top - 2].truelist;
			break;
		default:
			break;
		}
		break;
	case 9:
		switch (choose_1)
		{
		case 0:
			
			break;
		case 1: {
			instrpos++;
			int ss = st->Temp(statestack[top - 3].type);
			gen(st->get(ss) + " = " + st->get(statestack[top - 3].addr) + " + " + st->get(statestack[top - 1].addr));
			statestack[top - 3].addr = ss;
			break; }
		case 2: {
			instrpos++;

			int ss = st->Temp(statestack[top - 3].type);
			gen(st->get(ss) + " = " + st->get(statestack[top - 3].addr) + " - " + st->get(statestack[top - 1].addr));
			statestack[top - 3].addr = ss;
			break; }
		case 3: {
			instrpos++;

			int ss = st->Temp(statestack[top - 1].type);
			gen(st->get(ss) + " = " + " minus " + st->get(statestack[top - 1].addr));
			statestack[top - 3].addr = ss;
			break; }
		default:
			break;
		}
		break;
	case 10:
		switch (choose_1)
		{
		case 0:

			break;
		case 1: {
			instrpos++;

			int ss = st->Temp(statestack[top - 3].type);
			gen(st->get(ss) + " = " + st->get(statestack[top - 3].addr) + " * " + st->get(statestack[top - 1].addr));
			statestack[top - 3].addr = ss;
			break; }
		case 2: {
			instrpos++;

			int ss = st->Temp(statestack[top - 3].type);
			gen(st->get(ss) + " = " + st->get(statestack[top - 3].addr) + " / " + st->get(statestack[top - 1].addr));
			statestack[top - 3].addr = ss;
			break;
		}
		default:
			break;
		}
		break;
	case 11:
		switch (choose_1)
		{
		case 0:
			statestack[top - 3].falselist = statestack[top - 2].falselist;
			statestack[top - 3].truelist = statestack[top - 2].truelist;
			break;
		case 1: {
			int posval = st->get(statestack[top - 1].NAME);
			if (posval != -1) {
				statestack[top - 1].addr = posval;

			}
			else
			{
				throw Error::stateError::identifer_not_defined + ":" + statestack[top - 1].NAME;
			}}
			break;
		case 2: 
			break;
		default:
			break;
		}
		break;
	case 12:
		switch (choose_1)
		{
		case 0:
			statestack[top - 1].op = statestack[top - 1].NAME;
			break;
		case 1:
			statestack[top - 1].op = statestack[top - 1].NAME;
			break;
		case 2:
			statestack[top - 1].op = statestack[top - 1].NAME;
			break;
		case 3:
			statestack[top - 1].op = statestack[top - 1].NAME;
			break;
		default:
			break;
		}
	default:
		break;
	}


}

bool parser::LR(lex::Token &token)
{
	if (token.addr == -1 && (token.syn==99 || token.syn==100)) {

		throw Error::stateError::identifer_not_defined;
	}
	bool CONTINUE=true;


	//Error recovery
	if (ignore) {
		int pos = getlrpos(token);
		if (pos == _CANT_FIND_TERMINAL) {
			return false;
		}
		for (int i = 0; i < nonterminal.size(); i++) {
			if (followset[i][pos]) {
				attribute* p = new attribute;
				p->NAME = nonterminal[i];
				p->addr = st->Temp(symboltable::typeset::VOID);
				p->state = GOTOTABLE[statestack[statestack.size() - 1].state][i].state;
				statestack.push_back(*p);
				top = statestack.size() - 1;

				SDTaction();
				ignore = false;
				break;
			}
			
		}
	}

	//LR analysis
	while (CONTINUE) {
		if (statestack[top].NAME[0] != 'M') {
			for (auto i = statestack.begin(); i < statestack.end(); i++) {
				if (i->NAME[0] == 'M')
					continue;
				as<< i->NAME << " ";
			}
			as << std::endl;

		}
		std::string a = token.val;
		int pos = getlrpos(token);
		if (pos == _CANT_FIND_TERMINAL) {
			throw Error::SyntexError::syntexerror_1;
		}
		attribute* s = &statestack[statestack.size() - 1];
		SLRtabletuple slr = ACTIONTABLE[s->state][getlrpos(token)];

		//SHIST
		if (slr.action == SHIFT) {
			attribute* s2 = new attribute;
			s2->addr = token.addr;
			s2->NAME = token.val;
			s2->state = slr.state;
			statestack.push_back(*s2);
			CONTINUE = false;
		}
		//REDUCE
		else if (slr.action == REDUCE)
		{
			if (slr.c.pos >=1 ) {
				for (int j = 0; j < slr.c.pos - 1; j++) {
					statestack.pop_back();
				}
				int tempstate = statestack[statestack.size() - 2].state;
				s = &statestack[statestack.size() - 1];
				s->state = GOTOTABLE[tempstate][slr.c.left - 1].state;
				s->NAME = nonterminal.at(slr.c.left - 1);
			}
			else
			{
				s = &statestack[statestack.size() - 1];
				attribute* s2 = new attribute;
				s2->state = GOTOTABLE[s->state][slr.c.left - 1].state;
				s2->NAME = nonterminal.at(slr.c.left - 1);
				statestack.push_back(*s2);
			}
			top = statestack.size()-1;
			SDTaction();

		}
		//ACCEPT
		else if (slr.action == ACCEPT) {
			SDTaction();
			return true;
		}
		//Error process
		else
		{
			while (true) {
				s = &statestack[statestack.size() - 1];
				for (int i = 0; i < nonterminal.size(); i++) {
					if (GOTOTABLE[s->state][i].action == SHIFT) {
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

		////Print state stack
	}
	return false;
}

void parser::SetSymboltable(symboltable &_st)
{
	st = &_st;
}

int parser::getlrpos(lex::Token token)
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

void parser::loadtokeblist(lex& lexobject)
{

}	  

parser::character::character()
{
	left = 0;
	right = 0;
	pos = 0;
}

parser::character::character(int i, int j, int k)
{
	left = i;
	right = j;
	pos = k;
}

parser::SLRtabletuple::SLRtabletuple()
{
	action = ERROR;
	state = 0;
}

parser::derivate::derivate()
{
	NAME = "";
	brother = nullptr;
	son = nullptr;
	next = nullptr;

}



