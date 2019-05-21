#include "symboltable.hpp"

symboltable::_SYNBL::_SYNBL()
{
	prev = nullptr;
	nextp = nullptr;
	_TYPEL * typel = new _TYPEL;
	_CONSL * consl = new _CONSL;
	_AINFL * ainfl = new _AINFL;
	CLEN = 0;
	tl = typel;
	cl = consl;
	al = ainfl;
}

bool symboltable::initialTYPEL()
{
	_TYPEL *TABLE = pp->tl;
	TABLE->push_back(typeltuple(VOID));
	TABLE->push_back(typeltuple(BOOL));
	TABLE->push_back(typeltuple(CHAR));
	TABLE->push_back(typeltuple(SCHAR));
	TABLE->push_back(typeltuple(USCHAR));
	TABLE->push_back(typeltuple(SHORT));
	TABLE->push_back(typeltuple(UNSIGNED));
	TABLE->push_back(typeltuple(INT));
	TABLE->push_back(typeltuple(SIGNEDINT));
	TABLE->push_back(typeltuple(FLOAT));
	TABLE->push_back(typeltuple(LONG));
	TABLE->push_back(typeltuple(UNSIGNEDLONG));
	TABLE->push_back(typeltuple(DOUBLE));
	TABLE->push_back(typeltuple(LONGDOUBLE));
	return true;
}

bool symboltable::Scanner(lex::Token &token)
{
	if (pp->tl->size() == 0) {
		initialTYPEL();
	}
	int  addr=0;
	_SYNBL *p = pp;
	bool flag = true;
	std::string str;
	symboltuple st = symboltuple();
	for (int i = 0; i < (int)p->TABLE.size(); i++) {
		if (token.syn==_IDENTIFIER)
			if (token.val == p->TABLE[i].NAME) {
				flag = false;
				addr = i;
				break;
			}
	}

	if (flag)
	{
		switch (token.syn)
		{
			case _IDENTIFIER:
				st.CAT = VAL;
				st.NAME = token.val;
				st.ADDR = nullptr;
				p->TABLE.push_back(st);
				token.addr = p->TABLE.size() - 1;
				break;
			case _NUMBER:
				st.CAT = CONS;
				st.NAME = "_CONS_";
				str = token.val;
				p->cl->push_back(std::stoi(str));
				st.ADDR = &(p->cl->at(p->cl->size()-1));
				p->TABLE.push_back(st);
				token.addr = p->TABLE.size() - 1;
				break;
			case 74: {
				st.NAME = "_SUB_BLOCK_";
				st.CAT = RECORD;
				_SYNBL * p2=new _SYNBL;
				pp = p2;
				p2->prev = p;
				st.ADDR = p2;
				p->TABLE.push_back(st);
				token.addr = p->TABLE.size() - 1;
				break;
			}
			case 75: {
				//pp = p->prev;

				break;
			}
		default:
			break;
		}

		//p->TABLE.push_back(st);
		//token.addr = &p->TABLE[p->TABLE.size() - 1];
	}
	else
	{
		token.addr = addr;
	}

	return true;
}

bool symboltable::SetType(ExToken& a)
{
	int size = pp->tl->size();
	switch (a.TVAL)
	{
	case symboltable::ARRAY: {
		ainfltuple alt;
		alt.UP = a.UP;
		if (a.ARRTVAL <= LONGDOUBLE) {
			alt.TYPE = &pp->tl->at(LONGDOUBLE);
		}
		else if (a.ARRTVAL == STRUCT) {
			bool flag = false;
			for (int i = typeset::LONGDOUBLE; i < size; i++) {
				if (pp->tl->at(i).TVAL == STRUCT)
				{
					if (((symboltuple*)(pp->tl->at(i).TPOINT))->NAME == a.constomtype) {
						flag = true;
						alt.TYPE = &pp->tl->at(i);
						break;
					}
				}
			}
			if (!flag) {
				throw Error::typeError::havenotype;
			}
		}
		alt.CLEN = a.width;
		pp->tl->push_back(typeltuple(ARRAY, &pp->al[pp->al->size()]));
		pp->al->push_back(alt);
		((symboltuple*)(a.ADDR))->ADDR = &pp->al[pp->al->size() - 1];
		break; 
	}
	case symboltable::STRUCT: {
		bool flag = false;
		for (int i = 14; i < size; i++) {
			if (pp->tl->at(i).TVAL == STRUCT)
			{
				if (((symboltuple*)(pp->tl->at(i).TPOINT))->NAME == a.constomtype) {
					flag = true;
					a.TYPE = &pp->tl[i];
					break;
				}
			}
		}
		if (!flag) {
			throw Error::typeError::havenotype;
		}
		break;
	}
	case symboltable::RETURN:
		break;
	case symboltable::SUBBLOCK:
		break;
	default:
		pp->TABLE.at(a.addr).TYP=a.TVAL;
		pp->TABLE.at(a.addr).CAT = a.CAT;
		break;
	}
	return true;
}

