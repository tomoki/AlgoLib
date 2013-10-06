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

~~~~~~{.cpp}
#include <iostream>
#include <vector>
using namespace std;

//上限より余裕を取ること。

vector<bool> sieve(const int M){
    vector<bool> isPrime(M);
    for(int i=2;i<M;i++) isPrime[i] = true;
    for(int i=2;i*i < M;i++){
        if(not isPrime[i]) continue;
        for(int j=i*i;j<M;j+=i){
            isPrime[j] = false;
        }
    }
    return isPrime;
}
~~~~~~

~~~~~~{.py}
from math import *

def sieve(N):
    primes = set()
    for i in range(2,N):
        primes.add(i)

    for i in range(2,ceil(sqrt(N))):
        if i in primes:
            for j in range(i*i,N,i):
                primes.discard(j)

    return primes
~~~~~~

素数のリストが欲しかったら、適当に突っ込むこと。
実際には$O(n \log \log n)$だけれど、大体$O(n)$だと思っていい。

### 素因数分解

## コンビネーション
くみあわせ。

### コンビネーションの数

~~~~~~{.cpp}
#include <iostream>
#include <vector>

using namespace std;
typedef long long ll;

//いい感じのやつ (n=61まで大丈夫)
ll combi(int n,int r){
    if(n < r) return 0;
    r = min(r,n-r);
    ll ret = 1;
    for(int i=0;i<r;i++){
        ret *= n-i;
        ret /= i+1;
    }
    return ret;
}

//パスカルの三角形 (n=66まで大丈夫)
//たくさん必要になるときはこっちのほうがいい。
ll combi_tri(int n,int r){
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

// 重複を許してn個からk個を選ぶ。
// 未検証
ll multicombi(ll n,ll k){
    return combi(n+k-1,n-1);
}

~~~~~~

### 列挙

~~~~~~{.cpp}
int main(){
    int N,M;
    cin >> N >> M;

    vector<int> numbers(N);
    for(int i=0;i<N;i++) numbers[i] = i;

    stack<pair<int,vector<int> > > stack;
    stack.push(make_pair(0,vector<int>()));

    vector<vector<int> > combis;
    while(!stack.empty()){
        int lower = stack.top().first;
        vector<int> choose = stack.top().second;
        stack.pop();

        if(choose.size() == M){
            combis.push_back(choose);
            continue;
        }

        for(int i=lower;i<N-M+choose.size()+1;i++){
            vector<int> cop = choose;
            cop.push_back(numbers[i]);
            stack.push(make_pair(i+1,cop));
        }
    }

    cout << "size :" << combis.size() << endl;
    for(int i=0;i<combis.size();i++){
        for(int j=0;j<combis[i].size();j++){
            cout << combis[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}
~~~~~~

## 確率的なアレ
テスト中。

~~~~~~{.cpp}
// n< 341550071728321 , ok.
#include <vector>
#include <cmath>
using namespace std;

typedef long long ll;

// will not overflow even if mod mod is too big..
ll mod_mul(ll a,ll b,ll mod){
    if(b == 0) return 0;
    ll res = mod_mul((a+a)%mod,b/2,mod);
    if(b % 2 == 1) res = (res + a)%mod;
    return res;
}

// use mod_mul if mod is too big.
ll mod_pow(ll x,ll n,ll mod){
    if(n==0) return 1;
    ll ret = mod_pow(x * x % mod,n/2,mod);
    if(n%2== 1)  ret = ret * x % mod;
    return ret;
}

// return probably prime.
//  if n < 341550071728321 return correct answer.
bool miller_rabin(ll n){
    if(n == 2) return true;
    if(n % 2 == 0 or n <= 1) return false;
    ll s=0,d=n-1;
    static const ll a[] = {2,3,5,7,11,13,17,n+1};

    while(d % 2 == 0){
        s++;d/=2;
    }
    for(int i=0;a[i]<n;i++){
        ll x = mod_pow(a[i],d,n);
        if(x != 1){
            ll r;
            for(r=0;r<s;r++){
                if(x == n-1) break;
                x = mod_mul(x,x,n);
            }
            if(r == s) return false;
        }
    }
    return true;
}

ll gcd(ll a,ll b){
    return b==0 ? a : gcd(b,a%b);
}

inline ll random(ll x,ll c,ll m){
    return (mod_mul(x,x,m)+c)%m;
}

vector<bool> sieve(const int M){
    vector<bool> isPrime(M);
    for(int i=2;i<M;i++) isPrime[i] = true;
    for(int i=2;i*i < M;i++){
        if(not isPrime[i]) continue;
        for(int j=i*i;j<M;j+=i){
            isPrime[j] = false;
        }
    }
    return isPrime;
}

// primes must contatin 2.
vector<ll> pollard_rho(ll n,const vector<ll>& primes,bool precheck=true){
    if(n == 0 or n == 1) return vector<ll>(1,n);
    vector<ll> ret;
    static const int constants[] = {1,51,73,0};
    if(precheck){
        for(size_t i=0;i<primes.size();i++){
            while(n % primes[i] == 0){
                n /= primes[i];
                ret.push_back(primes[i]);
            }
            if(n == 1) return ret;
        }
    }
    if(miller_rabin(n)){
        ret.push_back(n);
        return ret;
    }
    for(int i=0;constants[i];i++){
        ll x = 2,y = 2,d = 1;
        while(d == 1){
            x = random(x,constants[i],n);
            y = random(random(y,constants[i],n),constants[i],n);
            d = gcd(abs(x-y),n);
        }
        if(d == n) continue;

        vector<ll> dp = pollard_rho(d,primes,false);
        vector<ll> ndp = pollard_rho(n/d,primes,false);
        for(size_t j=0;j<dp.size();j++){
            ret.push_back(dp[j]);
        }
        for(size_t j=0;j<ndp.size();j++){
            ret.push_back(ndp[j]);
        }
        return ret;
    }
    return ret;
}


int main(){
    int N = 100;
    vector<bool> isprime = sieve(5);
    vector<ll> primes;
    for(int i=0;i<isprime.size();i++){
        if(isprime[i]){
            primes.push_back(i);
        }
    }
    for(int i=0;i<N;i++){
        //cout << i << " " << miller_rabin(i) << " " << endl;
        vector<ll> ps = pollard_rho(i,primes,true);
        for(int j=0;j<ps.size();j++){
            cout << ps[j] << " ";
        }
        cout << endl;
    }
    return 0;
}
~~~~~~
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

## 基数変換

~~~~~~{.cpp}
vector<int> int_to_digits(int base,int N){
    vector<int> rev_ret;
    while(N != 0){
        rev_ret.push_back(N % base);
        N /= base;
    }
    reverse(all(rev_ret));
    return rev_ret;
}

int digits_to_int(int base,const vector<int>& digits){
    int ret = 0;
    for(int i=0;i<(int)digits.size();i++){
        ret *= base;
        ret += digits[i];
    }
    return ret;
}
~~~~~~
