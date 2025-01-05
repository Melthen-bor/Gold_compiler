#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <cstdarg>
#include <cstdlib>
#define create_vector(type) typedef std::vector<type> type ## s
unsigned char pass() {
	return 0;
}
struct Token {
	unsigned long long type;
	std::string val = "";
};
create_vector(Token);
struct Type {
	std::string name;
	char start_character;
};
create_vector(Type);
unsigned long long find_type(Types& types, std::string name) {
	unsigned long long handle = 0;
	while (handle < types.size()) {
		if (types.at(handle).name == name) break;
		handle++;
	}
	return handle;
}
bool does_type_exist(Types& types, std::string name){
	return !(find_type(types, name) == types.size());
}
unsigned find_type_with_char(Types& types, char start_char) {
	unsigned handle = 0;
	while (handle < types.size()) {
		if (types.at(handle).start_character == start_char) break;
		handle++;
	}
	return handle;
}
bool does_type_exist_with_char(Types& types, char start_char) {
	return !(find_type_with_char(types, start_char) == types.size());
}
template<class Type> unsigned long long match(unsigned long long amount,Type value, ...) {
	unsigned long long check = 0;
	std::va_list args;
	va_start(args, value);
	while (check < amount) {
		if (value == va_arg(args, Type)) break;
		check++;
	}
	va_end(args);
	return check;
}
template<class Type, class Basic> unsigned long long _loose_match(unsigned long long amount, Type value, ...) {
	unsigned long long check = 0;
	std::va_list args;
	va_start(args, value);
	while (check < amount) {
		if (value == Type(va_arg(args, Basic))) break;
		check++;
	}
	va_end(args);
	return check;
}
#define loose_match(amount,value,test,...) _loose_match<decltype(value),test>(amount,value,__VA_ARGS__)
struct Char {
	bool exist;
	char val;
	void print() {
		if (exist) std::cout << "true,";
		else std::cout << "false,";
		std::cout << (int)val << '\n';
	}
};
class lexer {
	unsigned long long index;
	std::string src;
	Char peek(unsigned long long ahead = 0) {
		if ((index + ahead) < src.size()) return Char{ true, this->src.at(index + ahead) };
		return Char{ false,'\0' };
	}
	char consume() {
		return this->src.at(this->index++);
	}
public:
	lexer(std::string source) {
		src = source;
		index = 0;
	}
	void reset(std::string source) {
		src = source;
		index = 0;
	}
	Tokens tokenize() {
		Tokens tkns;
		std::string buf;
		Char prev = { 0,0 };
		while (peek().exist) {
			if (isalpha(peek().val)) {
				buf.push_back(consume());
				while ((peek().exist && isalnum(peek().val))||peek().val=='_') buf.push_back(consume());
				switch (loose_match(30, buf, char*,"return", "start", "end", "program", "class","auto", "let", "if", "elf", "else", "unless",
					"while", "until", "function", "clr", "member", "sysout","procedure","sysin", "create", "destroy", "letter","embed","import","as",
					"open_class","struct","null","forever","pass")) {
				case 0:
					tkns.push_back({ 1 });
					break;
				case 1:
					tkns.push_back({ 38 });
					break;
				case 2:
					tkns.push_back({ 39 });
					break;
				case 3:
					tkns.push_back({ 40 });
					break;
				case 4:
					tkns.push_back({ 41 });
					break;
				case 5:
					tkns.push_back({ 47 });
					break;
				case 6:
					tkns.push_back({ 48 });
					break;
				case 7:
					tkns.push_back({ 49 });
					break;
				case 8:
					tkns.push_back({ 50 });
					break;
				case 9:
					tkns.push_back({ 51 });
					break;
				case 10:
					tkns.push_back({ 52 });
					break;
				case 11:
					tkns.push_back({ 53 });
					break;
				case 12:
					tkns.push_back({ 54 });
					break;
				case 13:
					tkns.push_back({ 55 });
					break;
				case 14:
					tkns.push_back({ 56 });
					break;
				case 15:
					tkns.push_back({ 58 });
					break;
				case 16:
					tkns.push_back({ 59 });
					break;
				case 17:
					tkns.push_back({ 60 });
					break;
				case 18:
					tkns.push_back({ 61 });
					break;
				case 19:
					tkns.push_back({ 64 });
					break;
				case 20:
					tkns.push_back({ 65 });
					break;
				case 21:
					tkns.push_back({ 67 });
					break;
				case 22:
					tkns.push_back({ 68 });
					break;
				case 23:
					tkns.push_back({ 69 });
					break;
				case 24:
					tkns.push_back({ 70 });
					break;
				case 25:
					tkns.push_back({ 71 });
					break;
				case 26:
					tkns.push_back({ 72 });
					break;
				case 27:
					tkns.push_back({ 73 });
					break;
				case 28:
					tkns.push_back({ 77 });
					break;
				case 29:
					tkns.push_back({ 78 });
					break;
				default:
					tkns.push_back({ 0,buf });
				}
				buf.clear();
			}
			else if (isdigit(peek().val)) {
				buf.push_back(consume());
				while (peek().exist && isdigit(peek().val)) buf.push_back(consume());
				tkns.push_back({ 2,buf });
				buf.clear();
			}
			else if (peek().val == '"') {
				consume();
				while (peek().exist && !(peek().val == '"')) buf.push_back(consume());
				consume();
				tkns.push_back({ 3,buf });
				buf.clear();
			}
			else if (peek().val == ';') {
				consume();
				tkns.push_back({ 4 });
			}
			else if (peek().val == '+') {
				consume();
				switch (peek().val) {
				case 43:
					consume();
					tkns.push_back({ 16 });
					break;
				case 61:
					consume();
					tkns.push_back({ 17 });
					break;
				default:
					tkns.push_back({ 5 });
				}
			}
			else if (peek().val == '-') {
				consume();
				switch (peek().val) {
				case 62:
					consume();
					tkns.push_back({ 12 });
					break;
				case 61:
					consume();
					tkns.push_back({ 13 });
					break;
				case 45:
					consume();
					tkns.push_back({ 15 });
					break;
				default:
					tkns.push_back({ 6 });
				}
			}
			else if (peek().val == '/') {
				consume();
				tkns.push_back({ 7 });
			}
			else if (peek().val == '*') {
				consume();
				switch (peek().val) {
				case 61:
					consume();
					tkns.push_back({ 18 });
					break;
				case 42:
					consume();
					switch (peek().val) {
					case 61:
						consume();
						tkns.push_back({ 21 });
					default:
						tkns.push_back({ 19 });
					}
					break;
				default:
					tkns.push_back({ 8 });
				}
			}
			else if (peek().val == '&') {
				consume();
				switch (peek().val) {
				case 42:
					consume();
					tkns.push_back({ 9 });
					break;
				case 33:
					consume();
					tkns.push_back({ 10 });
					break;
				case 62:
					consume();
					tkns.push_back({ 11 });
					break;
				case 38:
					consume();
					tkns.push_back({ 20 });
					break;
				case 61:
					consume();
					tkns.push_back({ 22 });
					break;
				default:
					tkns.push_back({ 14 });
				}
			}
			else if (peek().val == '<') {
				consume();
				switch (peek().val) {
				case 60:
					consume();
					switch (peek().val) {
					case 61:
						consume();
						tkns.push_back({ 25 });
						break;
					default:
						tkns.push_back({ 24 });
					}
					break;
				default:
					tkns.push_back({ 23 });
				}
			}
			else if (peek().val == '>') {
				consume();
				switch (peek().val) {
				case 62:
					consume();
					switch (peek().val) {
					case 61:
						consume();
						tkns.push_back({ 28 });
						break;
					default:
						tkns.push_back({ 27 });
					}
					break;
				default:
					tkns.push_back({ 26 });
				}
			}
			else if (peek().val == '|') {
				consume();
				switch (peek().val) {
				case 61:
					consume();
					tkns.push_back({ 31 });
					break;
				case 124:
					consume();
					tkns.push_back({ 30 });
					break;
				default:
					tkns.push_back({ 29 });
				}
			}
			else if (peek().val == '^') {
				consume();
				switch (peek().val) {
				case 61:
					consume();
					tkns.push_back({ 34 });
					break;
				case 94:
					consume();
					tkns.push_back({ 33 });
					break;
				default:
					tkns.push_back({ 32 });
				}
			}
			else if (peek().val == '=') {
				consume();
				switch (peek().val) {
				case '=':
					consume();
					tkns.push_back({ 36 });
					break;
				default:
					tkns.push_back({ 35 });
				}
			}
			else if (peek().val == '[') {
				consume();
				while (peek().val != ']') {
					consume();
				}
				consume();
			}
			else if (peek().val == '@') {
				consume();
				while (peek().val != ' ' && peek().val != '\n') buf.push_back(consume());
				tkns.push_back({ 37,buf });
				buf.clear();
			}
			else if (peek().val == '{') {
				consume();
				tkns.push_back({ 42 });
			}
			else if (peek().val == '}') {
				consume();
				tkns.push_back({ 43 });
			}
			else if (peek().val == '(') {
				consume();
				tkns.push_back({ 44 });
			}
			else if (peek().val == ')') {
				consume();
				tkns.push_back({ 45 });
			}
			else if (peek().val == '$') {
				consume();
				tkns.push_back({ 47 });
			}
			else if (peek().val == ',') {
				consume();
				tkns.push_back({ 57 });
			}
			else if (peek().val == '%') {
				consume();
				switch (peek().val) {
				case 37:
					consume();
					tkns.push_back({ 75 });
					break;
				case 38:
					consume();
					tkns.push_back({ 76 });
					break;
				case 61:
					consume();
					tkns.push_back({ 66 });
					break;
				case 62:
					consume();
					tkns.push_back({ 62 });
					break;
				case 94:
					consume();
					tkns.push_back({ 74 });
					break;
				default:
					tkns.push_back({ 63 });
				}
			}
			else if (peek().val == '!') {
				consume();
				while (peek().exist && peek().val != '\n') consume();
			}
			else {
				consume();
			}
		}
		return tkns;
	}
};
struct flags {
	unsigned char val;
	flags() {
		this->val = 0;
	}
	bool get(unsigned char loc) {
		return this->val & (1 << loc);
	}
	void store(unsigned char loc, bool val) {
		if (val) this->val |= (1 << loc);
		else this->val &= 255 - (1 << loc);
	}
};
std::string join(std::vector<std::string>& list, char connector) {
	std::string out = list.at(0);
	unsigned long long count = 1;
	while (count < list.size()) {
		out.push_back(connector);
		out += list.at(count);
		count++;
	}
	return out;
}
void split(std::vector<std::string>& out, std::string String, char delim) {
	std::string tempString;
	char tempChar;
	unsigned long long count = 0;
	while (count < String.size()) {
		tempChar = String.at(count);
		if (tempChar == delim) {
			out.push_back(tempString);
			tempString.clear();
		}
		else tempString.push_back(tempChar);
		count++;
	}
	out.push_back(tempString);
}
template<class A,class B> bool contains(std::vector<A>& list, B item) {
	unsigned long long count = 0;
	while (count < list.size()) if (list.at(count++) == item) return true;
	return false;
}
template<class A,class B> unsigned long long indexOf(std::vector<A>& list, B item) {
	unsigned long long count = 0;
	while (count < list.size()) if (list.at(count++) == item) return count-1;
	return list.size();
}
template<class T> T remove(std::vector<T>& list, unsigned long long index) {
	std::vector<T> out;
	T str;
	unsigned long long count = 0;
	while (count < list.size()) if (!(count == index)) out.push_back(list.at(count++));
	str = list.at(index);
	list = out;
	return str;
}
std::string load_file(std::string name) {
	std::ifstream file(name);
	if (!file.is_open()) {
		std::cerr << "\033[31mFile " << name << " failed to open[Crashing program]\n\033[0m";
		std::exit(1);
	}
	std::vector<std::string> out;
	std::string temp;
	while (std::getline(file, temp)) out.push_back(temp);
	file.close();
	return join(out, '\n');
}
void load_flags(std::vector<std::string>& flags, char* argv[], int argc) {
	int count = 0;
	if (argc < 2) {
		std::cout << "Not enough arguments are supplies!\n";
		std::exit(1);
	}
	while (count < argc) flags.push_back(argv[count++]);
}
struct Variable {
	unsigned long long type;
	unsigned char ptr_type;
	std::string name;
};
typedef std::vector<Variable> Variables;
unsigned find_variable(Variables& variables, std::string name) {
	unsigned count = 0;
	while (count < variables.size()) {
		if (variables.at(count).name == name) break;
		count++;
	}
	return count;
}
bool does_variable_exist(Variables& variables, std::string name) {
	return !(find_variable(variables, name) == variables.size());
}
struct Argument {
	unsigned long long value_type;
	unsigned char type_of_argument;
	std::string name;
};
typedef std::vector<Argument> Arguments;
unsigned find_argument(Arguments& args, std::string name) {
	unsigned count = 0;
	while (count < args.size()) {
		if (args.at(count).name == name) break;
		count++;
	}
	return count;
}
bool does_argument_exist(Arguments& args, std::string name) {
	return !(find_argument(args, name) == args.size());
}
struct Args {
	Arguments args;
	unsigned char flags;
};
struct Function {
	std::string name;
	Arguments args;
	unsigned long long Rtype;
	unsigned char ptr_type;
	bool no_discard;
	void print(Types& types) {
		std::cout << "function " << types.at(Rtype).name << ' ' << name << '{';
		unsigned index = 0;
		while (index < args.size()) {
			if (index) std::cout << ',';
			std::cout << types.at(args.at(index).value_type).name;
			if (args.at(index).type_of_argument) std::cout << '*';
			std::cout << ' ' << args.at(index++).name;
		}
		std::cout << "}\n";
	}
};
typedef std::vector<Function> Functions;
unsigned find_function(Functions& functions, std::string name) {
	unsigned count = 0;
	while (count < functions.size()) {
		if (functions.at(count).name == name) break;
		count++;
	}
	return count;
}
bool does_function_exist(Functions& functions, std::string name) {
	return !(find_function(functions, name) == functions.size());
}
struct Procedure {
	std::string name;
	Arguments args;
	void print(Types& types) {
		std::cout << "procedure " << name << '{';
		unsigned index = 0;
		while (index < args.size()) {
			if (index) std::cout << ',';
			std::cout << types.at(args.at(index).value_type).name;
			if (args.at(index).type_of_argument) std::cout << '*';
			std::cout << ' ' << args.at(index++).name;
		}
		std::cout << "}\n";
	}
};
typedef std::vector<Procedure> Procedures;
unsigned find_procedure(Procedures& procedures, std::string name) {
	unsigned count = 0;
	while (count < procedures.size()) {
		if (procedures.at(count).name == name) break;
		count++;
	}
	return count;
}
bool does_procedure_exist(Procedures& procedures, std::string name) {
	return !(find_procedure(procedures, name) == procedures.size());
}
struct Class {
	std::string name;
	Functions methods;
	Variables members;
	Procedures procedures;
	bool has_constructor;
	bool has_deconstructor;
	bool is_open;
	bool doth_arg_exist(std::string name) {
		return does_argument_exist(methods.at(methods.size() - 1).args, name);
	}
	bool doth_proc_arg_exist(std::string name) {
		return does_argument_exist(procedures.at(procedures.size() - 1).args, name);
	}
	bool doth_method_exist(std::string name) {
		return does_function_exist(methods, name);
	}
	bool doth_member_exist(std::string name) {
		return does_variable_exist(members, name);
	}
	bool doth_proc_exist(std::string name) {
		return does_procedure_exist(procedures, name);
	}
	Argument& get_arg(std::string name) {
		return methods.at(methods.size() - 1).args.at(find_argument(methods.at(methods.size() - 1).args, name));
	}
	Argument& get_proc_arg(std::string name) {
		return procedures.at(procedures.size() - 1).args.at(find_argument(procedures.at(procedures.size() - 1).args, name));
	}
	Function& get_method(std::string name) {
		return methods.at(find_function(methods, name));
	}
	Variable& get_member(std::string name) {
		return members.at(find_variable(members, name));
	}
	Procedure& get_proc(std::string name) {
		return procedures.at(find_procedure(procedures, name));
	}
};
typedef std::vector<Class> Classes;
unsigned find_class(Classes& classes, std::string name) {
	unsigned count = 0;
	while (count < classes.size()) {
		if (classes.at(count).name == name) break;
		count++;
	}
	return count;
}
bool does_class_exist(Classes& classes, std::string name) {
	return !(find_class(classes, name) == classes.size());
}
struct Constant {
	std::string name;
	std::string value;
};
typedef std::vector<Constant> Constants;
unsigned find_constant(Constants& constants, std::string name) {
	unsigned count = 0;
	while (count < constants.size()) {
		if (constants.at(count).name == name) break;
		count++;
	}
	return count;
}
bool does_constant_exist(Constants& constants, std::string name) {
	return !(find_constant(constants, name) == constants.size());
}
unsigned char is_what(Types& types, Functions& functions, Variables& variables,Constants& constants,std::string name) {
	if (does_type_exist(types, name)) return 1;
	if (does_function_exist(functions, name)) return 2;
	if (does_variable_exist(variables, name)) return 3;
	if (does_constant_exist(constants, name)) return 4;
	return 0;
}
struct node {
	unsigned char type;
	Token tkn;
	node* nd_one;
	node* nd_two;
	node(Token tkn) {
		this->type = 1;
		this->tkn = tkn;
		this->nd_one = nullptr;
		this->nd_two = nullptr;
	}
	node(node* one, node* two) {
		this->type = 2;
		this->nd_one = one;
		this->nd_two = two;
	}
	unsigned long long level() {
		if (this->type < 2) return this->type;
		if (this->nd_one->level() > this->nd_two->level()) return this->nd_one->level() + 1;
		return this->nd_two->level() + 1;
	}
};
class parse_tree {
	std::vector<node*> nodes;
	unsigned long long current_level;
public:
	parse_tree(Tokens tkns) {
		unsigned long long index = 0;
		while (index < tkns.size()) {
			nodes.push_back(new node(tkns.at(index)));
			index++;
		}
		this->current_level = 1;
	}
	void collapse() {
		unsigned long long index = 0;
		while (index < this->nodes.size()) {
			if (this->nodes[index]->level() == this->current_level) {

			}
			index++;
		}
	}
	node* get_tree() {
		return this->nodes[0];
	}
};
struct scope {
	Variables variables;
	Constants constants;
	bool is_class=false;
	bool is_function = false;
};
struct Struct {
	std::string name;
	Variables members;
};
typedef std::vector<Struct> Structs;
unsigned find_struct(Structs& structs, std::string name) {
	unsigned count = 0;
	while (count < structs.size()) {
		if (structs.at(count).name == name) break;
		count++;
	}
	return count++;
}
bool does_struct_exist(Structs& structs, std::string name) {
	return !(find_struct(structs, name) == structs.size());
}
struct Library {
	std::string name;
	Classes classes;
	Structs structs;
	Functions functions;
	Procedures procedures;
	bool includeth_function(std::string name) {
		return does_function_exist(functions, name);
	}
	bool includeth_procedure(std::string name) {
		return does_procedure_exist(procedures, name);
	}
	bool includeth_class(std::string name) {
		return does_class_exist(classes, name);
	}
	bool includeth_struct(std::string name) {
		return does_struct_exist(structs, name);
	}
};
typedef std::vector<Library> Libraries;
typedef std::vector<scope> Scopes;
typedef unsigned char language_type;
typedef unsigned char system_type;
void print(Tokens& tkns) {
	unsigned long long index = 0;
	while (index < tkns.size()) std::cout << tkns.at(index).type << ':' << tkns.at(index++).val << '\n';
}
void print(Tokens& tkns,unsigned long long& index) {
	std::cout << index << ':' << tkns.at(index).type << ':' << tkns.at(index).val << '\n';
}
struct binding_token {
	unsigned char type;
	std::string value;
};
typedef std::vector<binding_token> BTokens;
void print(BTokens& tkns) {
	unsigned long long index = 0;
	while (index < tkns.size()) std::cout << (int)tkns.at(index).type << ':' << tkns.at(index++).value << '\n';
}
void print(BTokens& tkns, unsigned long long& index) {
	std::cout << index << ':' << (int)tkns.at(index).type << ':' << tkns.at(index).value << '\n';
}
class Binding_lexer {
	unsigned long long index;
	std::string src;
	Char peek(unsigned long long ahead = 0) {
		if ((index + ahead) < src.size()) return Char{ 1,src.at(index + ahead) };
		return { 0 };
	}
	char consume() {
		return src.at(index++);
	}
public:
	Binding_lexer(std::string name) {
		index = 0;
		src = load_file(name+"_bindings.data");
		std::cout << src << '\n';
	}
	BTokens tokenize() {
		BTokens out;
		std::string buf = "";
		while (peek().exist) {
			if (isalpha(peek().val)) {
				buf.push_back(consume());
				while (peek().exist&&(isalnum(peek().val) || peek().val == '_')) buf.push_back(consume());
				switch (loose_match(9, buf, char*, "class","end","lib","func","proc","member","open_class","struct","letter")) {
				case 0:
					out.push_back({ 6 });
					break;
				case 1:
					out.push_back({ 2 });
					break;
				case 2:
					out.push_back({ 3 });
					break;
				case 3:
					out.push_back({ 4 });
					break;
				case 4:
					out.push_back({ 5 });
					break;
				case 5:
					out.push_back({ 7 });
					break;
				case 6:
					out.push_back({ 9 });
					break;
				case 7:
					out.push_back({ 10 });
					break;
				case 8:
					out.push_back({ 11 });
					break;
				default:
					out.push_back({ 0,buf });
				}
				buf.clear();
			}
			else if (peek().val == '*') {
				consume();
				out.push_back({ 8 });
			}
			else if (peek().val == '\n') {
				consume();
				out.push_back({ 1 });
			}
			else consume();
		}
		return out;
	}
};
class compiler {
	Types types;
	Classes classes;
	Functions functions;
	Procedures procedures;
	Structs structs;
	Libraries libs;
	Scopes scopes;
	flags mode;
	std::string project;
	unsigned long long index;
public:
	compiler(std::string name) {
		project = name;
		types = Types();
		types.push_back(Type{ "byte",'b'});
		types.push_back(Type{ "char",'c'});
		types.push_back(Type{ "Sbyte",'B'});
		types.push_back(Type{ "Umini",'m' });
		types.push_back(Type{ "mini",'M'});
		types.push_back(Type{ "Ucint",'t'});
		types.push_back(Type{ "cint",'T'});
		types.push_back(Type{ "Ulong",'i'});
		types.push_back(Type{ "long",'I'});
		types.push_back(Type{ "Ullong",'l'});
		types.push_back(Type{ "llong",'L'});
		scopes.push_back({});
	}
	Args get_args(std::ofstream& file,Tokens& tkns) {
		Arguments args;
		unsigned char loop = 1;
		file << '(';
		while (loop) {
			if (index == tkns.size()) {
				std::cout << "Unpaired brace detected: reached end of token list with a closing brace!\n";
				std::exit(1);
			}
			print(tkns, index);
			switch (tkns.at(index).type) {
			case 0:
				switch (tkns.at(index + 1).type) {
				case 0:
					file << tkns.at(index).val << ' ' << tkns.at(index + 1).val;
					args.push_back({ find_type(types,tkns.at(index).val),0,tkns.at(index + 1).val });
					index++;
					break;
				case 9:
					if (tkns.at(index + 2).type) return { args,1 };
					file << tkns.at(index).val << "* " << tkns.at(index + 2).val;
					if (mode.get(0)) args.push_back({ find_type(types,tkns.at(index).val),2,tkns.at(index + 2).val });
					else args.push_back({ find_type(types,tkns.at(index).val),1,tkns.at(index + 2).val });
					mode.store(0, 0);
					index += 2;
					break;
				default:
					std::cout << "Args:thing doth not exist\n";
					return { args,1 };
				}
				break;
			case 37:
				if (tkns.at(index).val == "ARRAY_POINTER") mode.store(0, 1);
				break;
			case 42:
				file << '(';
				loop++;
				break;
			case 43:
				file << ')';
				loop--;
				break;
			case 47:
				switch (tkns.at(index + 1).type) {
				case 0:
					file << types.at(find_type_with_char(types, tkns.at(index + 1).val[0])).name << ' ' << tkns.at(index + 1).val;
					args.push_back({ find_type_with_char(types,tkns.at(index+1).val[0]),0,tkns.at(index + 1).val});
					index++;
					break;
				case 9:
					if (!tkns.at(index + 2).type) return { args,1 };
					file << types.at(find_type_with_char(types, tkns.at(index + 1).val[0])).name << "* " << tkns.at(index + 1).val;
					args.push_back({ find_type_with_char(types,tkns.at(index + 2).val[0]),1,tkns.at(index + 2).val });
					index += 2;
					break;
				default:
					return { args,1 };
				}
				break;
			case 57:
				file << ',';
				break;
			default:
				std::cout << "Args:token type not recognized\n";
				return { args,1 };
			}
			index++;
		}
		return { args,0 };
	}
	std::string indent(unsigned long long amount) {
		std::string out;
		while (amount > 0) {
			out.push_back(9);
			amount--;
		}
		return out;
	}
	void impl_sizeof_var(flags& values,std::ofstream& file) {
		if (values.get(0)) file << "Ullong " << project << "_sizeof_var(Ullong var){\n";
		else file << "Ullong sizeof_var(Ullong var) {\n";
		file << indent(1) << "switch(var){\n";
		unsigned long long index = 0;
		while (index < types.size()) file << indent(1) << "case " << index << ":\n" << indent(2) << "return sizeof(" << types.at(index++).name << ");\n";
		file << "}\n";
	}
	void setup_cpp(flags& values,std::ofstream& file) {
		file << "#include <iostream>\n";
		//file << "struct byte{\n";
		//file << "unsigned char data;\n";
		//file << "byte(unsigned char);\n";
		//file << "byte add(byte other);\n";
		//file << "void ass(byte other);\n"
		//file << "};\n";
		file << "typedef unsigned char byte;\n";
		file << "typedef signed char Sbyte;\ntypedef unsigned short Umini;\ntypedef short mini;\ntypedef unsigned Ucint;\ntypedef int cint;\n";
		file << "typedef unsigned long Ulong;\ntypedef unsigned long long Ullong;\ntypedef long long llong;\n";
		if (values.get(0)) file << "Ullong " << project << "_sizeof_var(Ullong var);\n";
		else file << "Ullong sizeof_var(Ullong var);\n";
		file << "byte pass();";
		std::ofstream Lfile;
		if (values.get(0)) {
			Lfile.open(project + "_default.cpp");
			Lfile << "#include <" << project << ".hpp>\n";
		}
		else {
			Lfile.open("defaults.cpp");
			Lfile << "#include <declarations.hpp>\n";
		}
		impl_sizeof_var(values, Lfile);
		Lfile << "byte pass(){\n"<<indent(1)<<"return 0;\n}";
		//Lfile << "byte::byte(unsigned char value){\n";
		//Lfile << "  this->data=value;\n";
		//Lfile << "}\n";
		//Lfile << "byte byte::add(byte other){\n";
		//Lfile << "  return this->data+other.data;\n";
		//Lfile << "}\n";
		//Lfile << "void byte::ass(byte other){\n";
		//Lfile << " this->data=other.data;\n";
		//Lfile << "}\n";
		Lfile.close();
	}
	void setup_c(std::ofstream& file) {
		file << "#include <stdio.h>\n#include <stdlib.h>\ntypedef unsigned char byte;\n";
		file << "typedef signed char Sbyte;\ntypedef unsigned short Umini;\ntypedef short mini;\ntypedef unsigned Ucint;\ntypedef int cint;\n";
		file << "typedef unsigned long Ulong;\ntypedef unsigned long long Ullong;\ntypedef long long llong;";
	}
	bool doth_constant_exist(std::string name) {
		unsigned index = 0;
		while (index < scopes.size()) if (does_constant_exist(scopes.at(index++).constants, name)) return 1;
		return 0;
	}
	unsigned get_constant_scope(std::string name) {
		unsigned index = scopes.size();
		while (index > 0) if (does_constant_exist(scopes.at(--index).constants, name)) break;
		return index;
	}
	bool doth_variable_exist(std::string name){
		unsigned index = 0;
		while (index < scopes.size()) if (does_variable_exist(scopes.at(index++).variables, name)) return 1;
		return 0;
	}
	unsigned get_variable_scope(std::string name) {
		unsigned index = scopes.size();
		while (index > 0) if (does_variable_exist(scopes.at(--index).variables, name)) break;
		return index;
	}
	bool doth_argument_exist(std::string name) {
		if (!functions.size()) return 0;
		return does_argument_exist(functions.at(functions.size() - 1).args, name);
	}
	bool is_library_included(std::string name) {
		unsigned index = 0;
		while (index < libs.size()) if (libs.at(index++).name == name) return 1;
		return 0;
	}
	bool is_function_in_library(std::string name) {
		unsigned index = 0;
		while (index < libs.size()) if(libs.at(index++).includeth_function(name)) return 1;
		return 0;
	}
	bool is_procedure_in_library(std::string name) {
		unsigned index = 0;
		while (index < libs.size()) if (libs.at(index++).includeth_procedure(name)) return 1;
		return 0;
	}
	bool is_class_in_library(std::string name) {
		unsigned index = 0;
		while (index < libs.size()) if (libs.at(index++).includeth_class(name)) return 1;
		return 0;
	}
	bool is_struct_in_library(std::string name) {
		unsigned index = 0;
		while (index < libs.size()) if (libs.at(index++).includeth_struct(name)) return 1;
		return 0;
	}
	Argument& get_argument(std::string name) {
		return functions.at(functions.size() - 1).args.at(find_argument(functions.at(functions.size() - 1).args, name));
	}
	Argument& get_proc_argument(std::string name) {
		return procedures.at(procedures.size() - 1).args.at(find_argument(procedures.at(procedures.size() - 1).args, name));
	}
	bool doth_proc_argument_exist(std::string name) {
		if (!procedures.size()) return 0;
		return does_argument_exist(procedures.at(procedures.size() - 1).args, name);
	}
	void compile_arguments(std::ofstream& file, Arguments& args) {
		unsigned index = 0;
		file << '(';
		while (index < args.size()) {
			if (index > 0) file << ',';
			file << types.at(args.at(index).value_type).name;
			if (args.at(index).type_of_argument) file << '*';
			file << ' ';
			file << args.at(index++).name;
		}
		file << ')';
	}
	void compile_default_call(std::ofstream& file, Arguments& args) {
		unsigned index = 0;
		file << '(';
		while (index < args.size()) {
			if (index > 0) file << ',';
			file << args.at(index++).name;
		}
		file << ')';
	}
	void compile_struct_members(std::ofstream& file,Class& cl) {
		file << '{';
		unsigned count = 0;
		while (count < cl.members.size()) {
			if (count) file << ',';
			file << "this->" << cl.members.at(count++).name;
		}
		file << '}';
	}
	void set_struct_members(std::ofstream& file, Class& cl,std::string name) {
		unsigned count = 0;
		while (count < cl.members.size()) file << "this->" << cl.members.at(count).name << '=' << name << '.' << cl.members.at(count++).name << ";\n" << indent(scopes.size() - 1);
	}
	void compile_method_arguments(std::ofstream& file, Arguments& args,std::string name) {
		unsigned index = 0;
		file << '(' << name << "* this";
		while (index < args.size()) {
			file << ',' << types.at(args.at(index).value_type).name;
			if (args.at(index).type_of_argument) file << '*';
			file << ' ';
			file << args.at(index++).name;
		}
		file << ')';
	}
	unsigned char read_argument_binding(BTokens& tkns, unsigned long long& index, Arguments& args) {
		while (index < tkns.size()) {
			switch (tkns.at(index).type) {
			case 0:
				args.push_back({ find_type(types,tkns.at(index++).value) });
				if (tkns.at(index).type == 8) {
					args.at(args.size() - 1).type_of_argument = 3;
					index++;
				}
				if (tkns.at(index).type) return 1;
				args.at(args.size() - 1).name = tkns.at(index).value;
				break;
			case 1:
				return 0;
			default:
				return 1;
			}
			index++;
		}
	}
	unsigned char read_function_binding(BTokens& tkns, unsigned long long& index, Functions& funcs) {
		if (tkns.at(index).type) return 1;
		funcs.push_back({ tkns.at(index++).value });
		if (tkns.at(index).type) return 1;
		funcs.at(funcs.size() - 1).Rtype = find_type(types, tkns.at(index++).value);
		if (tkns.at(index).type == 8) {
			funcs.at(funcs.size() - 1).ptr_type = 3;
			index++;
		}
		if (read_argument_binding(tkns, index, funcs.at(funcs.size() - 1).args)) return 1;
		return 0;
	}
	unsigned char read_procedure_binding(BTokens& tkns, unsigned long long& index, Procedures& procs) {
		if (tkns.at(index).type) return 1;
		procs.push_back({ tkns.at(index++).value });
		if (read_argument_binding(tkns, index, procs.at(procs.size() - 1).args)) return 1;
		return 0;
	}
	unsigned char read_member_binding(BTokens& tkns, unsigned long long& index, Variables& members) {
		if (tkns.at(index).type) return 1;
		members.push_back({ find_type(types, tkns.at(index++).value) });
		if (tkns.at(index).type == 8) {
			members.at(members.size() - 1).ptr_type = 3;
			index++;
		}
		if (tkns.at(index).type) return 1;
		members.at(members.size() - 1).name = tkns.at(index).value;
		if (!(tkns.at(index).type == 1)) return 1;
		return 0;
	}
	unsigned char read_class_binding(BTokens& tkns, unsigned long long& index,Classes& classes) {
		if (tkns.at(index).type) return 1;
		classes.push_back({ tkns.at(index++).value });
		if (!(tkns.at(index++).type == 1)) return 1;
		while (index < tkns.size()) {
			switch (tkns.at(index).type) {
			case 2:
				index++;
				return 0;
			case 4:
				index++;
				read_function_binding(tkns, index, classes.at(classes.size() - 1).methods);
				break;
			case 5:
				index++;
				read_procedure_binding(tkns, index, classes.at(classes.size() - 1).procedures);
				break;
			case 7:
				index++;
				read_member_binding(tkns, index, classes.at(classes.size() - 1).members);
				break;
			default:
				return 1;
			}
		}
		return 1;
	}
	unsigned char read_open_class_binding(BTokens& tkns, unsigned long long& index, Classes& classes) {
		if (tkns.at(index).type) return 1;
		classes.push_back({ tkns.at(index).value });
		classes.at(classes.size() - 1).is_open = 1;
		index++;
		if (!(tkns.at(index++).type == 1)) return 1;
		while (index < tkns.size()) {
			switch (tkns.at(index).type) {
			case 2:
				index++;
				return 0;
			case 4:
				index++;
				read_function_binding(tkns, index, classes.at(classes.size() - 1).methods);
				break;
			case 5:
				index++;
				read_procedure_binding(tkns, index, classes.at(classes.size() - 1).procedures);
				break;
			case 7:
				index++;
				read_member_binding(tkns, index, classes.at(classes.size() - 1).members);
				break;
			default:
				return 1;
			}
		}
		return 1;
	}
	unsigned char read_struct_binding(BTokens& tkns, unsigned long long& index, Structs& structs) {
		if (tkns.at(index).type) return 1;
		structs.push_back({ tkns.at(index++).value });
		if (!(tkns.at(index++).type == 1)) return 1;
		while (index < tkns.size()) {
			switch (tkns.at(index).type) {
			case 2:
				index++;
				return 0;
			case 7:
				index++;
				read_member_binding(tkns, index, structs.at(structs.size() - 1).members);
				break;
			default:
				return 1;
			}
		}
		return 1;
	}
	unsigned char read_bindings(std::string name) {
		BTokens tkns;
		libs.push_back({ name });
		Binding_lexer binds(name);
		tkns = binds.tokenize();
		print(tkns);
		std::cout << tkns.size() << '\n';
		unsigned long long index = 0;
		while (index < tkns.size()) {
			print(tkns, index);
			switch (tkns.at(index).type) {
			case 3:
				index++;
				if (tkns.at(index).type) return 1;
				if (!is_library_included(tkns.at(index).value)) read_bindings(tkns.at(index).value);
				index++;
				if (!(tkns.at(index).type == 1)) return 1;
				break;
			case 4:
				index++;
				if (read_function_binding(tkns, index, libs.at(libs.size() - 1).functions)) return 1;
				functions.push_back((Function)libs.at(libs.size() - 1).functions.at(libs.at(libs.size() - 1).functions.size() - 1));
				break;
			case 5:
				index++;
				if (read_procedure_binding(tkns, index, libs.at(libs.size() - 1).procedures)) return 1;
				procedures.push_back((Procedure)libs.at(libs.size() - 1).procedures.at(libs.at(libs.size() - 1).procedures.size() - 1));
				break;
			case 6:
				index++;
				if (read_class_binding(tkns, index,libs.at(libs.size()-1).classes)) return 1;
				classes.push_back((Class)libs.at(libs.size() - 1).classes.at(libs.at(libs.size() - 1).classes.size() - 1));
				types.push_back({ classes.at(classes.size() - 1).name });
				break;
			case 9:
				index++;
				if (read_open_class_binding(tkns, index, libs.at(libs.size() - 1).classes)) return 1;
				classes.push_back((Class)libs.at(libs.size() - 1).classes.at(libs.at(libs.size() - 1).classes.size() - 1));
				types.push_back({ classes.at(classes.size() - 1).name });
				break;
			case 10:
				index++;
				if (read_struct_binding(tkns, index, libs.at(libs.size() - 1).structs)) return 1;
				structs.push_back((Struct)libs.at(libs.size() - 1).structs.at(libs.at(libs.size() - 1).structs.size() - 1));
				types.push_back({ structs.at(structs.size() - 1).name });
				break;
			case 11:
				index++;
				if (tkns.at(index).type) return 1;
				types.at(types.size() - 1).start_character = tkns.at(index).value[0];
				break;
			default:
				return 1;
			}
			index++;
		}
		return 0;
	}
	unsigned long long compile_cpp(Tokens tkns,std::string& name,flags& values,system_type sys) {
		index = 0;
		print(tkns);
		std::ofstream file(name + ".cpp");
		if (!file.is_open()) {
			std::cout << "File cannot be openned\n!";
			return 2;
		}
		unsigned char stream_type = 0;
		unsigned long long last_var_type = 0;
		bool inClass = false;
		bool inStruct = false;
		bool isMember = false;
		bool inFunction = false;
		bool inProgram = false;
		bool is_last_varPointer = false;
		bool afterAssignment = false;
		bool afterReturn = false;
		bool inProcedure = false;
		bool afterImports = false;
		//bool afterOperation = false;
		if (values.get(0)) file << "#include <" + project + ".hpp>\n";
		else file << "#include <declarations.hpp>\n";
		while (index < tkns.size()) {
			if (inClass || inFunction || inProgram || inProcedure || inStruct || afterImports) afterImports = true;
			print(tkns, index);
			switch (tkns[index].type) {
			case 0:
				switch (tkns.at(index + 1).type) {
				case 0:
					{
						unsigned long long type = find_type(types, tkns.at(index).val);
						if (type == types.size()) return 1;
						if (isMember) {
							if (inClass) classes.at(classes.size() - 1).members.push_back({ type,0,tkns.at(index + 1).val });
							else if (inStruct) structs.at(structs.size() - 1).members.push_back({ type,0,tkns.at(index + 1).val });
							else return 1;
						}
						else {
							file << types.at(type).name << ' ' << tkns.at(index + 1).val;
							scopes.at(scopes.size() - 1).variables.push_back({ type,0,tkns.at(index + 1).val });
						}
						index++;
					}
					break;
				case 9:
					if (tkns.at(index + 2).type) return 1;
					{
						unsigned long long type = find_type(types, tkns.at(index).val);
						unsigned char ptr_type = 1;
						if (mode.get(2)) {
							ptr_type = 3;
							mode.store(2, 0);
						}
						else if (mode.get(0)) ptr_type = 2;
						if (type == types.size()) return 1;
						if (isMember) {
							if (inClass) classes.at(classes.size() - 1).members.push_back({ type,ptr_type,tkns.at(index + 2).val });
							else if (inStruct) structs.at(structs.size() - 1).members.push_back({ type,ptr_type,tkns.at(index + 2).val });
							else return 1;
						}
						else {
							file << types.at(type).name << "* " << tkns.at(index + 2).val;
							scopes.at(scopes.size() - 1).variables.push_back({ type,ptr_type,tkns.at(index + 2).val });
						}
						index+=2;
					}
					break;
				case 74:
					if (doth_variable_exist(tkns.at(index).val)) file << scopes.at(get_variable_scope(tkns.at(index).val)).variables.at(find_variable(scopes.at(get_variable_scope(tkns.at(index).val)).variables, tkns.at(index).val)).type;
					else if (inFunction && doth_argument_exist(tkns.at(index).val) && !inClass) file << get_argument(tkns.at(index).val).value_type;
					else if (inProcedure && doth_proc_argument_exist(tkns.at(index).val) && !inClass) file << get_proc_argument(tkns.at(index).val).value_type;
					else if (inFunction && inClass && classes.at(classes.size() - 1).doth_arg_exist(tkns.at(index).val)) file << classes.at(classes.size() - 1).get_arg(tkns.at(index).val).value_type;
					else if (inProcedure && inClass && classes.at(classes.size() - 1).doth_proc_arg_exist(tkns.at(index).val)) file << classes.at(classes.size() - 1).get_proc_arg(tkns.at(index).val).value_type;
					else if (does_function_exist(functions, tkns.at(index).val)) file << functions.at(find_function(functions, tkns.at(index).val)).Rtype;
					else if (does_type_exist(types, tkns.at(index).val)) {
						switch(tkns.at(index + 2).type) {
						case 12:
							if (tkns.at(index + 3).type) return 1;
							if (does_class_exist(classes, tkns.at(index).val)) {
								if (classes.at(find_class(classes, tkns.at(index).val)).is_open && classes.at(find_class(classes, tkns.at(index).val)).doth_member_exist(tkns.at(index + 3).val)) file << classes.at(find_class(classes, tkns.at(index).val)).get_member(tkns.at(index + 3).val).type;
								else if (classes.at(find_class(classes, tkns.at(index).val)).doth_method_exist(tkns.at(index + 3).val)) file << classes.at(find_class(classes, tkns.at(index).val)).get_method(tkns.at(index + 3).val).Rtype;
								else return 1;
							}
							else if (does_struct_exist(structs, tkns.at(index).val)) file << structs.at(find_struct(structs, tkns.at(index).val)).members.at(find_variable(structs.at(find_struct(structs, tkns.at(index).val)).members, tkns.at(index + 3).val)).type;
							index += 2;
							break;
						default:
							file << find_type(types, tkns.at(index).val);
						}
					}
					else return 1;
					index++;
					break;
				case 75:
					if (does_type_exist(types, tkns.at(index).val)) file << "sizeof(" << tkns.at(index++).val << ')';
					else if (doth_variable_exist(tkns.at(index).val)) {
						if (values.get(0)) file << project << "_sizeof_var(" << tkns.at(index++).val << ')';
						else file << "sizeof_var(" << tkns.at(index++).val << ')';
					}
					else return 1;
					break;
				default:
					if (doth_constant_exist(tkns.at(index).val)) file << scopes.at(get_constant_scope(tkns.at(index).val)).constants.at(find_constant(scopes.at(get_constant_scope(tkns.at(index).val)).constants, tkns.at(index).val)).value;
					else if (doth_variable_exist(tkns.at(index).val)) {
						last_var_type = scopes.at(get_variable_scope(tkns.at(index).val)).variables.at(find_variable(scopes.at(get_variable_scope(tkns.at(index).val)).variables, tkns.at(index).val)).type;
						is_last_varPointer = scopes.at(get_variable_scope(tkns.at(index).val)).variables.at(find_variable(scopes.at(get_variable_scope(tkns.at(index).val)).variables, tkns.at(index).val)).ptr_type;
						file << tkns.at(index).val;
					}
					else if (inFunction && doth_argument_exist(tkns.at(index).val)&&!inClass) {
						last_var_type = get_argument(tkns.at(index).val).value_type;
						is_last_varPointer = get_argument(tkns.at(index).val).type_of_argument;
						file << tkns.at(index).val;
					}
					else if (inProcedure && doth_proc_argument_exist(tkns.at(index).val)&&!inClass) {
						last_var_type = get_proc_argument(tkns.at(index).val).value_type;
						is_last_varPointer = get_proc_argument(tkns.at(index).val).type_of_argument;
						file << tkns.at(index).val;
					}
					else if (inFunction && inClass && classes.at(classes.size() - 1).doth_arg_exist(tkns.at(index).val)) {
						last_var_type = classes.at(classes.size() - 1).get_arg(tkns.at(index).val).value_type;
						is_last_varPointer = classes.at(classes.size() - 1).get_arg(tkns.at(index).val).type_of_argument;
						file << tkns.at(index).val;
					}
					else if (inProcedure && inClass && classes.at(classes.size() - 1).doth_proc_arg_exist(tkns.at(index).val)) {
						last_var_type = classes.at(classes.size() - 1).get_proc_arg(tkns.at(index).val).value_type;
						is_last_varPointer = classes.at(classes.size() - 1).get_proc_arg(tkns.at(index).val).type_of_argument;
						file << tkns.at(index).val;
					}
					else if (does_function_exist(functions, tkns.at(index).val)) {
						last_var_type = functions.at(find_function(functions, tkns.at(index).val)).Rtype;
						is_last_varPointer = functions.at(find_function(functions, tkns.at(index).val)).ptr_type;
						if (functions.at(find_function(functions, tkns.at(index).val)).no_discard && !afterAssignment) return 1;
						file << tkns.at(index).val;
						if (tkns.at(index + 1).type == 4) compile_default_call(file, functions.at(find_function(functions, tkns.at(index).val)).args);
						else if (!(tkns.at(index + 1).type == 42)) return 1;
					}
					else if (does_procedure_exist(procedures, tkns.at(index).val)) {
						file << tkns.at(index).val;
						if (tkns.at(index + 1).type == 4) compile_default_call(file, procedures.at(find_procedure(procedures, tkns.at(index).val)).args);
						else if (!(tkns.at(index + 1).type == 42)) return 1;
					}
					else if (inProgram) {
						if (tkns.at(index).val == "argc") file << "argc";
						else if (tkns.at(index).val == "argv") {
							file << "argv";
							if (tkns.at(index + 1).type == 1) {
								file << '[' << tkns.at(index + 1).val << ']';
								index++;
							}
						}
					}
					else return 1;
				}
				break;
			case 1:
			if(!mode.get(4)){
				unsigned var_index = 0;
				unsigned scope_index = 1;
				while (!scopes.at(scopes.size()-scope_index).is_function) {
					var_index = 0;
					while (var_index < scopes[scopes.size() - scope_index].variables.size()) {
						switch (scopes[scopes.size() - scope_index].variables[var_index].ptr_type) {
						case 1:
							file << indent(scopes.size()) << "delete " << scopes[scopes.size() - scope_index].variables[var_index].name << ";\n";
							break;
						case 2:
							file << indent(scopes.size()) << "delete[] " << scopes[scopes.size() - scope_index].variables[var_index].name << ";\n";
							break;
						}
						index++;
					}
					scope_index++;
				}
			}
			file << "return ";
			if ((tkns.at(index + 1).type == 0) || (tkns.at(index + 1).type == 2) || (tkns.at(index + 1).type == 3)) {
				index++;
				file << tkns.at(index).val;
			}
			afterReturn = true;
			break;
			case 2:
				file << tkns.at(index).val;
				break;
			case 3:
				file << '"' << tkns.at(index).val << '"';
				break;
			case 4:
				last_var_type = 0;
				is_last_varPointer = 0;
				[[unlikely]] if (stream_type) stream_type = 0;
				if (inClass||inStruct) {
					if (tkns.at(index + 1).type == 39) file << ";\n" << indent(scopes.size() - 3);
					else if (!isMember) file << ";\n" << indent(scopes.size() - 2);
				}
				else if (tkns.at(index + 1).type == 39) file << ";\n" << indent(scopes.size() - 2);
				else file << ";\n" << indent(scopes.size() - 1);
				isMember = false;
				break;
			case 5:
				/*if (!tkns.at(index - 1).type) {
					file << ".add(";
					index++;
					if (tkns.at(index).type == 9) {
						file << '*';
						index++;
					}
					if (tkns.at(index).type) return 1;
					else file << tkns.at(index).val << ')';
				}
				else file << '+';*/
				file << '+';
				break;
			case 6:
				file << '-';
				break;
			case 7:
				file << '*';
				break;
			case 8:
				file << '/';
				break;
			case 9:
				index++;
				[[unlikely]] if (tkns.at(index).type) return 1;
				[[likely]] if (!(doth_variable_exist(tkns.at(index).val))) return 1;
				[[unlikely]] if (!(scopes.at(get_variable_scope(tkns.at(index).val)).variables.at(find_variable(scopes.at(get_variable_scope(tkns.at(index).val)).variables, tkns.at(index).val)).ptr_type)) return 1;
				file << "(*" << tkns.at(index).val << ')';
				break;
			case 10:
				file << '~';
				break;
			case 11:
				index++;
				[[unlikely]] if (inClass) return 1;
				[[unlikely]] if (tkns.at(index).type) return 1;
				[[unlikely]] if (does_variable_exist(classes.at(classes.size() - 1).members, tkns.at(index).val)) last_var_type = classes.at(classes.size() - 1).members.at(find_variable(classes.at(classes.size() - 1).members, tkns.at(index).val)).type;
				else if (does_function_exist(classes.at(index).methods, tkns.at(index).val)) {
					last_var_type = classes.at(classes.size() - 1).methods.at(find_function(classes.at(classes.size() - 1).methods, tkns.at(index).val)).Rtype;
				}
				else if (does_procedure_exist(classes.at(index).procedures, tkns.at(index).val)) pass();
				else return 1;
				file << "this->" << tkns.at(index).val;
				if (does_function_exist(classes.at(classes.size() - 1).methods, tkns.at(index).val)) {
					if (classes.at(classes.size() - 1).methods.at(find_function(classes.at(classes.size() - 1).methods, tkns.at(index).val)).no_discard && !afterAssignment) return 1;
					if (tkns.at(index + 1).type == 4) compile_default_call(file, classes.at(classes.size() - 1).methods.at(find_function(classes.at(classes.size() - 1).methods, tkns.at(index).val)).args);
					else if (!(tkns.at(index + 1).type == 42)) return 1;
				}
				else if (does_procedure_exist(classes.at(classes.size() - 1).procedures, tkns.at(index).val)) {
					if (tkns.at(index + 1).type == 4) compile_default_call(file, classes.at(classes.size() - 1).procedures.at(find_procedure(classes.at(classes.size() - 1).procedures, tkns.at(index).val)).args);
					else if (!(tkns.at(index + 1).type == 42)) return 1;
				}
				break;
			case 12:
				index++;
				[[unlikely]] if (tkns.at(index).type) return 1;
				if (does_class_exist(classes, types.at(last_var_type).name)) {
					[[likely]] if ((!(does_function_exist(classes.at(find_class(classes, types.at(last_var_type).name)).methods, tkns.at(index).val))) && (!(does_procedure_exist(classes.at(find_class(classes, types.at(last_var_type).name)).procedures, tkns.at(index).val)))) {
						[[unlikely]] if (classes.at(find_class(classes, types.at(last_var_type).name)).is_open) {
							[[likely]] if (!(does_variable_exist(classes.at(find_class(classes, types.at(last_var_type).name)).members, tkns.at(index).val))) return 1;
						}
						else return 1;
					}
					if (is_last_varPointer) file << "->";
					else file << '.';
					file << tkns.at(index).val;
					if (does_function_exist(classes.at(find_class(classes, types.at(last_var_type).name)).methods, tkns.at(index).val)) {
						if (classes.at(find_class(classes, types.at(last_var_type).name)).methods.at(find_function(classes.at(find_class(classes, types.at(last_var_type).name)).methods, tkns.at(index).val)).no_discard && !afterAssignment) return 1;
						is_last_varPointer = classes.at(find_class(classes, types.at(last_var_type).name)).methods.at(find_function(classes.at(find_class(classes, types.at(last_var_type).name)).methods, tkns.at(index).val)).ptr_type;
						last_var_type = classes.at(find_class(classes, types.at(last_var_type).name)).methods.at(find_function(classes.at(find_class(classes, types.at(last_var_type).name)).methods, tkns.at(index).val)).Rtype;
						if (tkns.at(index + 1).type == 4) compile_default_call(file, classes.at(find_class(classes, types.at(last_var_type).name)).methods.at(find_function(classes.at(find_class(classes, types.at(last_var_type).name)).methods, tkns.at(index).val)).args);
						else if (!(tkns.at(index + 1).type == 42)) return 1;
					}
					else if (does_procedure_exist(classes.at(find_class(classes, types.at(last_var_type).name)).procedures, tkns.at(index).val)) {
						if (tkns.at(index + 1).type == 4) compile_default_call(file, classes.at(find_class(classes, types.at(last_var_type).name)).procedures.at(find_procedure(classes.at(find_class(classes, types.at(last_var_type).name)).procedures, tkns.at(index).val)).args);
						else if (!(tkns.at(index + 1).type == 42)) return 1;
					}
					else if (does_variable_exist(classes.at(find_class(classes, types.at(last_var_type).name)).members, tkns.at(index).val)) {
						is_last_varPointer = classes.at(find_class(classes, types.at(last_var_type).name)).members.at(find_variable(classes.at(find_class(classes, types.at(last_var_type).name)).members, tkns.at(index).val)).ptr_type;
						last_var_type = classes.at(find_class(classes, types.at(last_var_type).name)).members.at(find_variable(classes.at(find_class(classes, types.at(last_var_type).name)).members, tkns.at(index).val)).type;
					}
				}
				else if (does_struct_exist(structs, types.at(last_var_type).name)) {
					[[likely]] if (!(does_variable_exist(structs.at(find_struct(structs, types.at(last_var_type).name)).members, tkns.at(index).val))) return 1;
					if (is_last_varPointer) file << "->";
					else file << ',';
					file << tkns.at(index).val;
					is_last_varPointer = structs.at(find_struct(structs, types.at(last_var_type).name)).members.at(find_variable(structs.at(find_struct(structs, types.at(last_var_type).name)).members, tkns.at(index).val)).ptr_type;
					last_var_type = structs.at(find_struct(structs, types.at(last_var_type).name)).members.at(find_variable(structs.at(find_struct(structs, types.at(last_var_type).name)).members, tkns.at(index).val)).type;
				}
				else return 1;
				break;
			case 13:
				file << "-=";
				afterAssignment = true;
				break;
			case 14:
				file << '&';
				break;
			case 15:
				file << "--";
				break;
			case 16:
				file << "++";
				break;
			case 17:
				file << "+=";
				afterAssignment = true;
				break;
			case 18:
				file << "*=";
				afterAssignment = true;
				break;
			case 19:
				std::cout << "Not yet implemented!\n";
				break;
			case 20:
				file << "&&";
				break;
			case 21:
				std::cout << "Not yet implemented!\n";
				break;
			case 22:
				file << "&=";
				afterAssignment = true;
				break;
			case 23:
				file << '<';
				break;
			case 24:
				if (!(stream_type == 2)) file << "<<";
				else return 1;
				break;
			case 25:
				file << "<<=";
				afterAssignment = true;
				break;
			case 26:
				file << '>';
				break;
			case 27:
				if (!(stream_type == 1)) file << ">>";
				else return 1;
				break;
			case 28:
				file << ">>=";
				afterAssignment = true;
				break;
			case 29:
				file << '|';
				break;
			case 30:
				file << "||";
				break;
			case 31:
				file << "|=";
				afterAssignment = true;
				break;
			case 32:
				file << '^';
				break;
			case 33:
				std::cout << "Not yet implemented!\n";
				break;
			case 34:
				file << "^=";
				afterAssignment = true;
				break;
			case 35:
				//file << ".ass(";
				//index++;
				//if (tkns.at(index).type == 9) {
				//	file << '*';
				//	index++;
				//}
				file << '=';
				afterAssignment = true;
				break;
			case 36:
				file << "==";
				break;
			case 37:
				if (tkns.at(index).val == "ARRAY_POINTER") mode.store(0, 1);
				else if (tkns.at(index).val == "POINTER_FUNCTION") mode.store(1, 1);
				else if (tkns.at(index).val == "KEEP_POINTER") mode.store(2, 1);
				else if (tkns.at(index).val == "NO_DISCARD") mode.store(3, 1);
				else if (tkns.at(index).val == "AUTO") mode.store(4, 0);
				else if (tkns.at(index).val == "MANUAL") mode.store(4, 1);
				else if (tkns.at(index).val == "VARS") {
					unsigned scope_index = 0;
					unsigned var_index;
					Variable temp;
					while (scope_index < scopes.size()) {
						var_index = 0;
						while (var_index < scopes.at(scope_index).variables.size()) {
							temp = scopes.at(scope_index).variables.at(var_index);
							std::cout << scope_index << ',' << temp.name << ',' << temp.type << ',' << temp.ptr_type << '\n';
							var_index++;
						}
						scope_index++;
					}
				}
				else if (tkns.at(index).val == "CONSTS") {
					unsigned scope_index = 0;
					unsigned var_index;
					Constant temp;
					while (scope_index < scopes.size()) {
						var_index = 0;
						while (var_index < scopes.at(scope_index).constants.size()) {
							temp = scopes.at(scope_index).constants.at(var_index);
							std::cout << scope_index << ',' << temp.name << ',' << temp.value << '\n';
							var_index++;
						}
						scope_index++;
					}
				}
				else if (tkns.at(index).val == "TYPE") {
					index++;
					if (tkns.at(index).type) return 1;
					std::cout << types.at(scopes.at(get_variable_scope(tkns.at(index).val)).variables.at(find_variable(scopes.at(get_variable_scope(tkns.at(index).val)).variables, tkns.at(index).val)).type).name;
				}
				else if (tkns.at(index).val == "CREATE") mode.store(5, 0);
				else if (tkns.at(index).val == "DEFINE") mode.store(5, 1);
				else if (tkns.at(index).val == "EMBED_DEFINITION") mode.store(6, 0);
				else if (tkns.at(index).val == "OVERRIDE") mode.store(6, 1);
				else if (tkns.at(index).val == "LAST_PROC") procedures.at(procedures.size() - 1).print(types);
				else if (tkns.at(index).val == "PROCS") {
					unsigned proc_index = 0;
					while (proc_index < procedures.size()) procedures.at(proc_index++).print(types);
				}
				else if (tkns.at(index).val == "FUNCS") {
					unsigned func_index = 0;
					while (func_index < functions.size()) functions.at(func_index++).print(types);
				}
				break;
			case 38:
				if (inClass||inStruct) file << "{\n" << indent(scopes.size() - 1);
				else file << "{\n" << indent(scopes.size());
				scopes.push_back({});
				break;
			case 39:
				if ((!mode.get(4))&&(!afterReturn)) {
					while (scopes[scopes.size() - 1].variables.size() > 0) {
						switch (scopes[scopes.size() - 1].variables[scopes[scopes.size()-1].variables.size()-1].ptr_type) {
						case 1:
							if(inClass) file << indent(scopes.size() - 2) << "delete " << scopes[scopes.size() - 1].variables[scopes[scopes.size() - 1].variables.size() - 1].name << ";\n";
							else file << indent(scopes.size()-1) << "delete " << scopes[scopes.size() - 1].variables[scopes[scopes.size() - 1].variables.size() - 1].name << ";\n";
							break;
						case 2:
							if(inClass) file << indent(scopes.size() - 2) << "delete[] " << scopes[scopes.size() - 1].variables[scopes[scopes.size() - 1].variables.size() - 1].name << ";\n";
							else file << indent(scopes.size()-1) << "delete[] " << scopes[scopes.size() - 1].variables[scopes[scopes.size() - 1].variables.size() - 1].name << ";\n";
							break;
						default:
							break;
						}
						scopes[scopes.size() - 1].variables.pop_back();
					}
				}
				if (scopes.at(scopes.size() - 1).is_class) {
					inClass = false;
					inStruct = false;
				}
				else file << "}\n";
				if (scopes.at(scopes.size() - 1).is_function) {
					inFunction = false;
					inProcedure = false;
				}
				if (((index+1)!=tkns.size())&&(tkns.at(index + 1).type != 39)) {
					if (inClass) file << indent(scopes.size() - 3);
					else file << indent(scopes.size() - 2);
				}
				scopes.pop_back();
				afterReturn = false;
				break;
			case 40:
				file << "int main(int argc,char** argv)";
				inProgram = true;
				break;
			case 41:
				inClass = true;
				index++;
				switch (tkns.at(index).type) {
				case 0:
					classes.push_back({ tkns.at(index).val });
					break;
				case 42:
					{
						std::string name = "";
						bool test = true;
						while (test) {
							index++;
							switch (tkns.at(index).type) {
							case 0:
								name+=tkns.at(index).val;
								break;
							case 43:
								test = false;
								break;
							}
						}
						classes.push_back({ name });
					}
					break;
				}
				index++;
				types.push_back({ classes.at(classes.size() - 1).name });
				if (!(tkns.at(index).type == 38)) return 1;
				scopes.push_back({ .is_class = true });
				break;
			case 42:
				file << '(';
				break;
			case 43:
				file << ')';
				break;
			case 44:
				file << '[';
				break;
			case 45:
				file << ']';
				break;
			case 46:
				file << '!';
				break;
			case 47:
				index++;
				[[unlikely]] if (tkns.at(index).type == 9) {
					index++;
					[[unlikely]] if (tkns.at(index).type) return 1;
					else {
						unsigned long long type = find_type_with_char(types, tkns.at(index).val[0]);
						if (type == types.size()) return 1;
						if (isMember) classes.at(classes.size() - 1).members.push_back({ type,3,tkns.at(index).val });
						else {
							file << types.at(type).name << "* " << tkns.at(index).val;
							scopes.at(scopes.size() - 1).variables.push_back({ type,3,tkns.at(index).val });
						}
					}
				}
				else if (tkns.at(index).type) return 1;
				else {
					unsigned long long type = find_type_with_char(types, tkns.at(index).val[0]);
					if (type == types.size()) return 1;
					if (isMember) classes.at(classes.size() - 1).members.push_back({ type,0,tkns.at(index).val });
					else {
						file << types.at(type).name << ' ' << tkns.at(index).val;
						scopes.at(scopes.size() - 1).variables.push_back({ type,0,tkns.at(index).val });
					}
				}
				break;
			case 48:
				if (doth_constant_exist(tkns.at(index + 1).val)) {
					{
						Constant& contst = scopes.at(get_constant_scope(tkns.at(index + 1).val)).constants.at(find_constant(scopes.at(get_constant_scope(tkns.at(index + 1).val)).constants, tkns.at(index + 1).val));
						switch (tkns.at(index + 2).type) {
						case 0:
							if (doth_constant_exist(tkns.at(index + 2).val)) contst.value = scopes.at(get_constant_scope(tkns.at(index + 2).val)).constants.at(find_constant(scopes.at(get_constant_scope(tkns.at(index + 1).val)).constants, tkns.at(index + 1).val)).value;
							else contst.value = tkns.at(index + 2).val;
							break;
						case 16:
							contst.value = std::to_string(std::stoi(contst.value) + 1);
							break;
						}
					}
				}
				else scopes[scopes.size() - 1].constants.push_back({ tkns[index + 1].val,tkns[index + 2].val });
				index += 2;
				break;
			case 49:
				file << "if";
				break;
			case 50:
				file << "else if";
				break;
			case 51:
				file << "else";
				break;
			case 52:
				file << "if(";
				if (!(tkns.at(index + 1).type == 42)) return 1;
				index++;
				break;
			case 53:
				file << "while";
				break;
			case 54:
				file << "while(!";
				if (!(tkns.at(index + 1).type == 42)) return 1;
				index++;
				break;
			case 55:
				if (inStruct) return 1;
				index++;
				if (!(!tkns.at(index).type)&&(!tkns.at(index+1).type)) return 1;
				{
					Token ret = tkns.at(index);
					Token name = tkns.at(index + 1);
					index+=2;
					Arguments args;
					unsigned char pointer_type = 0;
					if (inClass) file << ret.val << ' ' << classes.at(classes.size() - 1).name << "::" << name.val;
					else file << ret.val << ' ' << name.val;
					if (tkns.at(index).type == 42) {
						index++;
						{
							Args temp = get_args(file, tkns);
							if (temp.flags) return 1;
							args = temp.args;
						}
					}
					else if (tkns.at(index).type == 38) file<<"()";
					if (mode.get(1)) {
						if (mode.get(0)) pointer_type = 2;
						else pointer_type = 1;
						mode.store(0, 0);
						mode.store(1, 0);
					}
					if (!mode.get(5)) {
						if (inClass) classes.at(classes.size() - 1).methods.push_back({ name.val, args, find_type(types, ret.val),pointer_type,mode.get(3) });
						else functions.push_back({ name.val,args,find_type(types,ret.val),pointer_type,mode.get(3) });
					}
					mode.store(3, 0);
				}
				inFunction = true;
				if (!(tkns.at(index).type == 38)) return 1;
				if (inClass) file << "{\n" << indent(scopes.size() - 1);
				else file << "{\n" << indent(scopes.size());
				scopes.push_back({ .is_function = true });
				break;
			case 56:
				switch (sys) {
				case 0:
					file << "\"cls\"";
					break;
				}
				break;
			case 57:
				file << ',';
				break;
			case 58:
				isMember = true;
				break;
			case 59:
				file << "std::cout";
				stream_type = 1;
				break;
			case 60:
				if (inStruct) return 1;
				index++;
				if (tkns.at(index).type) return 1;
				{
					Token name = tkns.at(index);
					index++;
					Arguments args;
					unsigned char pointer_type = 0;
					if (inClass) file << "void " << classes.at(classes.size() - 1).name << "::" << name.val;
					else file << "void " << name.val;
					if (tkns.at(index).type == 42) {
						index++;
						{
							Args temp = get_args(file, tkns);
							if (temp.flags) return 1;
							args = temp.args;
						}
					}
					else if (tkns.at(index).type == 38) file << "()";
					if (inClass) classes.at(classes.size() - 1).procedures.push_back({ name.val,args });
					else procedures.push_back({ name.val,args });
				}
				inProcedure = true;
				if (!(tkns.at(index).type == 38)) return 1;
				if (inClass||inStruct) file << "{\n" << indent(scopes.size() - 1);
				else file << "{\n" << indent(scopes.size());
				scopes.push_back({ .is_function = true });
				break;
			case 61:
				file << "std::cin";
				stream_type = 2;
				break;
			case 62:
				[[unlikely]] if (!(inClass||inStruct)) return 1;
				file << find_type(types, classes.at(classes.size() - 1).name);
				break;
			case 63:
				file << '%';
				break;
			case 64:
				index++;
				switch (tkns.at(index).type) {
				case 0:
					if (tkns.at(index).type) return 1;
					if (!doth_variable_exist(tkns.at(index).val)) return 1;
					file << tkns.at(index).type << "=new " << types.at(scopes.at(get_variable_scope(tkns.at(index).val)).variables.at(find_variable(scopes.at(get_variable_scope(tkns.at(index).val)).variables, tkns.at(index).val)).type).name;
					switch (scopes.at(get_variable_scope(tkns.at(index).val)).variables.at(find_variable(scopes.at(get_variable_scope(tkns.at(index).val)).variables, tkns.at(index).val)).ptr_type) {
					case 1:
						if ((tkns.at(index + 1).type == 0) || (tkns.at(index + 1).type == 2)) {
							index++;
							file << '{' << tkns.at(index).val << '}';
						}
						else if (tkns.at(index + 1).type == 3) {
							index++;
							file << "{\"" << tkns.at(index).val << "\"}";
						}
						break;
					case 2:
						index++;
						file << '[' << tkns.at(index).val << ']';
						mode.store(0, 0);
						if ((tkns.at(index + 1).type == 0) || (tkns.at(index + 1).type == 2)) {
							index++;
							file << '{' << tkns.at(index).val << '}';
						}
						else if (tkns.at(index + 1).type == 3) {
							index++;
							file << "{\"" << tkns.at(index).val << "\"}";
						}
						break;
					case 3:
						if (mode.get(0)) {
							index++;
							if (!((tkns.at(index).type == 0) || (tkns.at(index).type == 2))) return 1;
							file << '[' << tkns.at(index).val << ']';
							mode.store(0, 0);
						}
						if ((tkns.at(index + 1).type == 0) || (tkns.at(index + 1).type == 2)) {
							index++;
							file << '{' << tkns.at(index).val << '}';
						}
						else if (tkns.at(index + 1).type == 3) {
							index++;
							file << "{\"" << tkns.at(index).val << "\"}";
						}
						break;
					default:
						return 1;
					}
					break;
				case 11:
					index++;
					if (tkns.at(index).type) return 1;
					if (!does_variable_exist(classes.at(classes.size() - 1).members, tkns.at(index).val)) return 1;
					file << "this->" << tkns.at(index).type << "=new " << types.at(classes.at(classes.size() - 1).members.at(find_variable(classes.at(classes.size() - 1).members, tkns.at(index).val)).type).name;
					switch (classes.at(classes.size()-1).members.at(find_variable(classes.at(classes.size()-1).members,tkns.at(index).val)).ptr_type) {
					case 1:
						if ((tkns.at(index + 1).type==0)||(tkns.at(index+1).type==2)) {
							index++;
							file << '{' << tkns.at(index).val << '}';
						}
						else if (tkns.at(index + 1).type == 3) {
							index++;
							file << "{\"" << tkns.at(index).val << "\"}";
						}
						break;
					case 2:
						index++;
						if (!((tkns.at(index).type == 0) || (tkns.at(index).type == 2))) return 1;
						file << '[' << tkns.at(index).val << ']';
						mode.store(0, 0);
						if ((tkns.at(index + 1).type == 0) || (tkns.at(index + 1).type == 2)) {
							index++;
							file << '{' << tkns.at(index).val << '}';
						}
						else if (tkns.at(index + 1).type == 3) {
							index++;
							file << "{\"" << tkns.at(index).val << "\"}";
						}
						break;
					case 3:
						if (mode.get(0)) {
							index++;
							if (!((tkns.at(index).type == 0) || (tkns.at(index).type == 2))) return 1;
							file << '[' << tkns.at(index).val << ']';
							mode.store(0, 0);
						}
						if ((tkns.at(index + 1).type == 0) || (tkns.at(index + 1).type == 2)) {
							index++;
							file << '{' << tkns.at(index).val << '}';
						}
						else if (tkns.at(index + 1).type == 3) {
							index++;
							file << "{\"" << tkns.at(index).val << "\"}";
						}
						break;
					default:
						return 1;
					}
					break;
				default:
					return 1;
				}
				break;
			case 65:
				index++;
				switch (tkns.at(index).type) {
				case 0:
					if (!doth_variable_exist(tkns.at(index).val)) return 1;
					switch (scopes.at(get_variable_scope(tkns.at(index).val)).variables.at(find_variable(scopes.at(get_variable_scope(tkns.at(index).val)).variables, tkns.at(index).val)).ptr_type) {
					case 1:
						file << "delete ";
						break;
					case 2:
						file << "delete[] ";
						break;
					case 3:
						if (mode.get(0)) file << "delete[] ";
						else file << "delete ";
						mode.store(0, 0);
						break;
					default:
						return 1;
					}
					file << tkns.at(index).val;
					break;
				case 11:
					index++;
					if (tkns.at(index).type) return 1;
					if (!does_variable_exist(classes.at(classes.size() - 1).members, tkns.at(index).val)) return 1;
					switch (classes.at(classes.size() - 1).members.at(find_variable(classes.at(classes.size() - 1).members, tkns.at(index).val)).ptr_type) {
					case 1:
						file << "delete ";
						break;
					case 2:
						file << "delete[] ";
						break;
					case 3:
						if (mode.get(0)) file << "delete[] ";
						else file << "delete ";
						mode.store(0, 0);
						break;
					default:
						return 1;
					}
					file << tkns.at(index).val;
					break;
				default:
					return 1;
				}
				break;
			case 66:
				file << "%=";
				break;
			case 67:
				index++;
				if (tkns.at(index).type) return 1;
				types.at(types.size() - 1).start_character = tkns.at(index).val.at(0);
				index++;
				if (!(tkns.at(index).type == 4)) return 1;
				break;
			case 68:
			{
				if (!(inClass||inStruct)) return 1;
				unsigned char embed_type = 0;
				index++;
				if (tkns.at(index).type) return 1;
				if (!does_class_exist(classes, tkns.at(index).val)) {
					if (!does_struct_exist(structs, tkns.at(index).val)) return 1;
					else embed_type = 1;
				}
				if (embed_type) {
					if (inStruct && find_struct(structs, tkns.at(index).val) == (structs.size() - 1)) return 1;
					unsigned count = 0;
					while (count < structs.at(find_struct(structs, tkns.at(index).val)).members.size()) {

					}
				}
				else{
					if (inStruct) return 1;
					if (inClass&&find_class(classes, tkns.at(index).val) == (classes.size() - 1)) return 1;
					unsigned count = 0;
					while (count < classes.at(find_class(classes, tkns.at(index).val)).members.size()) {
						if (!(find_variable(classes.at(classes.size() - 1).members, classes.at(find_class(classes, tkns.at(index).val)).members.at(count).name) == classes.at(classes.size() - 1).members.size())) return 1;
						classes.at(classes.size() - 1).members.push_back(classes.at(find_class(classes, tkns.at(index).val)).members.at(count++));
					}
					count = 0;
					while (count < classes.at(find_class(classes, tkns.at(index).val)).methods.size()) {
						if (!(find_function(classes.at(classes.size() - 1).methods, classes.at(find_class(classes, tkns.at(index).val)).methods.at(count).name) == classes.at(classes.size() - 1).methods.size())) return 1;
						classes.at(classes.size() - 1).methods.push_back(classes.at(find_class(classes, tkns.at(index).val)).methods.at(count));
						if (!mode.get(6)) {
							file << types.at(classes.at(classes.size() - 1).methods.at(classes.at(classes.size() - 1).methods.size() - 1).Rtype).name << ' ';
							file << classes.at(classes.size() - 1).name << "::";
							file << classes.at(classes.size() - 1).methods.at(classes.at(classes.size() - 1).methods.size() - 1).name;
							compile_arguments(file, classes.at(classes.size() - 1).methods.at(classes.at(classes.size() - 1).methods.size() - 1).args);
							file << "{\n" << indent(scopes.size() - 1);
							file << classes.at(find_class(classes, tkns.at(index).val)).name << " temp=";
							compile_struct_members(file, classes.at(find_class(classes, tkns.at(index).val)));
							file << ";\n" << indent(scopes.size() - 1);
							file << types.at(classes.at(classes.size() - 1).methods.at(classes.at(classes.size() - 1).methods.size() - 1).Rtype).name << " ret=";
							file << "temp." << classes.at(classes.size() - 1).methods.at(classes.at(classes.size() - 1).methods.size() - 1).name;
							compile_default_call(file, classes.at(classes.size() - 1).methods.at(classes.at(classes.size() - 1).methods.size() - 1).args);
							file << ";\n" << indent(scopes.size() - 1);
							set_struct_members(file, classes.at(find_class(classes, tkns.at(index).val)), "temp");
							file << "return ret;\n" << indent(scopes.size() - 2);
							file << "}\n" << indent(scopes.size() - 2);
						}
						count++;
					}
					count = 0;
					while (count < classes.at(find_class(classes, tkns.at(index).val)).procedures.size()) {
						if (!(find_procedure(classes.at(classes.size() - 1).procedures, classes.at(find_class(classes, tkns.at(index).val)).procedures.at(count).name) == classes.at(classes.size() - 1).procedures.size())) return 1;
						classes.at(classes.size() - 1).procedures.push_back(classes.at(find_class(classes, tkns.at(index).val)).procedures.at(count));
						if (!mode.get(6)) {
							file << "void ";
							file << classes.at(classes.size() - 1).name << "::";
							file << classes.at(classes.size() - 1).procedures.at(classes.at(classes.size() - 1).procedures.size() - 1).name;
							compile_arguments(file, classes.at(classes.size() - 1).procedures.at(classes.at(classes.size() - 1).procedures.size() - 1).args);
							file << "{\n" << indent(scopes.size() - 1);
							file << classes.at(find_class(classes, tkns.at(index).val)).name << " temp=";
							compile_struct_members(file, classes.at(find_class(classes, tkns.at(index).val)));
							file << ";\n" << indent(scopes.size() - 1);
							file << "temp." << classes.at(classes.size() - 1).procedures.at(classes.at(classes.size() - 1).procedures.size() - 1).name;
							compile_default_call(file, classes.at(classes.size() - 1).procedures.at(classes.at(classes.size() - 1).procedures.size() - 1).args);
							file << ";\n" << indent(scopes.size() - 1);
							set_struct_members(file, classes.at(find_class(classes, tkns.at(index).val)), "temp");
							file.seekp(file.tellp().operator-(1));
							file << "}\n" << indent(scopes.size() - 2);
						}
						count++;
					}
				}
				}
				index++;
				if (!(tkns.at(index).type == 4)) return 1;
				break;
			case 69:
				if (afterImports) return 1;
				index++;
				if (tkns.at(index).type) return 1;
				if (!is_library_included(tkns.at(index).val)) {
					if (read_bindings(tkns.at(index).val)) {
						std::cout << "Failed to read bindings!\n";
						return 1;
					}
					else std::cout << "Included!\n";
				}
				else {
					std::cout << "Already included!\n";
				}
				index++;
				if (!(tkns.at(index).type == 4)) return 1;
				break;
			case 70:
				index++;
				if (tkns.at(index).type) return 1;
				if (!does_type_exist(types, tkns.at(index).val)) return 1;
				file << '(' << types.at(find_type(types, tkns.at(index).val)).name;
				if (tkns.at(index + 1).type == 9) {
					index++;
					file << '*';
				}
				file << ')';
				break;
			case 71:
				inClass = true;
				index++;
				switch (tkns.at(index).type) {
				case 0:
					classes.push_back({ tkns.at(index).val });
					break;
				case 42:
				{
					std::string name = "";
					bool test = true;
					while (test) {
						index++;
						switch (tkns.at(index).type) {
						case 0:
							name += tkns.at(index).val;
							break;
						case 43:
							test = false;
							break;
						}
					}
					classes.push_back({ name });
				}
				break;
				}
				classes.at(classes.size() - 1).is_open = 1;
				index++;
				types.push_back({ classes.at(classes.size() - 1).name });
				if (!(tkns.at(index).type == 38)) return 1;
				scopes.push_back({ .is_class = true });
				break;
			case 72:
				inStruct = true;
				index++;
				switch (tkns.at(index).type) {
				case 0:
					structs.push_back({ tkns.at(index).val });
					break;
				case 42:
				{
					std::string name = "";
					bool test = true;
					while (test) {
						index++;
						switch (tkns.at(index).type) {
						case 0:
							name += tkns.at(index).val;
							break;
						case 42:
							test = false;
							break;
						}
					}
					structs.push_back({ name });
				}
				index++;
				types.push_back({ structs.at(structs.size() - 1).name });
				if (!(tkns.at(index).type == 38)) return 1;
				scopes.push_back({ .is_class = true });
				}
				break;
			case 73:
				file << '0';
				break;
			case 76:
				file << '&';
				break;
			case 77:
				file << "while(1)";
				break;
			case 78:
				file << "pass()";
				break;
			}
			index++;
		}
		file.close();
		return 0;
	}
	unsigned long long compile_asm(node* base_node) {
		return pass();
	}
	unsigned long long compile_java() {
		return pass();
	}
	unsigned long long compile_c(Tokens tkns,std::string& name,flags& values,system_type sys) {
		index = 0;
		std::ofstream file(name + ".c");
		if (!file.is_open()) {
			std::cout << "File cannot be openned!\n";
			return 2;
		}
		if (values.get(0)) file << "#include <" + project + ".h>\n";
		else file << "#include <declarations.h>\n";
		bool inClass = false;
		bool inFunction = false;
		bool inProcedure = false;
		bool inProgram = false;
		bool isMember = false;
		bool afterReturn = false;
		bool afterImports = false;
		while (index < tkns.size()) {
			if (inClass || inFunction || inProcedure || inProgram || afterImports) afterImports = 1;
			switch (tkns.at(index).type) {
			case 0:
				switch (tkns.at(index + 1).type) {
				case 0:
					{
						unsigned long long type = find_type(types, tkns.at(index).val);
						if (type == types.size()) return 1;
						if (isMember) classes.at(classes.size() - 1).members.push_back({ type,0,tkns.at(index + 1).val });
						else {
							file << types.at(type).name << ' ' << tkns.at(index + 1).val;
							scopes.at(scopes.size() - 1).variables.push_back({ type,0,tkns.at(index + 1).val });
						}
						index++;
					}
					break;
				case 9:
					if (!tkns.at(index + 2).type) return 1;
					{
						unsigned long long type = find_type(types, tkns.at(index).val);
						unsigned char ptr_type = 1;
						if (mode.get(2)) ptr_type = 3;
						else if (mode.get(0)) ptr_type = 2;
						if (type == types.size()) return 1;
						if (isMember) classes.at(classes.size() - 1).members.push_back({ type,1,tkns.at(index + 2).val });
						else {
							file << types.at(type).name << "* " << tkns.at(index + 2).val;
							scopes.at(scopes.size() - 1).variables.push_back({ type,1,tkns.at(index + 2).val });
						}
						index += 2;
					}
					break;
				case 12:
					if (!tkns.at(index + 2).type) return 1;

					break;
				default:
					break;
				}
				break;
			case 1:
				afterReturn = true;
				{
					unsigned count = 0;
					while (count < scopes.at(scopes.size() - 1).variables.size()) {
						if (scopes.at(scopes.size() - 1).variables.at(count).ptr_type) {
							file << "free(" << scopes.at(scopes.size() - 1).variables.at(count).name << ");\n";
							if (inClass) file << indent(scopes.size() - 2);
							else file << indent(scopes.size() - 1);
						}
						count++;
					}
				}
				file << "return ";
				break;
			case 4:
				file << ";\n";
				if ((tkns.at(index + 1).type == 39)&&(!(scopes.at(scopes.size()-1).variables.size()))) {
					if (inClass) file << indent(scopes.size() - 3);
					else file << indent(scopes.size() - 2);
				}
				else {
					if (inClass) file << indent(scopes.size() - 2);
					else file << indent(scopes.size() - 1);
				}
				break;
			case 38:
				file << "{\n";
				if (inClass) file << indent(scopes.size() - 1);
				else file << indent(scopes.size());
				scopes.push_back({});
				break;
			case 39:
				if (!afterReturn) {
					while (scopes.at(scopes.size() - 1).variables.size() > 0) {
						if (scopes.at(scopes.size() - 1).variables.at(scopes.at(scopes.size() - 1).variables.size() - 1).ptr_type) {
							file << "free(" << scopes.at(scopes.size() - 1).variables.at(scopes.at(scopes.size() - 1).variables.size() - 1).name << ");\n";
							if (scopes.at(scopes.size() - 1).variables.size() > 1) {
								if (inClass) file << indent(scopes.size() - 2);
								else file << indent(scopes.size() - 1);
							}
							else {
								if (inClass) file << indent(scopes.size() - 3);
								else file << indent(scopes.size() - 2);
							}
						}
						scopes.at(scopes.size() - 1).variables.pop_back();
					}
				}
				else afterReturn = false;
				scopes.pop_back();
				file << "}\n";
				if (inClass) file << indent(scopes.size() - 2);
				else file << indent(scopes.size() - 1);
				break;
			case 40:
				file << "int main(int argc,char* argv[])";
				inProgram = true;
				break;
			case 59:
				if (!(tkns.at(index + 1).type == 24)) return 1;
				index += 2;
				switch (tkns.at(index).type) {
				case 0:
					if (doth_variable_exist(tkns.at(index).val)) {
						switch (scopes.at(get_variable_scope(tkns.at(index).val)).variables.at(find_variable(scopes.at(get_variable_scope(tkns.at(index).val)).variables, tkns.at(index).val)).type) {
						case 1:
							if (scopes.at(get_variable_scope(tkns.at(index).val)).variables.at(find_variable(scopes.at(get_variable_scope(tkns.at(index).val)).variables, tkns.at(index).val)).ptr_type) file << "printf(\"%s\"," << tkns.at(index).val << ')';
							else file << "printf(\"%c\"," << tkns.at(index).val << ')';
							break;
						case 2:
						case 4:
						case 6:
						case 8:
						case 10:
							if (scopes.at(get_variable_scope(tkns.at(index).val)).variables.at(find_variable(scopes.at(get_variable_scope(tkns.at(index).val)).variables, tkns.at(index).val)).ptr_type) file << "printf(\"%p\"," << tkns.at(index).val << ')';
							else file << "printf(\"%lld\"," << tkns.at(index).val << ')';
							break;
						case 0:
						case 3:
						case 5:
						case 7:
						case 9:
							if (scopes.at(get_variable_scope(tkns.at(index).val)).variables.at(find_variable(scopes.at(get_variable_scope(tkns.at(index).val)).variables, tkns.at(index).val)).ptr_type) file << "printf(\"%p\"," << tkns.at(index).val << ')';
							else file << "printf(\"%llu\"," << tkns.at(index).val << ')';
							break;
						default:
							return 1;
						}
					}
					break;
				case 2:
					file << "printf(\"%lld\"," << tkns.at(index).val << ')';
					break;
				case 3:
					file << "printf(\"" << tkns.at(index).val << "\")";
					break;
				default:
					return 1;
				}
				break;
			case 69:
				if (afterImports) return 1;
				index++;
				if (tkns.at(index).type) return 1;
				if (!is_library_included(tkns.at(index).val)) read_bindings(tkns.at(index).val);
				index++;
				if (!(tkns.at(index).type == 4)) return 1;
			}
			index++;
		}
		return 0;
	}
	void compile_class_cpp(std::ofstream& file,unsigned class_index) {
		file << "\nstruct " << classes.at(class_index).name << '{';
		unsigned index = 0;
		while (index < classes.at(class_index).members.size()) {
			file << '\n' << indent(1) << types.at(classes.at(class_index).members.at(index).type).name;
			if (classes.at(class_index).members.at(index).ptr_type) file << '*';
			file << ' ' << classes.at(class_index).members.at(index++).name << ';';
		}
		index = 0;
		while (index < classes.at(class_index).methods.size()) {
			file << '\n' << indent(1) << types.at(classes.at(class_index).methods.at(index).Rtype).name;
			if (classes.at(class_index).methods.at(index).ptr_type) file << '*';
			file << ' ' << classes.at(class_index).methods.at(index).name;
			compile_arguments(file, classes.at(class_index).methods.at(index).args);
			file << ';';
			index++;
		}
		index = 0;
		while (index < classes.at(class_index).procedures.size()) {
			file << '\n' << indent(1) << "void " << classes.at(class_index).procedures.at(index).name;
			compile_arguments(file, classes.at(class_index).procedures.at(index).args);
			file << ';';
			index++;
		}
		file << "\n}";
	}
	void compile_struct(std::ofstream& file, unsigned struct_index) {
		file << "\nstruct " << structs.at(struct_index).name << '{';
		unsigned index = 0;
		while (index < structs.at(struct_index).members.size()) {
			file << '\n' << indent(1) << types.at(structs.at(struct_index).members.at(index).type).name;
			if (structs.at(struct_index).members.at(index).ptr_type) file << '*';
			file << ' ' << structs.at(struct_index).members.at(index++).name << ';';
		}
		file << "\n}";
	}
	void create_hpp(flags& values) {
		std::ofstream file;
		if (values.get(0)) file.open(project + ".hpp");
		else file.open("declarations.hpp");
		setup_cpp(values, file);
		unsigned index = 0;
		while (index < libs.size()) file << "\n#include <" + libs.at(index++).name << ".hpp>";
		index = 0;
		while (index < classes.size()) if(!is_class_in_library(classes.at(index++).name)) compile_class_cpp(file, index-1);
		index = 0;
		while (index < structs.size()) if (!is_struct_in_library(structs.at(index++).name)) compile_struct(file, index - 1);
		index = 0;
		while (index < functions.size()) {
			if (!is_function_in_library(functions.at(index).name)) {
				file << '\n' << types.at(functions.at(index).Rtype).name;
				if (functions.at(index).ptr_type) file << '*';
				file << ' ' << functions.at(index).name;
				compile_arguments(file, functions.at(index).args);
				file << ';';
			}
			index++;
		}
		index = 0;
		while (index < procedures.size()) {
			if (!is_procedure_in_library(procedures.at(index).name)) {
				file << "\nvoid " << procedures.at(index).name;
				compile_arguments(file, procedures.at(index).args);
				file << ';';
			}
			index++;
		}
	}
	void compile_class_c(std::ofstream& file, unsigned class_index) {
		file << "\ntypedef struct " << classes.at(class_index).name << "{";
		unsigned index = 0;
		while (index < classes.at(class_index).members.size()) {
			file << '\n' << indent(1) << types.at(classes.at(class_index).members.at(index).type).name;
			if (classes.at(class_index).members.at(index).ptr_type) file << '*';
			file << ' ' << classes.at(class_index).members.at(index).name << ";";
			index++;
		}
		file << "\n};";
		index = 0;
		while (index < classes.at(class_index).methods.size()) {
			file << '\n' << types.at(classes.at(class_index).methods.at(index).Rtype).name;
			if (classes.at(class_index).methods.at(index).ptr_type) file << '*';
			file << ' ' << classes.at(class_index).name << '_' << classes.at(class_index).methods.at(index).name;
			compile_method_arguments(file, classes.at(class_index).methods.at(index++).args, classes.at(class_index).name);
			file << ';';
		}
		index = 0;
		while (index < classes.at(class_index).procedures.size()) {
			file << "\nvoid " << classes.at(class_index).name << '_' << classes.at(class_index).procedures.at(index).name;
			compile_method_arguments(file, classes.at(class_index).procedures.at(index++).args, classes.at(class_index).name);
			file << ';';
		}
	}
	void create_h(flags& values) {
		std::ofstream file;
		if (values.get(0)) file.open(project + ".h");
		else file.open("declarations.h");
		setup_c(file);
		unsigned index = 0;
		while (index < libs.size()) file << "\n#include <" + libs.at(index++).name << ".h>";
		index = 0;
		while (index < classes.size()) if(!is_class_in_library(classes.at(index++).name)) compile_class_c(file, index-1);
		index = 0;
		while (index < structs.size()) if (!is_struct_in_library(structs.at(index++).name)) compile_struct(file, index - 1);
		index = 0;
		while (index < functions.size()) {
			if (!is_function_in_library(functions.at(index).name)) {
				file << '\n' << types.at(functions.at(index).Rtype).name;
				if (functions.at(index).ptr_type) file << '*';
				file << ' ' << functions.at(index).name;
				compile_arguments(file, functions.at(index).args);
				file << ';';
			}
			index++;
		}
		index = 0;
		while (index < procedures.size()) {
			if (!is_procedure_in_library(procedures.at(index).name)) {
				file << "\nvoid " << procedures.at(index).name;
				compile_arguments(file, procedures.at(index).args);
				file << ';';
			}
			index++;
		}
		file.close();
	}
	void generate_member_binding(std::ofstream& file, Variables& members) {
		unsigned index = 0;
		while (index < members.size()) {
			file << "\nmember " << types.at(members.at(index).type).name << ' ';
			if (members.at(index).ptr_type) file << "* ";
			file << members.at(index++).name;
		}
	}
	void generate_argument_binding(std::ofstream& file, Arguments args) {
		unsigned index = 0;
		while (index < args.size()) {
			file << ' ' << types.at(args.at(index).value_type).name<<' ';
			if (args.at(index).type_of_argument) file << "* ";
			file << args.at(index++).name;
		}
	}
	void generate_function_binding(std::ofstream& file,Function& func) {
		file << "\nfunction " << func.name << ' ' << types.at(func.Rtype).name;
		if (func.ptr_type) file << " *";
		generate_argument_binding(file, func.args);
	}
	void generate_procedure_binding(std::ofstream& file, Procedure& proc) {
		file << "\nprocedure " << proc.name;
		generate_argument_binding(file, proc.args);
	}
	void generate_class_binding(std::ofstream& file,Class& class_) {
		file << "\nclass " << class_.name;
		generate_member_binding(file, class_.members);
		unsigned index = 0;
		while (index < class_.methods.size()) generate_function_binding(file, class_.methods.at(index++));
		index = 0;
		while (index < class_.procedures.size()) generate_procedure_binding(file, class_.procedures.at(index++));
		file << "\nend";
	}
	void generate_open_class_binding(std::ofstream& file, Class& class_) {
		file << "\nopen_class " << class_.name;
		generate_member_binding(file, class_.members);
		unsigned index = 0;
		while (index < class_.methods.size()) generate_function_binding(file, class_.methods.at(index++));
		index = 0;
		while (index < class_.procedures.size()) generate_procedure_binding(file, class_.procedures.at(index++));
		file << "\nend";
	}
	void generate_struct_binding(std::ofstream& file, Struct& struct_) {
		file << "\nstruct " << struct_.name;
		generate_member_binding(file, struct_.members);
		file << "\nend";
	}
	void generate_bindings() {
		std::ofstream file(project+"_bindings.data");
		unsigned index = 0;
		while (index < libs.size()) {
			if (!index) file << '\n';
			file << "lib " << libs.at(index++).name;
		}
		index = 0;
		while (index < classes.size()) {
			if (!is_class_in_library(classes.at(index++).name)) {
				if (classes.at(index - 1).is_open) generate_open_class_binding(file, classes.at(index - 1));
				else generate_class_binding(file, classes.at(index - 1));
			}
		}
		index = 0;
		while (index < structs.size()) if (!is_struct_in_library(structs.at(index++).name)) generate_struct_binding(file, structs.at(index - 1));
		index = 0;
		while (index < functions.size()) if(!is_function_in_library(functions.at(index++).name)) generate_function_binding(file, functions.at(index-1));
		index = 0;
		while (index < procedures.size()) if(!is_procedure_in_library(procedures.at(index++).name)) generate_procedure_binding(file, procedures.at(index-1));
		file << '\n';
		file.close();
	}
};
template<class T> void put_in_vector(std::vector<T>& des, std::vector<T> loc) {
	unsigned index = 0;
	while (index < loc.size()) {
		des.push_back(loc.at(index));
		index++;
	}
}
template<class T> void put_in_vector(std::vector<T>& des, std::vector<T> loc, T addition) {
	unsigned index = 0;
	while (index < loc.size()) {
		des.push_back(addition + loc.at(index));
		index++;
	}
}
std::vector<std::string> get_files_in_path(std::filesystem::path path,std::error_code& what) {
	std::vector<std::string> out;
	std::filesystem::directory_iterator dirs{path};
	while (1) {
		if (dirs._At_end()) return out;
		dirs.increment(what);
		if ((*dirs).is_directory()) put_in_vector(out, get_files_in_path((*dirs).path(), what), std::filesystem::relative((*dirs).path(), path).string());
		else out.push_back((*dirs).path().filename().string());
	}
}
std::vector<std::string> get_files_in_path_with_ext(std::filesystem::path path, std::string ext, std::error_code& what) {
	std::vector<std::string> out;
	std::filesystem::directory_iterator dirs{path};
	while (1) {
		if (dirs._At_end()) return out;
		dirs.increment(what);
		if ((*dirs).is_directory()) put_in_vector(out, get_files_in_path_with_ext((*dirs).path(), ext, what), std::filesystem::relative((*dirs).path(), path).string());
		else if ((*dirs).path().extension() == ext) out.push_back((*dirs).path().stem().string());
	}
}
void compile(language_type lang, system_type os, flags& values,std::vector<std::string>& files,std::string name) {
	compiler stuff(name);
	unsigned index = 0;
	lexer lex("");
	while (index < files.size()) {
		lex.reset(load_file(files.at(index) + ".aur"));
		switch (lang) {
		case 0:
			try{
			if (stuff.compile_cpp(lex.tokenize(), files.at(index), values, os)) std::cout << "Failed to compile: " << files.at(index) + ".aur\n";
			} catch(...){
				std::cout<<"Failed to compile: "<<files.at(index)<<".aur due to exception\n";
			}
			break;
		case 1:
			try {
				if (stuff.compile_c(lex.tokenize(), files.at(index), values, os)) std::cout << "Failed to compile: " << files.at(index) + ".aur\n";
			}
			catch (...) {
				std::cout << "Failed to compile: " << files.at(index) << ".aur due to exception\n";
			}
			break;
		}
		index++;
	}
	switch (lang) {
	case 0:
		stuff.create_hpp(values);
		break;
	case 1:
		stuff.create_h(values);
		break;
	}
	if (values.get(0)) stuff.generate_bindings();
}
int main(int argc, char* argv[]) {
	language_type lang = 0;
	system_type operating = 0;
	std::string temp;
	std::vector<std::string> arguments;
	if (argc < 2) {
		std::cout << "No arguments supplied!\n";
		return 1;
	}
	load_flags(arguments, argv, argc);
	std::vector<std::string> files;
	flags values;
	unsigned long long count = 2;
	while (count < arguments.size()) {
		temp = arguments.at(count++);
		switch (loose_match(6, temp,char*, "-c", "-cpp", "-linux", "-windows","-lib","-app")) {
		case 0:
			lang = 1;
			break;
		case 1:
			lang = 0;
			break;
		case 2:
			operating = 1;
			break;
		case 3:
			operating = 0;
			break;
		case 4:
			values.store(0, 1);
			break;
		case 5:
			values.store(0, 0);
			break;
		default:
			files.push_back(temp);
		}
	}
	if (!files.size()) {
		std::cout << "No files supplied!\n";
		return 1;
	}
	compile(lang, operating, values, files, arguments.at(1));
}