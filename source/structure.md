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

struct Robot{
    int y,x,dir,step;
    Robot(int y,int x,int dir,int step) : y(y),x(x),dir(dir),step(step) {};
};

// <,>を定義すればless<Robot>みたいに扱える。
bool operator<(const Robot& lhs,const Robot& rhs){
    return lhs.step < rhs.step;
}
bool operator>(const Robot& lhs,const Robot& rhs){
    return lhs.step > rhs.step;
}

int main(){
    // 何も書かないと降順。(おっきい方からでてくる。)
    // これは昇順(ちいさいほうから出てくる)にしたもの。
    priority_queue<int,vector<int>,greater<int> > Qi;
    //関数オブジェクトを使っていい感じにもできる。
    priority_queue<pii,vector<pii>,Comp> Q;
    // 自作クラスの場合はこんな感じ
    priority_queue<Robot,vector<Robot>,greater<Robot> > que;

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

## 分数
テストちゅう.

~~~~~~{.cpp}
typedef long long ll;
ll gcd(ll a,ll b){
    return b==0 ? a : gcd(b,a%b);
}

ll lcm(ll a,ll b){
    if(a < 0) a *= -1;
    if(b < 0) b *= -1;
    return a*b / gcd(a,b);
}

struct Fraction{
    ll n,d;
    Fraction(ll _n,ll _d){
        ll c = lcm(_n,_d);
        n = c / _d;
        d = c / _n;
        if(d < 0){
            n *= -1;
            d *= -1;
        }
    }

    Fraction(ll _n){
        Fraction(_n,1);
    }

    bool operator<(const Fraction& r) const{
        ll c_d = lcm(d,r.d);
        return n*(c_d/d)< r.n*(c_d/r.d);
    }
    bool operator>(const Fraction& r) const{
        return not ((*this) < r or (*this) == r);
    }
    bool operator<=(const Fraction& r) const{
        return (*this) < r or (*this) == r;
    }
    bool operator>=(const Fraction& r) const{
        return (*this) > r or (*this) == r;
    }
    bool operator==(const Fraction& r) const{
        return n == r.n and d == r.d;
    }
    Fraction operator+(const Fraction& r) const{
        ll c_d = lcm(d,r.d);
        return Fraction(n*(c_d/d)+r.n*(c_d/r.d),c_d);
    }
    Fraction operator-(const Fraction& r) const{
        return (*this) + (-r);
    }
    Fraction operator*(const Fraction& r) const{
        return Fraction(n*r.n,d*r.d);
    }
    Fraction operator/(const Fraction& r) const{
        return (*this) * Fraction(r.d,r.n);
    }
    Fraction operator+() const{
        return Fraction(n,d);
    }
    Fraction operator-() const{
        return (*this) * -1;
    }
    Fraction operator*(const ll& a) const{
        return Fraction(a*n,d) ;
    }
};

ostream& operator<<(ostream &os,const Fraction& f){
    os << f.n << "/" << f.d;
    return os;
}
~~~~~~
