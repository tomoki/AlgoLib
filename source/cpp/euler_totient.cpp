// nと互いに素な数の個数
// n (1-1/p1) ... (1-1/pn)
template<typename T>
T totient(T n){
    if(n == 0) return 0;
    T ans = n;
    for(T x=2;x*x <= n;x++){
        if(n % x == 0){
            ans -= ans/x;
            while(n % x == 0) n /= x;
        }
    }
    if(n > 1){
        ans -= ans/n;
    }
    return ans;
}
