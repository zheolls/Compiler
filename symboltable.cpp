#include "symboltable.hpp"

using namespace symboltable;

bool symboltable::SYNBL::put(ExToken t)
{
	switch (t.CAT){
		case FUNC: 
			func.push_back(final());
			SYNBL::TABLE.push_back(symboltuple()); 
			break;
		case CONS:
			consl.push_back(t.CONS);
			break;
		case TYPE:
			switch (t.TVAL)
			{
			case ARRAY:
				Ainfl.push_back(ainfl(0,t.UP,t.CTP,t.CLEN));
				typel.put(typetable(t.TVAL, Ainfl.size()-1));
				break;
			case STRUCT:
				break;
			default:
				break;
			}
		case VAL:

	}
	return 1;
}

symboltuple symboltable::SYNBL::get(std::string s)
{
	bool flag = 0;
	symboltuple found;
	for (SYNBL* e = this; e != NULL; e = prev) 
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



bool symboltable::TYPEL::put(lex::Token token)
{

	return false;
}

bool symboltable::TYPEL::put(typetable t)
{
	TABLE.push_back(t);
	return false;
}
