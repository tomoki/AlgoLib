# 整数論

## 最大公約数,最小公倍数

ユークリッドの互除法を使う。intをlong longに置換してもいい。 $O(\log n)$

### 最大公約数

~~~~~~{.cpp}
int gcd(int a,int b){
    return b==0 ? a : gcd(b,a%b);
}
~~~~~~

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

~~~~~~{include="cpp/mod_pow.cpp" .cpp}
~~~~~~

ちなみにC++のpowを使うときに、引数が整数で、返り値も整数であることを期待
するときには、上記のpowを使うべき。なぜならC++のpowは
double,double->doubleな関数であるから。

## 素数

### エラトステネスの篩

~~~~~~{include="cpp/sieve.cpp" .cpp}
~~~~~~

~~~~~~{include="python/sieve.py" .py}
~~~~~~

素数のリストが欲しかったら、適当に突っ込むこと。
実際には$O(n \log \log n)$だけれど、大体$O(n)$だと思っていい。

### 素因数分解

~~~~~~{include="cpp/prime_decomposition.cpp" .cpp}

~~~~~~
## コンビネーション
くみあわせ。

~~~~~~{include="cpp/combi.cpp" .cpp}
~~~~~~

## 確率的なアレ
テスト中。

~~~~~~{include="cpp/pollard_rho.cpp" .cpp}
~~~~~~

## 乱数

XORShiftをつかったらうれしいかもしれない。

~~~~~~{.cpp}
unsigned long xor128(){
    static unsigned long x=123456789,y=362436069,z=521288629,w=88675123;
    unsigned long t;
    t=(x^(x<<11));x=y;y=z;z=w; return( w=(w^(w>>19))^(t^(t>>8)) );
}
~~~~~~

## 基数変換

~~~~~~{include="cpp/digit_conv.cpp" .cpp}
~~~~~~

## 二次方程式

~~~~~~{include="cpp/solve_quadratic_equation.cpp" .cpp}
~~~~~~

## 連立方程式
テスト中

~~~~~~{include="cpp/gauss_jordan.cpp" .cpp}
~~~~~~

## nと互いに素な数の個数
EulerのTotient関数による。

~~~~~~{include="cpp/euler_totient.cpp" .cpp}
~~~~~~
