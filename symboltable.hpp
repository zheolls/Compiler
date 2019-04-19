#include "Config.hpp"
#include "Error.hpp"
#include "lex.hpp"
#include <vector>
#include <string>
namespace symboltable {
	enum catset {FUNC,CONS,TYPE,DOM,VAL,YF,VN};
	enum typeset{
		VOID, BOOL, CHAR, SCHAR, USCHAR,    //1 byte
		SHORT,								//2 byte
		UNSIGNED,INT,SIGNEDINT, FLOAT,		//4 byte
		LONG,UNSIGNEDLONG,DOUBLE,			//8 byte
		LONGDOUBLE,							//16 byte
		ARRAY, STRUCT,RETURN				//特殊类型
	};
	struct symboltuple  {
		std::string NAME;
		int TYP;
		catset CAT;
		int ADDR;
		symboltuple(std::string name, int typ, catset cat, int addr) {
			NAME = name; TYP = typ; CAT = cat; ADDR = addr;
		}
		symboltuple() {};

	};
	struct ExToken {
		std::string NAME;
		catset CAT;
		typeset TVAL;
		std::string TYPEN;
		std::string CONS;
		int UP;
		int CTP;
		int CLEN;
	};
	struct typeltuple {
		typeset TVAL;
		int TPOINT;
		typeltuple(typeset tval, int ptr) {
			TVAL = tval; TPOINT = ptr;
		}

	};
	struct ainfltuple {
		int LOW;
		int UP;
		int CTP;
		int CLEN;
		ainfltuple(int low, int up, int ctp, int clen) {
			LOW = low; UP = up; CTP = ctp; CLEN = clen;
		}
	};
	struct finaltuple
	{
		int FN;
		void* PAR;
		void* ENT;

	};
	struct _SYNBL {
		_SYNBL *prev;
		std::vector<symboltuple> TABLE;
		bool put(ExToken t);
		_SYNBL() {
			prev = NULL;
			
		}
		_SYNBL(_SYNBL* p) {
			prev = p;
		};
		symboltuple get(std::string s);

	};
	struct _TYPEL {
		std::vector<typeltuple> TABLE;
		bool put(typeltuple t);
		void clear() {
			TABLE.clear();
		}
	};
	typedef std::vector<std::string> _CONSL;
	typedef std::vector<int> _LENL;
	typedef std::vector<_SYNBL> _STRUL;
	typedef std::vector<ainfltuple> _AINFL;
	typedef std::vector<finaltuple> _FINAL;
	_CONSL consl;
	_FINAL final;
	_AINFL ainfl;
	_LENL lenl;
	_STRUL strul;
	_TYPEL typel;

	//创建符号表
	//p指向当前表，a为Token序列，pos为序列扫描指针
	bool Generate_symbol_table (_SYNBL *p,std::vector<ExToken> a, int& pos,int &length) {
		if (pos >= a.size()) {
			return false;
		}
		while (a[pos].TVAL != RETURN) {
			switch (a[pos].CAT)
			{
			case symboltable::FUNC:
				break;
			case symboltable::CONS:
				break;
			case symboltable::TYPE:
				length = NULL;
				int returnpos = Generate_type_table(p, a, pos, length);
				(*p).TABLE.push_back(symboltuple(a[pos].NAME, returnpos, TYPE, length));
				break;
			case symboltable::DOM:
				break;
			case symboltable::VAL:
				break;
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
	int Generate_type_table (_SYNBL* p,std::vector<ExToken> a, int& pos,int &length) {
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
			ainfl.push_back(ainfltuple(0, a[pos].UP, Generate_type_table (p, a, pos, length_2), a[pos].CLEN));
			length += length_2 * (a[pos].UP-1);
			typel.put(typeltuple(ARRAY, ainfl.size() - 1));
			return typel.TABLE.size() - 1;
			break;
		case symboltable::STRUCT:
			pos++;
			strul.push_back(_SYNBL());
			typel.put(typeltuple(STRUCT, strul.size() - 1));
			int ptr = typel.TABLE.size() - 1;
			int length_2;
			strul[strul.size() - 1].prev = p;
			Generate_symbol_table (&strul[strul.size()-1], a, pos, length_2);
			length += length_2;
			return ptr;
			break;

		default:
			return _GET_WRONG_TYPE;
			break;
		}

		return (int)a[pos].TVAL;
	}

	void inital() {
		typel.put(typeltuple(VOID, NULL));
		typel.put(typeltuple(BOOL, NULL));
		typel.put(typeltuple(CHAR, NULL));
		typel.put(typeltuple(SCHAR, NULL));
		typel.put(typeltuple(USCHAR, NULL));
		typel.put(typeltuple(SHORT, NULL));
		typel.put(typeltuple(UNSIGNED, NULL));
		typel.put(typeltuple(INT, NULL));
		typel.put(typeltuple(SIGNEDINT, NULL));
		typel.put(typeltuple(FLOAT, NULL));
		typel.put(typeltuple(LONG, NULL));
		typel.put(typeltuple(UNSIGNEDLONG, NULL));
		typel.put(typeltuple(DOUBLE, NULL));
		typel.put(typeltuple(LONGDOUBLE, NULL));
		typel.put(typeltuple(ARRAY, NULL));
		typel.put(typeltuple(STRUCT, NULL));
		typel.put(typeltuple(RETURN, NULL));
	}
}