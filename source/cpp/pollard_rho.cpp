// n< 341550071728321 , ok.
#include <iostream>
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
