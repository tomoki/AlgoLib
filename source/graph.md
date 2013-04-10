# グラフ

## 構成要素

隣接行列を使うか、vector\<Edge\>みたいのを使うかの二択。場合によってはNode
みたいなのも使うかも。隣接行列を使うとメモリとか探すのとか重い。

~~~~~~{.cpp}
struct Edge{
    int to,cost;
    Edge(int to,int cost) : to(to),cost(cost) {};
};
~~~~~~

## ダイクストラ

負の経路があったらダメ

~~~~~~{.cpp}
#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <algorithm>
#include <functional>

using namespace std;

typedef pair<int,int> pii;

struct Edge{
    int from,to,cost;
    Edge(int from,int to,int cost)
        : from(from),to(to),cost(cost) {};
};

int main(){
    int n,m;
    cin >> n >> m;
    vector<vector<Edge> > V(m);

    for(int i=0;i<n;i++){
        int a,b,cost;
        cin >> a >> b >> cost;
        V[a].push_back(Edge(a,b,cost));
        V[b].push_back(Edge(b,a,cost));
    }

    int ret = -1;
    int p,q;
    cin >> p >> q;
    vector<char> visited(m,false);
    //                 cost,where
    priority_queue<pii,vector<pii>, greater<pii> > Q;
    Q.push(make_pair(0,p));
    while(!Q.empty()){
        int cost,where;
        cost = Q.top().first;
        where = Q.top().second;
        Q.pop();
        if(visited[where]) continue;
        if(where == q){
             ret = cost;
             break;
        }
        visited[where] = true;
        for(int j=0;j<(int)V[where].size();j++){
            Q.push(make_pair(V[where][j].cost+cost,V[where][j].to));
        }
    }
    // 到達不能なときは-1
    cout << ret << endl;
    return 0;
}
~~~~~~


## ワーシャルフロイド

負の経路があってもOK。すべてのノードに対してすべてのノードへの距離を求め
る。もし負の閉路があったらiからiはマイナスになる。$O(|V|^{3})$

~~~~~~{.cpp}
// m はノードの個数。NOはでかい数
vector<vector<int> > V(m,vector<int>(m,NO));
// i->iは0にする。
rep(i,m){
    V[i][i] = 0;
}

for(int k=0;k<m;k++){
    for(int i=0;i<m;i++){
        for(int j=0;j<m;j++){
            V[i][j] = min(V[i][j],V[i][k]+V[k][j]);
        }
    }
}
~~~~~~

## 最小全域木

プラム法による。$O(N^3$)だと思う。最小コストを求めるコードが以下。
ただし、vector<Edge>を使えばもっとよい。

~~~~~~{.cpp}
typedef pair<int,int> pii;

int main(){
    int N;
    while(cin >> N){
        vector<vector<int> > M(N,vector<int>(N));
        // 距離行列を読みこむ
        rep(i,N) rep(j,N) cin >> M[i][j];
        vector<char> used(N,false);
        ll ret = 0;
        // cost , where.
        priority_queue<pii,vector<pii>,greater<pii> > Q;
        Q.push(mp(0,0));
        while(!Q.empty()){
            int cost = Q.top().first;
            int where = Q.top().second;
            Q.pop();
            if(used[where]) continue;
            used[where] = true;
            ret += cost;
            for(int i=0;i<N;i++){
                Q.push(mp(M[where][i],i));
            }
        }
        cout << ret << endl;
    }
    return 0;
}
~~~~~~
