#include <vector>
#include "lex.hpp"

class symboltable {
private:
	int tvpos;

	bool initialTYPEL();
	union VALUE
	{
		int intval;
		float fval;
		double dval;
		bool bval;
	};
	struct TVP {
		VALUE val;
		int no;
	};

public:
	enum catset { FUNC, CONS, TYPE, RECORD, VAL, YF, VN,TV };
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
		typeset TVAL,ARRTVAL;
		std::string constomtype;
		void* ADDR;
		void* TYPE;
		void* VALUE;
		int CTP;
		int addr;
		int width;
		std::vector<int> UP;
		ExToken();
	};
	struct finaltuple
	{
		int FN;
		void* PAR;
		void* ENT;

	};
	struct ainfltuple {
		void* TYPE;
		void* ADDR;
		int CLEN;
		std::vector<int> UP;
		ainfltuple();
	};
	struct typeltuple {
		typeset TVAL;
		void* TPOINT;
		typeltuple(typeset tval, void* ptr);
		typeltuple(typeset t);
		typeltuple();
	};
	typedef std::vector<ainfltuple> _AINFL;
	typedef std::vector<typeltuple> _TYPEL;
	typedef std::vector<int> _CONSL;
	typedef std::vector<int> _LENL;
	typedef std::vector<finaltuple> _FINAL;

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

	_SYNBL synbl;
	_SYNBL* pp;

	std::string printsymbol(int deepth, _SYNBL* p);


	//创建符号表
	bool Scanner(lex::Token& token);
	bool SetType(ExToken& a);
	int GetTypeLength(_SYNBL& p, void* addr);
	symboltable();
	int Temp(int type);
	std::string get(int addr);
	std::string printsymbol();
	int  get(std::string val);
};
