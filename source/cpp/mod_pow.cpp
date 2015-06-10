template<typename T,typename F>
T apply_doubling(const T& x,
                 const long long& n,
                 const T& id_elem,
                 const F& binary_op){
    if(n == 0) return id_elem;
    T ret = apply_doubling(binary_op(x,x),n/2,id_elem,binary_op);
    if(n % 2 == 1) ret = binary_op(ret,x);
    return ret;
}

long long mod_pow(long long x,long long n,long long mod){
    return apply_doubling(x,n,1ll,[mod](ll a,ll b){return a*b%mod;});
}
