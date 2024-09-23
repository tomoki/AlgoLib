#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long ll;

//いい感じのやつ (n=61まで大丈夫)
//  modにしても良い。O(N)
ll nCr(int n,int r){
    if(n < r) return 0;
    r = min(r,n-r);
    ll ret = 1;
    for(int i=0;i<r;i++){
        ret *= n-i;
        ret /= i+1;
    }
    return ret;
}


// http://www.ravco.jp/cat/view.php?cat_id=6179
// 区別するn 個のものから，繰り返し用いることを許して，r 個取り出して作った組
// <-> 区別しないr 個のボールを，区別するn 個の箱に配る場合の総数
ll nHr(int n, int r){
    return nCr(r+n-1,r);
}

//パスカルの三角形 (n=66までオーバーフローしない)
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

// testing...
template <typename BidirectionalIterator>
bool next_combination(BidirectionalIterator first1,
                      BidirectionalIterator last1,
                      BidirectionalIterator first2,
                      BidirectionalIterator last2){
    if((first1 == last1) or (first2 == last2)){
        return false;
    }
    BidirectionalIterator m1 = last1;
    BidirectionalIterator m2 = last2;
    --m2;
    while(--m1 != first1 && not (*m1 < *m2)){
    }
    bool result = (m1 == first1) and not (*first1 < *m2);
    if(not result){
        while(first2 != m2 and not(*m1 < *first2)){
            ++first2;
        }
        first1 = m1;
        iter_swap(first1,first2);
        ++first1;++first2;
    }
    if((first1 != last1) and (first2 != last2)){
        m1 = last1; m2 = first2;
        while((m1 != first1) and (m2 != last2)){
            iter_swap(--m1,m2);
            ++m2;
        }
        reverse(first1,m1);
        reverse(first1,last1);
        reverse(m2,last2);
        reverse(first2,last2);
    }
    return not result;
}

template<typename BidirectionalIterator>
bool next_combination(BidirectionalIterator first,
                      BidirectionalIterator middle,
                      BidirectionalIterator last){
    return next_combination(first,middle,middle,last);
}

template<class BidirectionalIterator>
inline bool prev_combination(BidirectionalIterator first,
                             BidirectionalIterator middle,
                             BidirectionalIterator last){
  return next_combination(middle,last,first,middle);
}

template<typename T>
ostream& operator<<(ostream& os,const vector<T>& vec){
    os << "[";
    for(const auto& v : vec){
        os << v << ",";
    }
    os << "]";
    return os;
}

typedef unsigned long long ull;

void print_bit(ull S, int n=64){
    for(int i=n-1; i>=0; i--){
        if(S>>i & 1) std::cout << 1;
        else std::cout << 0;
    }
    std::cout << std::endl;
}

void subset_combination(int n, int k){
    ull S = (1ULL << k) - 1ULL;
    ull E = ~((1ULL << n) - 1ULL);
    while(!(S & E)){
        print_bit(S, n);
        ull smallest = S & -S;
        ull ripple = S + smallest;
        ull nsmallest = ripple & -ripple;
        S = ripple | (((nsmallest / smallest) >> 1) - 1);
    }
}


int main(){
    // poyoから2つえらぶ
    vector<int> poyo = {1,2,3,4,5};
    int N = poyo.size(),R = 3;
    do{
        vector<int> c(poyo.begin(),poyo.begin()+R);
        cerr << c  << endl;
    }while(next_combination(poyo.begin(),poyo.begin()+R,poyo.end()));

    return 0;
}

// mod 上の　nCr が大量に必要になるときは n!, 1/r!, 1/(n-r)! を先に求めておく
// https://atcoder.jp/contests/abc042/submissions/57187098
int main() {
    vector<md> exp(n+1);
    exp[0] = 1;
    for (int i = 1; i < static_cast<int>(exp.size()); i++) {
        exp[i] = exp[i-1] * i;
    }
    vector<md> dexp(exp.size());
    for (int i = 0; i < static_cast<int>(exp.size()); i++) {
        dexp[i] = 1 / exp[i];
    }
    // n 個から r 個を選ぶ組み合わせ数、 r の方が大きい時は 0
    auto nCr = [&exp, &dexp](ll n, ll r) -> md {
        if (n < r) return 0;
        assert(0 <= n && n < static_cast<ll>(exp.size()));
        assert(0 <= r && r < static_cast<ll>(exp.size()));
        return exp[n] * dexp[r] * dexp[n-r];
    };
    // 区別するn 個のものから，繰り返し用いることを許して，r 個取り出して作った組
    // <-> 区別しないr 個のボールを，区別するn 個の箱に配る場合の総数
    auto nHr = [&nCr](ll n, ll r) -> md {
        return nCr(r+n-1, r);
    };
}
