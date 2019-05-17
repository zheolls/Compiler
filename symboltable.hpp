#include <vector>
#include "lex.hpp"

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
		symboltuple();
	};
	struct ExToken {
		std::string NAME;
		catset CAT;
		typeset TVAL;
		std::string constomtype;
		void* ADDR;
		void* TYPE;
		void* VALUE;
		int CTP;
		std::vector<int> UP;
		
	};
	struct typeltuple {
		typeset TVAL;
		void* TPOINT;
		typeltuple(typeset tval, void* ptr);
		typeltuple(typeset t);
		typeltuple();
	};
	struct ainfltuple {
		void* TYPE;
		void* ADDR;
		int CLEN;
		std::vector<int> UP;
		ainfltuple();
	};
	typedef std::vector<ainfltuple> _AINFL;
	struct finaltuple
	{
		int FN;
		void* PAR;
		void* ENT;

	};
	typedef std::vector<typeltuple> _TYPEL;
	typedef std::vector<std::string> _CONSL;
	struct _SYNBL {
		_SYNBL* prev;
		_SYNBL* nextp;
		_TYPEL* tl;
		_CONSL* cl;
		_AINFL* al;
		int CLEN;
		std::vector<symboltuple> TABLE;
		_SYNBL();
	};
	typedef std::vector<int> _LENL;
	typedef std::vector<finaltuple> _FINAL;
	_SYNBL synbl;
	_SYNBL* pp;
	bool initialTYPEL();
	//创建符号表
	bool Scanner(lex::Token& token);
	bool SetType(_SYNBL& p, ExToken& a);
	int GetTypeLength(_SYNBL& p, void* addr);
	symboltable();
};
