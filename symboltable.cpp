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
	void* addr=nullptr;
	_SYNBL *p = pp;
	bool flag = true;
	std::string str;
	symboltuple st = symboltuple();
	for (int i = 0; i < (int)p->TABLE.size(); i++) {
		if (token.syn==_IDENTIFIER)
			if (token.val == p->TABLE[i].NAME) {
				flag = false;
				addr = &p->TABLE[i];
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
				break;
			case _NUMBER:
				st.CAT = CONS;
				st.NAME = "_CONS_";
				str = token.val;
				p->cl->push_back(str);
				st.ADDR = (void*)&(p->cl[p->cl->size()-1]);
				break;
			case 75: {
				_SYNBL p2;
				pp = &p2;
				p->nextp = &p2;
				p2.prev = p;
				break;
			}
			case 76: {
				pp = p->prev;

				break;
			}
		default:
			break;
		}

		p->TABLE.push_back(st);
		token.addr = &p->TABLE[p->TABLE.size() - 1];
	}
	else
	{
		token.addr = addr;
	}

	return true;
}

bool symboltable::SetType(_SYNBL& p, ExToken& a)
{
	int size = p.tl->size();
	switch (a.TVAL)
	{
	case symboltable::ARRAY: {
		ainfltuple alt;
		alt.UP = a.UP;
		if (a.TVAL <= LONGDOUBLE) {
			alt.TYPE = &p.tl->at(LONGDOUBLE);
		}
		else if (a.TVAL == STRUCT) {
			bool flag = false;
			for (int i = typeset::LONGDOUBLE; i < size; i++) {
				if (p.tl->at(i).TVAL == STRUCT)
				{
					if (((symboltuple*)(p.tl->at(i).TPOINT))->NAME == a.constomtype) {
						flag = true;
						alt.TYPE = &p.tl->at(i);
						break;
					}
				}
			}
			if (!flag) {
				throw Error::typeError::havenotype;
			}
		}
		p.tl->push_back(typeltuple(ARRAY, &p.al[p.al->size()]));
		p.al->push_back(alt);
		((symboltuple*)(a.ADDR))->ADDR = &p.al[p.al->size() - 1];
		break; 
	}
	case symboltable::STRUCT: {
		bool flag = false;
		for (int i = 14; i < size; i++) {
			if (p.tl->at(i).TVAL == STRUCT)
			{
				if (((symboltuple*)(p.tl->at(i).TPOINT))->NAME == a.constomtype) {
					flag = true;
					a.TYPE = &p.tl[i];
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
		a.TYPE = & p.tl[a.TVAL];
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
	pp = &synbl;
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
