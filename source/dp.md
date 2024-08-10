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

# 累積和

## 一次元
~~~~~~
// 累積和を計算する
// - ret[k] = 0 <= i < k を満たす v[i] の和
// - ret[a] - ret[b] = b <= i < a を満たす v[i] の部分和
// ret[0] = init
// ret[1] = v[0] + init
// ret[2] = v[1] + (v[0] + init)
// ruisekiwa({1, 2, 3}) = {0, 1, 3, 6}
template<typename T, typename R = T>
std::vector<R> ruisekiwa(const std::vector<T>& v, std::function<R(const T&, const R&)> op = std::plus<>(), R init = R {})
{
    std::vector<R> ret;
    ret.reserve(v.size() + 1);
    ret.push_back(init);
    for (size_t i = 1; i <= v.size(); i++) {
        ret.push_back(op(v[i-1], ret.back()));
    }
    return ret;
}
// 累積和へのアクセス
// x = [l, r) を満たす v[x] の和を返す
template<typename R>
R bubunwa(const std::vector<R>& v, int r, int l = 0)
{
    assert(r >= l);
    return v[r] - v[l];
}
~~~~~~

## 二次元

~~~~~~
// 2 次元の累積和
// - ret[a][b] = 0 <= y < a && 0 <= b < b を満たす v[x][y] の和
//  (bubunwa2 参照)
// ref: https://atcoder.jp/contests/abc366/submissions/56581627, 鉄則本 p60
template<typename T>
std::vector<std::vector<T>> ruisekiwa2(const std::vector<std::vector<T>>& v, std::function<T(const T&, const T&)> op = std::plus<>(), T init = T {})
{
#if DEBUG
    assert(v.size() > 0);
    size_t min_col = v[0].size(), max_col = v[0].size();
    for (const auto& c : v) {
        min_col = min(min_col, c.size());
        max_col = max(max_col, c.size());
    }
    assert(min_col == max_col && min_col > 0);
#endif
    size_t row = v.size();
    size_t col = v[0].size();
    std::vector<std::vector<T>> ret(row + 1, std::vector<T>(col + 1, init));
    // 横の累積和
    for (size_t r = 1; r <= row; r++)
        for (size_t c = 1; c <= col; c++)
            ret[r][c] = ret[r][c-1] + v[r-1][c-1];

    // 縦の累積和
    for (size_t r = 1; r <= row; r++)
        for (size_t c = 1; c <= col; c++)
            ret[r][c] += ret[r-1][c];

    return ret;
}

// 累積和へのアクセス
// y = [fy, ty) かつ y = [fx, tx) を満たす v[y][x] の和を返す
template<typename R>
R bubunwa2(const std::vector<std::vector<R>>& v, int ty, int tx, int fy, int fx)
{
    assert(tx >= fx);
    assert(ty >= fy);
    return v[ty][tx] - v[fy][tx] - v[ty][fx] + v[fy][fx];
}
~~~~~~