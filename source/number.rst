######################
整数論
######################

****************************************
最大公約数,最小公倍数
****************************************

ユークリッドの互除法を使う。intをlong longに置換してもいい。 O(logn)

最大公約数
====================

.. code-block:: cpp

    int gcd(int a,int b){
        return b==0 ? a : gcd(b,a%b);
    }

最小公倍数
====================
.. code-block:: cpp

    int lcm(int a,int b){
        return a*b / gcd(a,b);
    }

****************************************
mod
****************************************

long long に入らないような答えのときに mod が登場する。

modの計算式について
====================

.. math::
    \begin{aligned}
    a \equiv c & \pmod m \\
    b \equiv d & \pmod m\end{aligned}

の条件下では以下の式が成り立つ。

.. math::
    \begin{aligned}
    a+b \equiv c+d  & \pmod m \\
    a-b \equiv c-d  & \pmod m \\
    a \times b \equiv c \times d  & \pmod m\end{aligned}

さらに、mが素数の場合、以下の関係が成り立つ。

.. math::
    \begin{aligned}
    a ^ m \equiv a \pmod m \\
    a ^ {m-1} \equiv 1 \pmod m \\
    a ^ {m-2} \equiv \frac{1}{a} \pmod m\end{aligned}

つまり、 :math:`a` で割ることと、:math:`a^{m-2}` を掛けることは同じである。
これは、 :math:`C(10000,5000) \pmod p` といった式を計算する際、次の冪乗の演算と組みあわせて用いる。

冪乗のmod
====================
いわゆるmod\_pow。計算量は :math:`O(\log n)`。

.. literalinclude:: cpp/mod_pow.cpp

ちなみにC++のpowを使うときに、引数が整数で、返り値も整数であることを期待
するときには、上記のpowを使うべき。なぜならC++のpowは
double,double->doubleな関数であるから。

****************************************
素数
****************************************

エラトステネスの篩
====================

.. literalinclude:: cpp/sieve.cpp

.. literalinclude:: python/sieve.py

素数のリストが欲しかったら、適当に突っ込むこと。
実際には :math:`O(n \log \log n)` だけれど、大体 :math:`O(n)` だと思っていい。

素因数分解
====================

.. literalinclude:: cpp/prime_decomposition.cpp

****************************************
コンビネーション
****************************************
くみあわせ。

.. literalinclude:: cpp/combi.cpp

****************************************
確率的なアレ
****************************************
テスト中。

.. literalinclude:: cpp/pollard_rho.cpp

****************************************
乱数
****************************************

XORShiftをつかったらうれしいかもしれない。

.. code-block:: cpp

    unsigned long xor128(){
        static unsigned long x=123456789,y=362436069,z=521288629,w=88675123;
        unsigned long t;
        t=(x^(x<<11));x=y;y=z;z=w; return( w=(w^(w>>19))^(t^(t>>8)) );
    }

****************************************
基数変換
****************************************

.. literalinclude:: cpp/digit_conv.cpp

****************************************
二次方程式
****************************************

.. literalinclude:: cpp/solve_quadratic_equation.cpp

****************************************
連立方程式
****************************************
テスト中

.. literalinclude:: cpp/gauss_jordan.cpp

****************************************
nと互いに素な数の個数
****************************************
EulerのTotient関数による。

.. literalinclude:: cpp/euler_totient.cpp
