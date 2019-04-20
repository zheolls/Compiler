#include "symboltable.hpp"
#include "lex.hpp"
bool symboltable::_SYNBL::put(ExToken t)
{

	return 1;
}

symboltable::_SYNBL::_SYNBL()
{
	prev = NULL;
}

symboltable::_SYNBL::_SYNBL(_SYNBL* p)
{
	prev = p;
}

symboltable::symboltuple symboltable::_SYNBL::get(std::string s)
{
	bool flag = 0;
	symboltuple found;
	for (_SYNBL* e = this; e != NULL; e = prev)
		for (int i = 0; i < this->TABLE.size(); i++) {
			if (this->TABLE.at(i).NAME == s) {
				flag = true;
				found = this->TABLE.at(i);
				break;
			}
		}
	if (flag) {
		return found;
	}
	else
		return symboltuple();
}

bool symboltable::_TYPEL::put(typeltuple t)
{
	TABLE.push_back(t);
	return false;
}

void symboltable::_TYPEL::clear()
{
	TABLE.clear();
}

bool symboltable::is_existed(_SYNBL p, ExToken a)
{
	return false;
}

void symboltable::inital(_TYPEL& q)
{
	q.put(typeltuple(VOID, NULL));
	q.put(typeltuple(BOOL, NULL));
	q.put(typeltuple(CHAR, NULL));
	q.put(typeltuple(SCHAR, NULL));
	q.put(typeltuple(USCHAR, NULL));
	q.put(typeltuple(SHORT, NULL));
	q.put(typeltuple(UNSIGNED, NULL));
	q.put(typeltuple(INT, NULL));
	q.put(typeltuple(SIGNEDINT, NULL));
	q.put(typeltuple(FLOAT, NULL));
	q.put(typeltuple(LONG, NULL));
	q.put(typeltuple(UNSIGNEDLONG, NULL));
	q.put(typeltuple(DOUBLE, NULL));
	q.put(typeltuple(LONGDOUBLE, NULL));
	q.put(typeltuple(ARRAY, NULL));
	q.put(typeltuple(STRUCT, NULL));
	q.put(typeltuple(RETURN, NULL));
}

bool symboltable::Generate_symbol_table(_SYNBL& p, _TYPEL& q, std::vector<ExToken> a, int& pos, int& length) {
	p.tl = &q;
	if (pos >= a.size()) {
		return false;
	}

	while (a[pos].TVAL != RETURN) {
		if (symboltable::is_existed(p, a[pos]) && a[pos].CAT != FUNC) {
			throw Error::stateError::statement_error;
		}
		switch (a[pos].CAT)
		{
		case symboltable::FUNC: //函数
			break;
		case symboltable::CONS: //常量
		{
			int addr = GetTypeAddr(p, q, a, pos);
			p.TABLE.push_back(symboltuple(a[pos].NAME, addr, TYPE, a[pos].VALUE));
			break; 
		}
		case symboltable::TYPE: //新类型
		{

			length = NULL;
			int returnpos = Generate_type_table(p, q, a, pos, length);
			lenl.push_back(length);
			p.TABLE.push_back(symboltuple(a[pos].NAME, returnpos, TYPE, &lenl[lenl.size() - 1]));
			break;
		}
		case symboltable::DOM:
			break;
		case symboltable::VAL: //变量
		{
			int addr = GetTypeAddr(p, q, a, pos);
			p.TABLE.push_back(symboltuple(a[pos].NAME, addr, TYPE, a[pos].VALUE));
			break;
		}
		case symboltable::YF:
			break;
		case symboltable::VN:
			break;
		default:
			break;
		}
		pos++;
	}
	return true;

}

