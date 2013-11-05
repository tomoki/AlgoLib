# 文字列操作

stringをincludeする。cctypeもいるかも。

## std::string
charをラップしたテンプレートクラス。 基本的な使い方について

### 部分列
~~~~~~{.cpp}
          //0123456789
string str("abcdefghij");
// 5番目以降
str.substr(5);    // "fghij"
// 5番目から3つ
str.substr(5,3); // "fgh"
//全部小文字にする
transform(s.begin(),s.end(),s.begin(),::tolower);
~~~~~~

substrは一つの引数でそこから先全部、二つの場合は第一引数の位置から、第二
引数の数だけ持ってくる。

### 検索

stringには、いくつかのfindが定義されている。線形検索なので、早い検索が必
要なときには後述するKMP法やBM法を用いること。

-   find 引数が最初に現れる位置

-   rfind 引数が最後に表われる位置

-   find_first_of 引数の文字のどれかが最初に表われる位置

-   find_last_of 引数の文字のどれかが最後に表われる位置

-   find_first_not_of
    引数の文字のどれかではない文字が最初に表われる位置

-   find_first_not_of
    引数の文字のどれかではない文字が最後に表われる位置

第二引数として探すための最初の位置を指定できる。

#### Boyer Moore法


#### KMP法


## stringstream

cinやcoutのようなstreamをstringを元に作成したりする。基本的には、string
をフォーマットしたり、intやlongに、intやlongから変換するために使用する。

~~~~~~{.cpp}
stringstream ss;
ss << 102;
string s;
ss >> s;
~~~~~~

## 再帰下降構文解析
BNFを書いて、それにしたがっていく。左再帰除去を忘れずに。

~~~~~~{.cpp}
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
~~~~~~
