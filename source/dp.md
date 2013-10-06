# 動的計画法およびそれに似たやつら。(TODO)
## LCS
Longest common sequence.

## LIS
Longest increasing subsequence.

## 巡回セールスマン問題
bit演算をする。bitのループを先に回すこと。$O(N^2\times2^{N})$

~~~~~~{.cpp}
ll dp[1 << N][N];
dp[0][0] = 1;
// Bitのほうが先!
for(int i=0;i<(1<<N);i++){
    for(int j=0;j<N;j++){
        //something.
    }
}
~~~~~~~

## ナップサック問題
個数制限なしのとき

~~~~~~{.cpp}
#include <iostream>
#include <vector>

using namespace std;

#define rep(i,n) for(int i=0;i<(int)n;i++)

typedef vector<int> vi;

template<class T>
inline int len(const T& t){
    return t.size();
}

struct Treasure{
    int value,weight;
    Treasure():
        value(0),weight(0) {};
    Treasure(int _value,int _weight)
        : value(_value),weight(_weight) {}
};

int main(){
    int W,N;
    cin >> W;
    cin >> N;
    vector<Treasure> v(N);
    rep(i,N){
        cin >> v[i].value >> v[i].weight;
    }
    // もしも複数入れて良いなら、直接valueを更新する。
    vi value(W+1);
    rep(n,N){
        vi tmp(W+1);
        rep(i,W+1){
            tmp[i] = max(tmp[i],value[i]);
            int in = i+v[n].weight;
            if(in <= W){
                tmp[in] = max(tmp[in],value[i]+v[n].value);
            }
        }
        value.swap(tmp);
    }

    int retw=0;
    int retv=0;
    rep(i,len(value)){
        if(value[i] > retv){
            retv = value[i];
            retw = i;
        }
    }
    cout << retv << endl << retw << endl;
    return 0;
}
~~~~~~
