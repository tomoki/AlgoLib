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

## ベルマンフォード
O(N|E|)

~~~~~~{.cpp}
const int INF = 1 << 30;
// s:始点,dist:距離,prev:最短経路木
bool bellman(const vector<vector<Edge> >& graph,int s,vector<int> &dist,vector<int> &prev){
    int n = graph.size();
    for(int i=0;i<n;i++) dist[i] = INF;
    dist[s] = 0;
    for(int i=0;i<n;i++) prev[i] = -1;

    bool neg_cycle = false;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            for(size_t k=0;k<graph[j].size();k++){
                const Edge &e = graph[j][k];
                if(dist[e.to] > dist[j] + e.cost){
                    dist[e.to] = dist[j] + e.cost;
                    prev[e.to] = j;
                    if(i == n-1){
                        dist[e.to] = -INF;
                        neg_cycle = true;
                    }
                }
            }
        }
    }
    return !neg_cycle;
}
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

int dijkstra(const int start,const int goal,
             const vector<vector<Edge> > &graph){
    typedef pair<int,int> pii;
    int N = graph.size();
    vector<char> visited(N,false);
    // cost where
    priority_queue<pii,vector<pii>,greater<pii> > que;
    que.push(make_pair(0,start));
    while(not que.empty()){
        int cost,where;
        cost = que.top().first;
        where = que.top().second;
        que.pop();
        if(visited[where]) continue;
        if(where == goal) return cost;
        visited[where] = true;
        for(int j=0;j<(int)graph[where].size();j++){
            que.push(make_pair(graph[where][j].cost+cost,graph[where][j].to));
        }
    }
    return -1;
}


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

## 直径

木の直径 (最も遠い２点の距離) を求める O(n)

