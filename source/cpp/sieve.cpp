#include <iostream>
#include <vector>
using namespace std;

//上限より余裕を取ること。
vector<bool> sieve(const int M){
    vector<bool> isPrime(M);
    for(int i=2;i<M;i++) isPrime[i] = true;
    for(int i=2;i*i<M;i++){
        if(not isPrime[i]) continue;
        for(int j=i*i;j<M;j+=i){
            isPrime[j] = false;
        }
    }
    return isPrime;
}

// リストがほしいときはこっち
vector<int> sieve(const int M){
    vector<bool> isPrime(M);
    vector<int> primes;
    for(int i=2;i<M;i++) isPrime[i] = true;
    for(int i=2;i*i<M;i++){
        if(not isPrime[i]) continue;
        for(int j=i*i;j<M;j+=i){
            isPrime[j] = false;
        }
    }
    for(int i=2;i<M;i++){
        if(isPrime[i]) primes.push_back(i);
    }
    return primes;
}
