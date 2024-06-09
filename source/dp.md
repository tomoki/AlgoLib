# 動的計画法およびそれに似たやつら。(TODO)
## LCS
Longest common sequence.O(NM)

~~~~~~{.cpp}
string lcs(const string& a,const string& b){
    const int n = a.size(),m = b.size();
    vector<vector<int> > x(n+1,vector<int>(m+1));
    vector<vector<int> > y(n+1,vector<int>(m+1));

    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            if(a[i] == b[j]){
                x[i+1][j+1] = x[i][j]+1;
                y[i+1][j+1] = 0;
            }else if(x[i+1][j] < x[i][j+1]){
                x[i+1][j+1] = x[i][j+1];
                y[i+1][j+1] = 1;
            }else{
                x[i+1][j+1] = x[i+1][j];
                y[i+1][j+1] = -1;
            }
        }
    }
    string ret;
    for(int i=n,j=m;i>0 and j>0;){
        if(y[i][j] > 0){
            i--;
        }else if(y[i][j] < 0){
            j--;
        }else{
            ret.push_back(a[i-1]);
            i--;j--;
        }
    }
    reverse(all(ret));
    return ret;
}
~~~~~~

## LIS

### O(NlogN)

~~~~~~{.cpp}
// O(Nlog(N))
//  Sphagetthi source より.
vector<int> longest_increasing_subsequence(const vector<int>& a){
    const int n = a.size();
    vector<int> A(n,1<<30);
    vector<int> id(n);
    for(int i=0;i<n;i++){
        id[i] = distance(A.begin(),lower_bound(A.begin(),A.end(),a[i]));
        A[id[i]] = a[i];
    }
    int m = *max_element(id.begin(),id.end());
    vector<int> b(m+1);
    for(int i=n-1;i>=0;i--){
        if(id[i] == m) b[m--] = a[i];
    }
    return b;
}
~~~~~~

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
    repeat(i,N){
        cin >> v[i].value >> v[i].weight;
    }
    // もしも複数入れて良いなら、直接valueを更新する。
    vi value(W+1);
    repeat(n,N){
        vi tmp(W+1);
        repeat(i,W+1){
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
    repeat(i,len(value)){
        if(value[i] > retv){
            retv = value[i];
            retw = i;
        }
    }
    cout << retv << endl << retw << endl;
    return 0;
}
~~~~~~
## ヒストグラム内の最大長方形のサイズ

~~~~~~{include="cpp/hist.cpp" .cpp}
~~~~~~

## なんか長方形の和をもとめるやつ

~~~~~~{include="cpp/2d_area.cpp" .cpp}
~~~~~~
