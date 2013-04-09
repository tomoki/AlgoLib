# 算術型

## int
基本中の基本。$10^{9}$くらい。こわいときにはlong longを使うことを推奨。

## long long
大きい整数。$10^{18}$?くらい。

## double
floatは使ってはだめ。絶対値を得たいときには、complexの中にある、fabsを使う。

## char
-128 ~ 128くらい。ちいさい。基本的には文字を入れるのに使う。vector<char>を
vector<bool>の代わりに使ってもいい。

~~~~~~{.cpp}
char c = 'a';
//cctypeが必要。大文字に変換する。すでに大文字のときは何も起こらない。
c = toupper(c);
//小文字に変換する。
c = tolower(c);
//vector<bool>の代わり。
vector<char> used(10,false);
~~~~~~

## bool
true(==1)とかfalse(==0)を入れるためだけに使う。ただしvector<bool>は使ってはいけない。


## 補助関数
上記の型に関する便利な関数。


床関数、天井関数、および四捨五入。返り値はint。cmathをincludeする。

~~~~~~{.cpp}
double x = 0.3;
int f = floor(x); // -> 0
int c = ceil(x); // -> 1
int r = round(x) // -> 0
~~~~~~

任意の場所で四捨五入したいときには、$10^{n}$をかけて、roundした後に、
$10^{n}$で割ればいい。

~~~~~~{.cpp}
double x = 0.123456789;
//0.123
double r = round(x*1000) / 1000.0;
~~~~~~
