// primes is primes under sqrt(N).
vector<int> prime_decomposition(int N,const vector<int>& primes){
    vector<int> ret;
    for(int p : primes){
        while(N % p == 0){
            N /= p;
            ret.push_back(p);
        }
    }
    if(N != 1) ret.push_back(N);
    return ret;
}
