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

## Gauss-Jordan
GF上で連立方程式を解く

~~~~~~{.cpp}
// Gauss-Jordan. Solve equation on GF.
int invert(int x){
    int ret[2] = {0,1};
    return ret[x];
}

int modulo(int x){
    x %= 2;
    while(x < 0){
        x += 2;
    }
    return x;
}

const int none = 0; // if no answer
const int one = 1;  // if there is exactly one answer
const int many = 2; // many answer.
// answer will be inserted in b.
int gauss(matrix A,vector<int>& b){
    int n = A.size();
    int m = A[0].size();
    int pi = 0,pj = 0;
    while(pi < n and pj < m){
        for(int i=pi+1;i<n;i++){ // pivot
            if(abs(A[i][pj]) > abs(A[pi][pj])){
                swap(A[i],A[pi]);
                swap(b[i],b[pi]);
            }
        }
        if(abs(A[pi][pj]) > 0){
            int d = invert(A[pi][pj]);
            for(int j=0;j<m;j++){
                A[pi][j] = modulo(A[pi][j] * d);
            }
            b[pi] = modulo(b[pi]*d);
            for(int i=pi+1;i<n;i++){
                int k = A[i][pj];
                for(int j=0;j<m;j++){
                    A[i][j] = modulo(A[i][j] - k * A[pi][j]);
                }
                b[i] = modulo(b[i] - k*b[pi]);
            }
            pi++;
        }
        pj++;
    }
    for(int i=pi;i<n;i++){
        if(abs(b[i]) > 0){
            return none;
        }
    }
    if(pi < m or pj < m){
        return many;
    }
    for(int j=m-1;j>=0;j--){
        for(int i=0;i<j;i++){
            b[i] = modulo(b[i] - b[j] * A[i][j]);
        }
    }
    return one;
}
~~~~~~
