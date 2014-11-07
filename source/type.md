# 算術型

## int
基本中の基本。$10^{9}$くらい。こわいときにはlong longを使うことを推奨。

## long long
大きい整数。$10^{18}$?くらい。

### ビット演算
ビットDPとかに使う。

~~~~~~{.cpp}
// 下から2番目のビットを立てる。1llと書くことに注意
x |= (1ll << 1);

// 集合演算
// 和
ll z = x | y;
// 積
ll z = x & y;
// xor
ll z = x ^ y;
// ループ
for(int i=0;i<N;i++){
    // i番目が立っていれば
    if(x & (1ll << i)){
        //something.
    }
}

ll dp[1 << N][N];
dp[0][0] = 1;
// Bitのほうが先!
for(int i=0;i<(1<<N);i++){
    for(int j=0;j<N;j++){
        //something.
    }
}

~~~~~~

#### popcount
~~~~~~{include="tomoki-snippets/c++-mode/popcount.cpp" .yasnippet .cpp}
~~~~~~

## double
floatは使ってはだめ。

## char
-128 ~ 127くらい。ちいさい。基本的には文字を入れるのに使う。vector\<char\>を
vector\<bool\>の代わりに使ってもいい。

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
true(==1)とかfalse(==0)を入れるためだけに使う。ただしvector\<bool\>は使ってはいけない。


## 補助関数
上記の型に関する便利な関数。


床関数、天井関数、および四捨五入。返り値はdouble。cmathをincludeする。

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
