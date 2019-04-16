//从文件读入到string里
string readFileIntoString(char* filename)
{
	ifstream ifile(filename);
	//将文件读入到ostringstream对象buf中
	ostringstream buf;
	char ch;
	while (buf && ifile.get(ch))
		buf.put(ch);
	//返回与流对象buf关联的字符串
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