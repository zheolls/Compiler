//#include <string>
#include <vector>
#include "Error.hpp"
class symboltable {
public:
	enum catset { FUNC, CONS, TYPE, DOM, VAL, YF, VN };
	enum typeset {
		VOID, BOOL, CHAR, SCHAR, USCHAR,    //1 byte
		SHORT,								//2 byte
		UNSIGNED, INT, SIGNEDINT, FLOAT,		//4 byte
		LONG, UNSIGNEDLONG, DOUBLE,			//8 byte
		LONGDOUBLE,							//16 byte
		ARRAY, STRUCT, RETURN, SUBBLOCK				//特殊类型
	};


	struct symboltuple {
		std::string NAME;
		int TYP;
		catset CAT;
		void* ADDR;
		symboltuple(std::string name, int typ, catset cat, void* addr);
		symboltuple();
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
		void* VALUE;
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
	struct _TYPEL {
		_TYPEL* prev;
		std::vector<typeltuple> TABLE;
		bool put(typeltuple t);
		void clear();
	};
	struct _SYNBL {
		_SYNBL* prev;
		_SYNBL* nextp;
		symboltable::_TYPEL* tl;
		std::vector<symboltuple> TABLE;
		bool put(ExToken t);
		_SYNBL();
		_SYNBL(_SYNBL* p);
		symboltuple get(std::string s);

	};
	const int a[1] = { 0 };
	typedef std::vector<std::string> _CONSL;
	typedef std::vector<int> _LENL;
	typedef std::vector<_SYNBL> _STRUL, _SYNL;
	typedef std::vector<ainfltuple> _AINFL;
	typedef std::vector<finaltuple> _FINAL;
	_CONSL consl;
	_FINAL final;
	_AINFL ainfl;
	_LENL lenl;
	_STRUL strul;
	_SYNL synbl;
	_TYPEL typel;
	bool is_existed(_SYNBL p, ExToken a);
	void inital(_TYPEL& q);
	//创建符号表
	//p指向当前表，a为Token序列，pos为序列扫描指针
	bool Generate_symbol_table(_SYNBL& p, _TYPEL& q, std::vector<ExToken> a, int& pos, int& length);
	int Generate_type_table(_SYNBL& p, _TYPEL& q, std::vector<ExToken> a, int& pos, int& length);
	int GetTypeAddr(_SYNBL& p, _TYPEL& q, std::vector<ExToken> a, int& pos);
	int GetTypeLength(_SYNBL& p, _TYPEL& q, std::vector<ExToken> a, int& pos);
	bool Scanner() {


	}
};