~~~~~~{.cpp}
struct DiameterAnswer {
    int s, t; // 直径になる２点
    ll distance; // 直径
};
DiameterAnswer tree_diameter(const vector<vector<Edge>>& graph)
{
    ll INF = 1ll << 60;
    assert(graph.size() >= 1);
    // 適当なノードからスタートし、一番遠い点 s を求める
    // その後その点から一番遠い点 t を求める。
    // s -- t が木の直径

    // ノード番号、距離
    std::function<pair<int, ll>(int, int)> find_farthest_node = [&](int c, int from) {
        auto ret = make_pair(c, 0ll);
        for (const Edge& e : graph[c]) {
            if (e.to == from) continue;
            auto p = find_farthest_node(e.to, c);
            p.second += e.cost;
            if (p.second > ret.second) {
                ret = p;
            }
        }
        dump(ret);
        return ret;
    };
    int s = find_farthest_node(0, -1).first;
    auto p = find_farthest_node(s, -1);
    int t = p.first;
    auto dist = p.second;

    return {s, t, dist};
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

プリム法による。$O(N^2log(N))$だと思う。最小コストを求めるコードが以下。

~~~~~~{.cpp}
struct Edge{
    int to, cost;
    Edge(int to_,int cost_)
        : to(to_),cost(cost_) {}
};


vector<vector<Edge>> minimum_spanning_tree(const vector<vector<Edge>> &graph){
    typedef tuple<int,int,int> cft;  // cost,from,to
    int N = graph.size();
    vector<vector<Edge>> ret(N);
    vector<char> used(N,false);
    priority_queue<cft,vector<cft>,greater<cft>> que;
    que.push(make_tuple(0,-1,0));
    while(not que.empty()){
        int cost = get<0>(que.top());
        int from = get<1>(que.top());
        int to = get<2>(que.top());
        que.pop();
        if(used[to]) continue;
        used[to] = true;
        // ignore first.
        if(from != -1){
            ret[from].push_back(Edge(to,cost));
        }
        for(const Edge& e : graph[to]){
            que.push(make_tuple(e.cost,to,e.to));
        }
    }
    return ret;
}
~~~~~~

## 閉路の検出

~~~~~~
// グラフにある閉路を返す。 ({0, 1, 2, 3, 0}) のように最初と最後の値が同じものを返す
// https://drken1215.hatenablog.com/entry/2023/05/20/200517
optional<vector<int>> find_cycle(const vector<vector<int>>& graph)
{
    vector<bool> seen(graph.size()); // dfs を呼び出したか
    vector<bool> finished(graph.size()); // dfs を最後まで完了したか
    vector<int> history;
    std::function<int(int, int)> dfs = [&](int k, int p) -> int {
        seen[k] = true;
        history.push_back(k);
        for (int v : graph[k]) {
            if (v == p) continue; // 逆戻りはスキップ
            if (finished[v]) continue; // 操作済み
            if (seen[v] && !finished[v]) {
                // この呼び出しは v の中で発生しているので v を始点終点とするループがある
                history.push_back(v);
                return v;
            }
            int pos = dfs(v, k);
            if (pos >= 0) return pos;
        }
        finished[k] = true;
        history.pop_back();
        return -1;
    };

    for (int c = 0; c < graph.size(); c++) {
        if (seen[c]) continue;
        history.clear();
        int pos = dfs(c, -1);
        if (pos >= 0) {
            vector<int> ret;
            assert(history.size() >= 2);
            // history の最後はループの最初・最後の値。ループ開始前の値をここで削除する
            while (!history.empty()) {
                ret.push_back(history.back());
                history.pop_back();
                if (ret.size() >= 2 && ret.back() == pos) break; // ループの先頭までたどり着いた
            }
            // 逆順に追加したので戻す
            reverse(ret.begin(), ret.end());
            return ret;
        }
    }
    return nullopt;
}
~~~~~~

## 最大流

Dinic法による。また、最大流最小カット定理より、最大流と最小カットは一致する。

~~~~~~{.cpp}
struct Edge{
    int to,cap,rev;
    Edge(int _to,int _cap,int _rev) : to(_to),cap(_cap),rev(_rev) {};
};

void add_edge(vector<vector<Edge> >& E,int from,int to,int cap){
    E[from].push_back(Edge(to,cap,E[to].size()));
    E[to].push_back(Edge(from,0,E[from].size()-1));
}

vector<int> levels(vector<vector<Edge> > &E,int s){
    vector<int> level(E.size(),-1);
    level[s] = 0;
    queue<int> Q;
    Q.push(s);
    while(!Q.empty()){
        int v = Q.front();
        Q.pop();
        for(size_t i=0;i<E[v].size();i++){
            Edge &e = E[v][i];
            if(e.cap > 0 and level[e.to] == -1){
                level[e.to] = level[v]+1;
                Q.push(e.to);
            }
        }
    }
    return level;
}

int good_path(vector<vector<Edge> > &E,
        vector<int> &iter,
        vector<int> &level,
        int v,int t,int f){
    if(v == t) return f;
    for(int &i=iter[v];i<(int)E[v].size();i++){
        Edge &e = E[v][i];
        if(e.cap > 0 and level[v] < level[e.to]){
            int d = good_path(E,iter,level,e.to,t,min(f,e.cap));
            if(d > 0){
                e.cap -= d;
                E[e.to][e.rev].cap += d;
                return d;
            }
        }
    }
    return 0;
}

int max_flow(vector<vector<Edge> > E,int s,int t){
    int flow = 0;
    const int INF = 1 << 30;
    while(true){
        vector<int> level = levels(E,s);
        if(level[t] < 0) return flow;
        vector<int> iter(E.size());
        int f;
        while((f=good_path(E,iter,level,s,t,INF)) > 0){
            flow += f;
        }
    }
}

int main(){
    int N,M;
    while(cin >> N >> M){
        // [0,N) is cow,[N,N+M) is barn.
        vector<vector<Edge> > E(N+M+2);
        int s = N+M;
        int t = N+M+1;
        for(int i=0;i<N;i++){
            add_edge(E,s,i,1);
        }
        for(int i=0;i<M;i++){
            add_edge(E,N+i,t,1);
        }

        for(int i=0;i<N;i++){
            int S;
            cin >> S;
            for(int j=0;j<S;j++){
                int k;
                cin >> k;
                k--;
                add_edge(E,i,N+k,1);
            }
        }

        cout << max_flow(E,s,t) << endl;
    }
    return 0;
}

~~~~~~

## 最小費用流

Primal-Dual法による。

~~~~~~{.cpp}
typedef pair<int,int> pii;

struct Edge{
    int to,cap,cost,rev;
    Edge(int to,int cap,int cost,int rev)
        : to(to),cap(cap),cost(cost),rev(rev) {};
};

void add_edge(vector<vector<Edge> > &E,int from,int to,int cap,int cost){
    E[from].push_back(Edge(to,cap,cost,E[to].size()));
    E[to].push_back(Edge(from,0,-cost,E[from].size()-1));
}

// s -> t (flow f)
//  if cant, return -1.
int min_cost_flow(vector<vector<Edge> > E,int s,int t,int f){
    const int INF = 1 << 30;
    int ret = 0;
    // potential
    vector<int> h(E.size());
    vector<int> prevv(E.size());
    vector<int> preve(E.size());

    while(f > 0){
        vector<int> dist(E.size(),INF);
        dist[s] = 0;
        priority_queue<pii,vector<pii>,greater<pii> > que;
        que.push(make_pair(0,s));
        while(!que.empty()){
            pii p = que.top();
            que.pop();
            int pf = p.first,ps = p.second;
            if(dist[ps] < pf) continue;
            for(int i=0;i<E[ps].size();i++){
                Edge &e = E[ps][i];
                if(e.cap > 0 and dist[e.to] > dist[ps] + e.cost + h[ps] - h[e.to]){
                    dist[e.to] = dist[ps] + e.cost + h[ps] - h[e.to];
                    prevv[e.to] = ps;
                    preve[e.to] = i;
                    que.push(make_pair(dist[e.to],e.to));
                }
            }
        }
        if(dist[t] == INF){
            return -1;
        }
        for(int v=0;v<E.size();v++){
            h[v] += dist[v];
        }
        int d = f;
        for(int v=t;v!=s;v=prevv[v]){
            d = min(d,E[prevv[v]][preve[v]].cap);
        }
        f -= d;
        ret += d * h[t];
        for(int v=t;v!=s;v=prevv[v]){
            Edge &e = E[prevv[v]][preve[v]];
            e.cap -= d;
            E[v][e.rev].cap += d;
        }
    }
    return ret;
}

int main(){
    while(true){
        int N,M;
        cin >> N >> M;
        if(N == 0 and M == 0) break;
        vector<pair<int,int> > men;
        vector<pair<int,int> > houses;
        for(int h=0;h<N;h++){
            for(int w=0;w<M;w++){
                char x;
                cin >> x;
                if(x == 'H') houses.push_back(make_pair(h,w));
                else if(x == 'm') men.push_back(make_pair(h,w));
            }
        }

        vector<vector<Edge> > E(men.size()+houses.size()+2);
        int s = men.size()+houses.size();
        int t = s+1;

        for(int i=0;i<men.size();i++){
            add_edge(E,s,i,1,0);
            for(int j=0;j<houses.size();j++){
                int dist = abs(men[i].first - houses[j].first)
                    + abs(men[i].second - houses[j].second);
                add_edge(E,i,men.size()+j,1,dist);
            }
        }

        for(int i=0;i<houses.size();i++){
            add_edge(E,men.size()+i,t,1,0);
        }

        cout << min_cost_flow(E,s,t,men.size()) << endl;
    }
    return 0;
}

~~~~~~

## 無向グラフにおける最小カット

Nagamochi-Ibaraki/Storer-Wagnerの方法によってO(V^3)で計算できる。

~~~~~~{.cpp}
struct Edge{
    int to,cap,rev;
    Edge(int _to,int _cap,int _rev) : to(_to),cap(_cap),rev(_rev) {};
};

void add_edge(vector<vector<Edge> >& E,int from,int to,int cap){
    E[from].push_back(Edge(to,cap,E[to].size()));
    E[to].push_back(Edge(from,0,E[from].size()-1));
}

// Nagamochi-Ibaraki/Stoer-Wagner
//  http://www.prefield.com/algorithm/graph/minimum_cut.html
int minimum_cut_of_undirected_graph(const vector<vector<Edge> > &graph){
    const int INF = 1 << 30;
    int n = graph.size();
    vector<vector<int> > adj(n,vector<int>(n));
    for(int i=0;i<n;i++){
        for(size_t j=0;j<graph[i].size();j++){
            Edge e = graph[i][j];
            adj[i][e.to] += e.cap;
        }
    }
    vector<int> h(n);
    for(int i=0;i<n;i++){
        h[i] = i;
    }
    int cut = INF;
    for(int m = n;m > 1;m--){
        vector<int> ws(m,0);
        int u,v,w;
        for(int k=0;k<m;k++){
            u = v;
            v = max_element(ws.begin(),ws.end()) - ws.begin();
            w = ws[v];
            ws[v] = -1;
            for(int i=0;i<m;i++){
                if(ws[i] >= 0) {
                    ws[i] += adj[h[v]][h[i]];
                }
            }
        }
        for(int i=0;i<m;i++){
            adj[h[i]][h[u]] += adj[h[i]][h[v]];
            adj[h[u]][h[i]] += adj[h[v]][h[i]];
        }
        h.erase(h.begin()+v);
        cut = min(cut,w);
    }
    return cut;
}

int main(){
    int N,M;
    while(scanf("%d %d\n",&N,&M) != EOF){
        vector<vector<Edge> > graph(N);
        for(int i=0;i<M;i++){
            int a,b,c;
            scanf("%d %d %d\n",&a,&b,&c);
            add_edge(graph,a,b,c);
            add_edge(graph,b,a,c);
        }
        printf("%d\n",minimum_cut_of_undirected_graph(graph));
    }
    return 0;

~~~~~~

## 強連結成分分解

~~~~~~{include="cpp/scc.cpp" .cpp}
~~~~~~

## 橋の列挙

~~~~~~{include="cpp/bridge.cpp" .cpp}
~~~~~~

## Lowest Common Ancestor
木において、根から最も遠い、u,vの共通の祖先をLCAと呼ぶ。

~~~~~~{include="cpp/lca.cpp" .cpp}
~~~~~~
