# データ構造

## Union-Find

~~~~~~{.cpp}
struct UnionFind{
    vector<int> par; // 親
    vector<int> rank; // 木の深さ
    UnionFind(int n){
        rep(i,n) par.push_back(i);
        rank = vector<int>(n);
    }
    // 親を探す
    int root(int x){
        if(par[x] == x){
            return x;
        }else{
            // 縮約
            return par[x] = root(par[x]);
        }
    }
    // x,yの含まれる集合を併合
    void unite(int x,int y){
        x = root(x);
        y = root(y);
        if(x==y) return;
        if(rank[x] < rank[y]){
            par[x] = y;
        }else{
            par[y] = x;
            if(rank[x] == rank[y]) rank[x]++;
        }
    }
    // 同じ集合にいるかどうか
    bool same(int x,int y){
        return root(x) == root(y);
    }
};
~~~~~~

## ヒープ

~~~~~~{.cpp}
#include <queue>
#include <iostream>

using namespace std;

typedef pair<int,int> pii;

struct Comp{
    bool operator()(pii left,pii right){
        if(left.second < right.second) return true;
        else if(left.second == right.second and left.first > right.first) return true;
        else return false;
    };
};

int main(){
    // 何も書かないと降順。(おっきい方からでてくる。)
    // これは降順にしたもの。
    priority_queue<int,vector<int>,greater<int> > Qi;
    //関数オブジェクトを使っていい感じにもできる。
    priority_queue<pii,vector<pii>,Comp> Q;
    Q.push(make_pair(1,2));
    Q.push(make_pair(2,2));
    Q.push(make_pair(3,2));
    while(not Q.empty()){
        cout << Q.top().first << " " << Q.top().second << endl;
        Q.pop();
    }
}
~~~~~~

## bitset
限られた大きさのvector\<bool\>を使いたいときに、bitsetを使うことができる。

~~~~~~{.cpp}
#include <iostream>
#include <bitset>

using namespace std;
const int N = 10;

struct bit_cmp{
    bool operator() (const bitset<N> &left,const bitset<N> &right) {
        for(int i=N-1;i>=0;i--){
            if(left[i] < right[i]) return true;
            if(left[i] > right[i]) return false;
        }
        return false;
    }
};


int main(){
    //定数じゃないとダメ。最初は全部false
    bitset<N> bits;
    // すべての要素をtrue -> 1111111111
    bits.set();
    if(bits.all()) cout << "all" << endl;
    // 立っているbitの数 -> 10
    cout << bits.count() << endl;
    // すべての要素をfalse -> 0000000000
    bits.reset();
    if(bits.none()) cout << "none" << endl;

    //1番目の要素をtrue -> 0100000000
    bits.set(1);
    if(bits.any()) cout << "any" << endl;

    // 0110000000
    bits.set(2);
    //1番目の要素をfalse -> 0010000000
    bits.reset(1);

    if(bits[2]) cout << 2 << endl;
    cout << bits << endl;

    bitset<N> newbits;
    // 和を取る
    bits |= newbits;
    // 積を取る
    bits &= newbits;

    // 関数オブジェクトを作る必要アリ
    map<bitset<N>,int,bit_cmp> M;
}
~~~~~~