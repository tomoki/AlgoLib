#include <iostream>
#include <string>
#include <cctype>
#include <sstream>

using namespace std;

typedef string::const_iterator Cursor;
class ParseError{};

// <四則演算の式> ::= <乗算除算の式> (+ or -) <乗算除算の式> (+ or -) ...
// <乗算除算の式> ::= <括弧か数> (* or /) <括弧か数> (* or /) ...
// <括弧か数>     ::= '(' <四則演算の式> ')' or <数>
// <数>           ::= (0|1|2|3|4|5|6|7|8|9)+

int expression(Cursor&);
int term(Cursor&);
int factor(Cursor&);
int number(Cursor&);

// <四則演算の式> ::= <乗算除算の式> (+ or -) <乗算除算の式> (+ or -) ...
int expression(Cursor &c){
    int ret = term(c);
    while(*c == '+' or *c == '-'){
        if(*c == '+'){
            c++;
            ret += term(c);
        }else{
            c++;
            ret -= term(c);
        }
    }
    return ret;
}

// <乗算除算の式> ::= <括弧か数> (* or /) <括弧か数> (* or /) ...
int term(Cursor &c){
    int ret = factor(c);
    while(*c == '*' or *c == '/'){
        if(*c == '*'){
            c++;
            ret *= factor(c);
        }else{
            c++;
            ret /= factor(c);
        }
    }
    return ret;
}

// <括弧か数>     ::= '(' <四則演算の式> ')' or <数>
int factor(Cursor &c){
    if(*c == '('){
        c++;
        int ret = expression(c);
        c++; // ')'
        return ret;
    }else{
        return number(c);
    }
}

// <数>           ::= (0|1|2|3|4|5|6|7|8|9)+
int number(Cursor &c){
    stringstream ss;
    while(isdigit(*c)){
        ss << *c;
        c++;
    }
    int ret;
    ss >> ret;
    return ret;
}

int main(){
    int test_case;
    cin >> test_case;
    cin.ignore();
    for(int i=0;i<test_case;i++){
        string s;
        getline(cin,s);
        Cursor c = s.begin();
        cout << expression(c) << endl;
    }
    return 0;
}
