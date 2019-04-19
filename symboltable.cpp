#include "symboltable.hpp"

using namespace symboltable;

bool symboltable::_SYNBL::put(ExToken t)
{
	switch (t.CAT){
		case FUNC: 
			final.push_back(finaltuple());
			_SYNBL::TABLE.push_back(symboltuple()); 
			break;
		case CONS:
			consl.push_back(t.CONS);
			break;
		case TYPE:





			switch (t.TVAL)
			{
			case ARRAY:
				ainfl.push_back(ainfltuple(0,t.UP,t.CTP,t.CLEN));
				typel.put(typeltuple(t.TVAL, ainfl.size()-1));
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

symboltuple symboltable::_SYNBL::get(std::string s)
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
