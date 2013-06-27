# 行列

## 基本要素
正方行列用 //いつかなおす。

~~~~~~{.cpp}
// 適宜intにしたりすること。
typedef vector<vector<ll> > ll_mat;
~~~~~~

## 基本演算

かけ算とmod。たしざんはcoming soon.

~~~~~~{.cpp}
namespace std{
    ll_mat operator*(const ll_mat& lhs,const ll_mat& rhs){
        int N = lhs.size();
        ll_mat ret(N,vector<ll>(N));
        for(int row=0;row<N;row++){
            for(int col=0;col<N;col++){
                for(int k=0;k<N;k++){
                    ret[row][col] += rhs[row][k] * lhs[k][col];
                }
            }
        }
        return ret;
    }

    ll_mat operator%(ll_mat lhs,ll rhs){
        int N = lhs.size();
        ll_mat ret = lhs;
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                ret[i][j] = ret[i][j] % rhs;
            }
        }
        return ret;
    }
};
~~~~~~

## 基本操作

### 累乗

~~~~~~{.cpp}
vector<vector<ll> >  mod_pow(vector<vector<ll> > x,ll n,ll mod){
    if(n==0){
        vector<vector<ll> > E(x.size(),vector<ll>(x.size()));
        for(int i=0;i<x.size();i++){
            E[i][i] = 1;
        }
        return E;
    }
    vector<vector<ll> > ret = mod_pow(x * x % mod,n/2,mod);
    if(n%2 == 1)  ret = ret * x % mod;
    return ret;
}
~~~~~~

### 表示

~~~~~~{.cpp}
void display_matrix(vector<vector<ll> > mat){
    for(int i=0;i<mat.size();i++){
        for(int j=0;j<mat[0].size();j++){
            cerr << mat[i][j] << " ";
        }
        cerr << endl;
    }
}
~~~~~~

### ベクトルとのかけ算
一次元列ベクトルとのかけ算

~~~~~~{.cpp}
vector<ll> mat_multi(vector<vector<ll> > lhs,vector<ll> rhs,int mod){
    vector<ll> ret(rhs.size());
    int N = lhs.size();
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            ret[i] = (ret[i] + lhs[i][j] * rhs[j]) % mod;;
        }
    }
    return ret;
}
~~~~~~

## テスト
AOJ 1327
