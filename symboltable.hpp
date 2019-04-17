#include "Config.hpp"
#include "Error.hpp"
#include "lex.hpp"
#include <vector>
#include <string>
namespace symboltable {
	enum symboltype {FUNC,CONS,TYPE,DOM,VAL,YF,VN};
	enum type
	{
		INTEGER, REAL, CHAR, BOOL, ARRAY, STRUCT
	};
	struct symboltuple  {
		std::string NAME;
		void* TYP;
		symboltype CAT;
		void* ADDR;
		symboltuple(std::string name, void* typ, symboltype cat, void* addr) {
			NAME = name; TYP = typ; CAT = cat; ADDR = addr;
		}
		symboltuple() {};

	};
	struct ExToken {
		std::string NAME;
		symboltype CAT;
		type TVAL;
		std::string CONS;
		int UP;
		void* CTP;
		int CLEN;
	};
	struct typetable {
		type TVAL;
		int TPOINT;
		typetable(type tval, int ptr) {
			TVAL = tval; TPOINT = ptr;
		}
	};
	struct ainfl {
		int LOW;
		int UP;
		void* CTP;
		int CLEN;
		ainfl(int low, int up, void* ctp, int clen) {
			LOW = low; UP = up; CTP = ctp; CLEN = clen;
		}
	};

	struct SYNBL {
		SYNBL *prev;
		std::vector<symboltuple> TABLE;
		bool put(ExToken t);
		SYNBL() {
			prev = NULL;
			
		}
		SYNBL(SYNBL* p) {
			prev = p;
		};
		symboltuple get(std::string s);
	};
	
	struct TYPEL {
		std::vector<typetable> TABLE;

		bool put(typetable t);

		void clear() {
			TABLE.clear();
		}
	};
	TYPEL typel;
	void inital() {
		typel.put(typetable(INTEGER, NULL));
		typel.put(typetable(REAL, NULL));
		typel.put(typetable(CHAR, NULL));
		typel.put(typetable(BOOL, NULL));
	}

	typedef std::vector<std::string> CONSL;
	CONSL consl;
	typedef std::vector<int> LENL;
	typedef std::vector<ainfl> AINFL;
	struct final
	{
		int FN;
		void* PAR;
		void* ENT;
	};
	typedef std::vector<final> FINAL;
	FINAL func;
	AINFL Ainfl;
	LENL lenl;
}