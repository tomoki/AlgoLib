#include <iostream>
// 素数のmodの体
// mod_powとかダサいのでなんとかする．
// 名前は後でかんがえる．
// templateでなんとかできるのかな
//  MOD<P>みたいに．できないきがする．
//  bitsetと同じでPはコンパイル時にしか決められない？
// そうすると入力でとれない．
// mod should be prime.
struct MOD{
    long long m;
    long long v;
    MOD(long long v_,long long p_)
        :m(p_),v(v_%m){
    }
};

std::ostream& operator<<(std::ostream& os,MOD v){
    return os << v.v;
}
MOD operator+(const MOD& l,const MOD& r){
    return MOD(l.v+r.v,l.m);
}
MOD operator+(const MOD& l,const long long& r){
    return l + MOD(r,l.m);
}
MOD operator+(const long long& l,const MOD& r){
    return r + l;
}
MOD operator-(const MOD& l,const MOD& r){
    long long mod = l.m;
    return MOD(l.v+(-r.v%mod)+mod,mod);
}
MOD operator-(const MOD& l,const long long& r){
    return l - MOD(r,l.m);
}
MOD operator-(const long long& l,const MOD& r){
    return MOD(l,r.m) - r;
}
MOD operator*(const MOD& l,const MOD& r){
    return MOD(l.v*r.v,l.m);
}
MOD operator*(const MOD& l,const long long& r){
    return l * MOD(r,l.m);
}
MOD operator*(const long long& l,const MOD& r){
    return r * l;
}
// long long n?
MOD pow(const MOD& x,long long n){
    long long mod = x.m;
    if(n == 0) return MOD(1,mod);
    MOD ret = pow(x*x,n/2);
    if(n%2 == 1) ret = ret * x;
    return ret;
}
// a^p = a
MOD operator/(const MOD& l,const MOD& r){
    long long mod = l.m;
    return l * pow(r,mod-2);
}
MOD operator/(const MOD& l,const long long& r){
    // ????
    return l / MOD(r,l.m);
}
MOD operator/(const long long& l,const MOD& r){
    return MOD(l,r.m) / r;
}

int main(){
    const long long p = 2;
    MOD hoge(1,p);
    MOD a = hoge + 1;
    std::cout << a << std::endl;
    return 0;
}
