# 整数論

## 最大公約数,最小公倍数

ユークリッドの互除法を使う。intをlong longに置換してもいい。 $O(\log n)$

### 最大公約数
~~~~~~{.cpp}
int gcd(int a,int b){
    return b==0 ? a : gcd(b,a%b);
}
~~~~~

### 最小公倍数
~~~~~~{.cpp}
int lcm(int a,int b){
    return a*b / gcd(a,b);
}
~~~~~~

## mod

long longに入らないような答えのときにmodが登場する。

### modの計算式について

$$\begin{aligned}
a \equiv c & \pmod m \\
b \equiv d & \pmod m\end{aligned}$$

の条件下では以下の式が成り立つ。

$$\begin{aligned}
a+b \equiv c+d  & \pmod m \\
a-b \equiv c-d  & \pmod m \\
a \times b \equiv c \times d  & \pmod m\end{aligned}$$

さらに、mが素数の場合、以下の関係が成り立つ。

$$\begin{aligned}
a ^ m \equiv a \pmod m \\
a ^ {m-1} \equiv 1 \pmod m \\
a ^ {m-2} \equiv \frac{1}{a} \pmod m\end{aligned}$$

つまり、$a$で割ることと、$a^{m-2}$を掛けることは同じである。\
これは、$C(10000,5000) \pmod p$といった式を計算する際、次の冪乗の演算と組みあわせて用いる。

### 冪乗のmod
いわゆるmod\_pow。計算量は$O(\log n)$。

~~~~~~{.cpp}
ll mod_pow(ll x,ll n,ll mod){
    if(n==0) return 1;
    ll ret = mod_pow(x * x % mod,n/2,mod);
    if(n%2== 1)  ret = ret * x % mod;
    return ret;
}
~~~~~~
ちなみにC++のpowを使うときに、引数が整数で、返り値も整数であることを期待
するときには、上記のpowを使うべき。なぜならC++のpowは
double,double->doubleな関数であるから。

## 素数

### エラトステネスの篩

素数のリストが欲しかったら、適当に突っ込むこと。
実際には$O(n \log \log n)$だけれど、大体$O(n)$だと思っていい。

## コンビネーション
くみあわせ。

~~~~~~{.cpp}
#include <iostream>
#include <vector>

using namespace std;
typedef long long ll;

//いい感じのやつ (n=61まで大丈夫)
ll combi2(int n,int r){
    if(n < r) return 0;
    ll ret = 1;
    for(int i=0;i<r;i++){
        ret *= n-i;
        ret /= i+1;
    }
    return ret;
}

ll mod_combi(ll n,ll r,ll mod){
    if(n < r) return 0;
    ll ret = 1;
    for(int i=0;i<r;i++){
        ret = (ret * (n-i)) % mod;
        ret = (ret * mod_pow(i+1,mod-2,mod)) % mod;
    }
    return ret;
}

//パスカルの三角形 (n=66まで大丈夫)
//たくさん必要になるときはこっちのほうがいい。
ll combi3(int n,int r){
    int N = n+1;
    vector<vector<ll> > memo(N,vector<ll>(N,0));
    memo[0][0] = 1;
    for(int i=1;i<N;i++){
        memo[i][0] = memo[i-1][0];
        for(int j=1;j<N;j++){
            memo[i][j] = memo[i-1][j-1] + memo[i-1][j];
        }
    }
    return memo[n][r];
}
~~~~~~

### 素因数分解

## ヨセフス数


## 乱数

XORShiftをつかったらうれしいかもしれない。

~~~~~~{.cpp}
unsigned long xor128(){
    static unsigned long x=123456789,y=362436069,z=521288629,w=88675123;
    unsigned long t;
    t=(x^(x<<11));x=y;y=z;z=w; return( w=(w^(w>>19))^(t^(t>>8)) );
}
~~~~~~