int symboltable::GetTypeLength(_SYNBL& p, void* addr)
{
	typeset a = ((typeltuple*)addr)->TVAL;
	if (a >= VOID && a <= USCHAR) {
		return 1;
	}
	else if (a == SHORT) {
		return 2;
	}
	else if (a >= UNSIGNED && a <= FLOAT) {
		return 4;
	}
	else if (a>=LONG && a<=DOUBLE)
	{
		return 8;
	}
	else if (a==LONGDOUBLE)
	{
		return 16;
	}
	else if (a == STRUCT)
	{
		return ((_SYNBL*)(((typeltuple*)addr)->TPOINT))->CLEN;
	}
	else if (a==ARRAY)
	{

		return ((ainfltuple*)(((typeltuple*)addr)->TPOINT))->CLEN;
	}
	return 0;
}

symboltable::symboltable()
{
	pp= new _SYNBL;
}

int symboltable::Temp(int type)
{
	TVP* tval = new TVP;
	tval->no = tvpos;
	tvpos++;
	symboltuple *p = new symboltuple();
	p->CAT =TV;
	p->NAME = "TV_" + std::to_string(tvpos);
	p->ADDR = tval;
	p->TYP = type;
	pp->TABLE.push_back(*p);
	return pp->TABLE.size()-1;
}

std::string symboltable::get(int addr)
{
	symboltuple *a = &pp->TABLE[addr];
	if (a->CAT==VAL) {
		return a->NAME;
	}
	else if (a->CAT == CONS)
	{
		int* b = (int*)(a->ADDR);
		return std::to_string(*b);
	}
	else if (a->CAT==TV)
	{
		return a->NAME;
	}
	throw Error::SyntexError::syntexerror;
}

std::string symboltable::printsymbol(int deepth, _SYNBL *p)
{
	std::string str;
	str += "deepth:" + std::to_string(deepth);
	for (int i = 0; i < p->TABLE.size(); i++) {
		if (p->TABLE[i].CAT == RECORD)
			str += "   "+printsymbol(deepth + 1, (_SYNBL*)p->TABLE[i].ADDR);
		else if (p->TABLE[i].CAT==VAL || p->TABLE[i].CAT == CONS)
		{
			str += "   (" + p->TABLE[i].NAME + "," + std::to_string(p->TABLE[i].CAT) +")";
		}
	}
	str += "\n";
	return str;
}

std::string symboltable::printsymbol()
{
	return printsymbol(0,pp);
}

int  symboltable::get(std::string val)
{
	auto j = pp->TABLE.begin();
	for (j; j < pp->TABLE.end(); j++) {
		if (j->NAME == val ) {
			if (j->TYP !=VOID )
				return std::distance(pp->TABLE.begin(),j);
			else
			{
				break;
			}
		}

	}

	for (_SYNBL* e = pp; e != nullptr; e = e->prev) {
		for (auto i = e->TABLE.begin(); i < e->TABLE.end(); i++) {
			if (i->NAME == val) {
				j = i;
				return std::distance(pp->TABLE.begin(),j);
			}
		}
	}

	return -1;
}

symboltable::ainfltuple::ainfltuple()
{
	TYPE = nullptr;
	ADDR = nullptr;
	CLEN = 0;
}

symboltable::typeltuple::typeltuple(symboltable::typeset tval, void* ptr)
{
	TVAL = tval; TPOINT = ptr;
}

symboltable::typeltuple::typeltuple(symboltable::typeset t)
{
	TVAL = t;
	TPOINT = nullptr;
}

symboltable::typeltuple::typeltuple()
{
	TVAL = INT;
	TPOINT = nullptr;
}

symboltable::symboltuple::symboltuple()
{
	NAME = "";
	TYP = 0;
	CAT = VAL;
	ADDR = nullptr;
}

symboltable::ExToken::ExToken()
{
	CAT = VAL;
	TVAL = VOID;
	ARRTVAL = VOID;
	ADDR = nullptr;
	TYPE = nullptr;
	VALUE = nullptr;
	CTP = 0;
	width = 1;
}
