//���ļ����뵽string��
string readFileIntoString(char* filename)
{
	ifstream ifile(filename);
	//���ļ����뵽ostringstream����buf��
	ostringstream buf;
	char ch;
	while (buf && ifile.get(ch))
		buf.put(ch);
	//������������buf�������ַ���
	return buf.str();
}
int main() {
	string scode;
	char* fn = (char*)"test.cpp";
	scode = readFileIntoString(fn);
	lex::lex(scode);
	while (lex::Scanner()) {
		cout << "( " << lex::Token << ", " << lex::syn << ")" << endl;
	}
}