base type:
int
double
void
bool
long 
short

type:
base type
customtype

constant:
const variable state

customtype:
struct indentifer {
	variable state | func | constant
}

start:
variable state
func
constant


variable state:
type indentifer
type * indentifer
type & indentifer


func:
variable state (blank | variable state)


while ( variable state | constant){
	constant
	variable state
}

for (variable state | constant) {
	constant
	variable state
}
if (variable state | constant) {
	constant
	variable state
}

int main() {
	std::string scode;
	std::vector<lex::Token> tokenlist;
	char* fn = (char*)"main.cpp";
	lex lex(scode);
	lex.per_process();
	struct symboltuple {
		std::string NAME;
		int TYP;
		catset CAT;
		void* ADDR;
	};
	while (lex.Scanner()) {
		int i=1;
	}
	for (int i=1;;){
		int j=1;
	}
}