int symboltable::Generate_type_table(_SYNBL& p, _TYPEL& q, std::vector<ExToken> a, int& pos, int& length) {
	symboltable::inital(q);
	if (pos >= a.size()) {
		return false;
	}

	switch (a[pos].TVAL)
	{
	case symboltable::VOID:
		length += 1;
		break;
	case symboltable::BOOL:
		length += 1;
		break;
	case symboltable::CHAR:
		length += 1;
		break;
	case symboltable::SCHAR:
		length += 1;
		break;
	case symboltable::USCHAR:
		length += 1;
		break;
	case symboltable::SHORT:
		length += 2;
		break;
	case symboltable::UNSIGNED:
		length += 4;
		break;
	case symboltable::INT:
		length += 4;
		break;
	case symboltable::SIGNEDINT:
		length += 4;
		break;
	case symboltable::FLOAT:
		length += 4;
		break;
	case symboltable::LONG:
		length += 8;
		break;
	case symboltable::UNSIGNEDLONG:
		length += 8;
		break;
	case symboltable::DOUBLE:
		length += 8;
		break;
	case symboltable::LONGDOUBLE:
		length += 16;
		break;

	case symboltable::ARRAY:
		pos += 1;
		int length_2;
		ainfl.push_back(ainfltuple(0, a[pos].UP, Generate_type_table(p, q, a, pos, length_2), a[pos].CLEN));
		length += length_2 * (a[pos].UP - 1);
		q.put(typeltuple(ARRAY, ainfl.size() - 1));
		return q.TABLE.size() - 1;
		break;
	case symboltable::STRUCT:
	{
		pos++;
		_SYNBL p2;
		p2.prev = &p;

		strul.push_back(p2);
		q.put(typeltuple(STRUCT, strul.size() - 1));
		int ptr = q.TABLE.size() - 1;
		int length_2;
		_TYPEL q2 = _TYPEL();
		q2.prev = &q;

		Generate_symbol_table(p2, q2, a, pos, length_2);
		length += length_2;
		return ptr;
		break;
	}
	case symboltable::SUBBLOCK:
	{
		pos++;

		_SYNBL p2;
		p2.prev = &p;
		synbl.push_back(p2);
		q.put(typeltuple(SUBBLOCK, synbl.size() - 1));
		int ptr = q.TABLE.size() - 1;
		int length_2;
		_TYPEL q2;
		q2.prev = &q;

		Generate_symbol_table(p2, q2, a, pos, length_2);
		length += length_2;
		return ptr;
		break;
	}
	default:
		return -1;
		break;
	}

	return (int)a[pos].TVAL;
}

int symboltable::GetTypeAddr(_SYNBL& p, _TYPEL& q, std::vector<ExToken> a, int& pos)
{
	if (a[pos].TVAL <= LONGDOUBLE) {
		return int(a[pos].TVAL);
	}
	else {
		for (int i = 0; i < p.TABLE.size(); i++) {
			if (a[pos].NAME == p.TABLE[i].NAME) {
				return p.TABLE[i].TYP;
			}
		}
	}
	throw Error::stateError::type_errot;
}

int symboltable::GetTypeLength(_SYNBL& p, _TYPEL& q, std::vector<ExToken> a, int& pos)
{
	if (a[pos].TVAL >= VOID && a[pos].TVAL <= USCHAR) {
		return 1;
	}
	else if (a[pos].TVAL == SHORT) {
		return 2;
	}
	else if (a[pos].TVAL >= UNSIGNED && a[pos].TVAL <= FLOAT) {
		return 4;
	}
	else if (a[pos].TVAL>=LONG && a[pos].TVAL<=DOUBLE)
	{
		return 8;
	}
	else if (a[pos].TVAL==LONGDOUBLE)
	{
		return 16;
	}
	else
	{
		for (int i = 0; i < p.TABLE.size(); i++) {
			if (a[pos].NAME == p.TABLE[i].NAME) {
				return *(int*)(p.TABLE[i].ADDR);
			}
		}
	}
	return 0;
}

symboltable::symboltuple::symboltuple(std::string name, int typ, catset cat, void* addr)
{
	NAME = name; TYP = typ; CAT = cat; ADDR = addr;
}

symboltable::symboltuple::symboltuple()
{
}

