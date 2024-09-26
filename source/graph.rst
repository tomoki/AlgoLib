######################
グラフ
######################

****************************************
構成要素
****************************************

隣接行列を使うか、vector\<Edge\>みたいのを使うかの二択。場合によってはNode
みたいなのも使うかも。隣接行列を使うとメモリとか探すのとか重い。

.. code-block:: cpp

    struct Edge{
        int to,cost;
        Edge(int to,int cost) : to(to),cost(cost) {};
    };

****************************************
ベルマンフォード
****************************************

ある点から他の点への最短経路を計算する。負の閉路があっても対応可能で、負の閉路の検出も可能 O(NE)

.. code-block:: cpp

    template<typename T>
    struct Edge {
        int to;
        T value;
        Edge(int to, const T& v) : to(to), value(v) {}
    };

    template<typename T>
    struct BellmanFordResult {
        bool has_negative_cycle;
        vector<T> dist;
        vector<int> prev;
        const T INF = std::numeric_limits<T>::max() / 2;
    };

    // 始点 s からの全てのノードへの最短距離を計算する
    // result.has_negative_cycle: 負の閉路が存在するかどうか
    // result.dist: それぞれのノードへの最短距離、ただし到達不可能な時は result.INF, 負の閉路の影響で無限に負になるときは -result.INF
    // result.prev: 最短経路を実現するための経路木
    template<typename T>
    BellmanFordResult<T> bellman_ford(const vector<vector<Edge<T>>>& graph, int s) {
        BellmanFordResult<T> result;
        result.has_negative_cycle = false;
        result.dist = vector<T>(graph.size(), result.INF);
        result.dist[0] = 0;
        result.prev = vector<int>(graph.size(), -1);

        for (size_t i = 0; i < graph.size(); i++) {
            for(size_t j = 0; j < graph.size(); j++) {
                for(size_t k = 0; k < graph[j].size(); k++) {
                    const auto& e = graph[j][k];
                    if(result.dist[e.to] > result.dist[j] + e.value){
                        result.dist[e.to] = result.dist[j] + e.value;
                        result.prev[e.to] = j;
                        if(i == graph.size()-1){
                            result.dist[e.to] = -result.INF;
                            result.has_negative_cycle = true;
                        }
                    }
                }
            }
        }
        return result;
    }

****************************************
ダイクストラ
****************************************

負の経路があったらダメ

.. code-block:: cpp

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

****************************************
直径
****************************************

木の直径 (最も遠い２点の距離) を求める O(n)

.. code-block:: cpp

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

****************************************
ワーシャルフロイド
****************************************

すべてのノードに対してすべてのノードへの距離を求める。
負の経路があってもOK。もし負の閉路があったらiからiはマイナスになる。 :math:`O(|V|^{3})`

.. code-block:: cpp

    // 隣接行列の形でグラフを受け取って、 全点間距離を計算する。 O(頂点の数 ^3)
    // 渡す際に graph[0][0] = 0 としなければならず、また多重辺があるなら最小の辺を行列の値としておく。
    // 負の辺があっても良く、負の閉路があった場合は graph[i][i] < 0 になる。
    template<typename T>
    void warshall_floyd(std::vector<std::vector<T>>& graph)
    {
    #ifdef DEBUG
        // N x N の行列でなければならない
        for (size_t i = 0; i < graph.size(); i++) {
            assert(graph[i].size() == graph.size());
        }
        // graph[x][x] = 0 でなければならない
        for (size_t i = 0; i < graph.size(); i++) {
            assert(graph[i][i] == 0);
        }
    #endif
        const size_t n = graph.size();
        for(size_t k = 0; k < n; k++){
            for(size_t i = 0; i < n; i++){
                for(size_t j = 0; j < n; j++){
                    graph[i][j] = std::min(graph[i][j], graph[i][k] + graph[k][j]);
                }
            }
        }
    }

****************************************
最小全域木
****************************************

クラスカル法
====================

クラスカル法によって最小全域木を構築する、大体 O(MlogM) (M = グラフの辺の数)
辺をコストによってソートしておいて、もしその辺を追加したら連結成分が更新される時だけ追加する
プリム法より早めかもしれない。

.. code-block:: cpp

    template<typename T>
    struct MinimumSpanningTreeResult {
        bool success; // そもそも全域木が構築できたかどうか
        vector<vector<Edge<T>>> graph; // 最小全域木
        T total_cost; // 最小全域木全体のコスト
    };

    // クラスカル法によって最小全域木を構築する、大体 O(MlogM) (M = グラフの辺の数)
    // 辺をコストによってソートしておいて、もしその辺を追加したら連結成分が更新される時だけ追加する
    // プリム法より早めかもしれない。
    // Ref: https://atcoder.jp/contests/typical90/tasks/typical90_aw
    template<typename T>
    MinimumSpanningTreeResult<T> minimum_spanning_tree(const vector<vector<Edge<T>>>& graph)
    {
        // UnionFind を使う、ありものを使ってもいいがライブラリ間の依存を減らすために内部で定義することにする
        vector<int> uf(graph.size(), -1); // uf の値は負ならその集合のサイズに -1 をかけたもの、非負なら親のインデックス
        // 親のインデックスを返す
        function<int(int)> root = [&uf, &root](int x) {
            if (uf[x] < 0) return x;
            return uf[x] = root(uf[x]);
        };
        // x と y を併合
        auto unite = [&uf, &root](int x, int y) {
            x = root(x);
            y = root(y);
            if (x == y) return;
            // 集合 x に 集合 y を併合する、 もし y の方が大きいなら x, y を入れ替え
            if (uf[y] < uf[x]) swap(x, y);
            uf[x] += uf[y];
            uf[y] = x;
        };

        // Edge 構造体に辺の元の情報がないので追加しておく
        vector<pair<int, Edge<T>>> sorted_edges;
        for (int i = 0; i < static_cast<int>(graph.size()); i++) {
            for (const auto& e : graph[i]) {
                sorted_edges.push_back({i, e});
            }
        }
        // コストでソート
        std::sort(sorted_edges.begin(), sorted_edges.end(), [](const auto& e1, const auto& e2) {
            return e1.second.value < e2.second.value;
        });
        T total_cost = 0;
        vector<vector<Edge<T>>> minimum_spanning_tree(graph.size());
        for (const auto& e : sorted_edges) {
            int from = e.first;
            int to = e.second.to;
            const auto& cost = e.second.value;
            if (root(from) == root(to)) continue; // すでに同じ連結成分に存在
            unite(from, to);
            total_cost += cost;
            minimum_spanning_tree[from].push_back(e.second);
        }

        int largest_group_size = -*min_element(uf.begin(), uf.end());
        if (largest_group_size != static_cast<int>(graph.size())) {
            // 全域木が構築できていない
            return MinimumSpanningTreeResult<T>{ false, {}, 0 };
        }
        return MinimumSpanningTreeResult<T> { true, minimum_spanning_tree, total_cost };
    }

プリム法
=====================

:math:`O(N^2log(N))` だと思う。最小コストを求めるコードが以下。

.. code-block:: cpp

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

****************************************
閉路の検出
****************************************

.. code-block:: cpp

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

****************************************
最大流
****************************************

Dinic 法と FordFulkerson 法がある、 FordFulkerson は計算量がフローの総量に依存する。 Dinic 法はノードの数とエッジの数に依存する。 特に二部マッチングの時、 FordFulkerson は O(n^2)、 Dinic 法は O(n^(3/2)) になる。
また、最大流最小カット定理より、最大流と最小カットは一致する。

.. code-block:: cpp

    template<typename Capacity>
    class MaxFlow {
    public:
        virtual ~MaxFlow() = default;
        virtual int add_edge(int from, int to, Capacity cap) = 0;
        virtual Capacity flow(int from, int to) = 0;
    };

    // フローを流す処理は ...
    template<typename Capacity>
    struct Dinic : public MaxFlow<Capacity> {
        static_assert(std::is_integral_v<Capacity>, "Capacity must be integral");

        explicit Dinic(size_t number_of_node)
        : graph(number_of_node)
        {
        }

        struct Edge {
            int to; // エッジの行き先
            int rev; // graph[to][rev] が残余エッジ
            Capacity flow; // 現在流れている量
            Capacity cap; // 辺の容量
            bool is_rev_edge; // 残余グラフ用に追加したエッジかどうか、フィルター用
        };

        friend ostream& operator<<(ostream& os, const Edge& e) noexcept
        {
            return os << "{" << e.to << ", " << e.flow << "/" << e.cap << "}";
        }

        // エッジを追加し、そのインデックスを返す。残余グラフがあるため返り値が連続とは限らない
        int add_edge(int from, int to, Capacity cap) override
        {
            assert(from < static_cast<int>(graph.size()));
            assert(to < static_cast<int>(graph.size()));
            graph[from].push_back(Edge { to, static_cast<int>(graph[to].size()), 0, cap, false });
            graph[to].push_back(Edge { from, static_cast<int>(graph[from].size())-1, cap, cap, true });

            return graph[from].size() - 1;
        }

        // from から to まで最大までフローを流し、流量を返す
        Capacity flow(int from, int to) override
        {
            assert(from < static_cast<int>(graph.size()));
            assert(to < static_cast<int>(graph.size()));
            Capacity total_flow = 0;

            // Dinic 法では from から to の最短経路 DAG (閉路のない有向グラフ) を BFS で作成し、
            // DFS でその上の増加パスに流せるだけ流す、を繰り返す
            while (true) {
                auto level = levels(from);
                if (level[to] < 0) break; // そもそも to まで到達不可能なら終了する
                vector<int> iter(graph.size());
                // DAG 上で流せるだけ流す
                while (true) {
                    Capacity f = good_path(from, to, std::numeric_limits<Capacity>::max(), iter, level);
                    if (f == 0) break;
                    total_flow += f;
                }
            }
            return total_flow;
        }

        // from から伸びているエッジを返す、デフォルトで残余グラフはフィルターアウトする
        std::vector<Edge> get_edges(int from, bool filter_rev_edge = true)
        {
            assert(from < static_cast<int>(graph.size()));

            std::vector<Edge> ret;
            for (const auto& e : graph[from]) {
                if (!filter_rev_edge || !e.is_rev_edge)
                    ret.push_back(e);
            }
            return ret;
        }

        // from から伸びているエッジを返す、index は add_edge が返した値
        Edge get_edge(int from, int index)
        {
            return graph[from][index];
        }

        size_t size() { return graph.size(); }
        bool resize(size_t n)
        {
            assert(n >= graph.size());
            graph.resize(n);
        }
    private:
        // from から始まる BFS により DAG を生成する。
        // 戻り値は v[i] = from から到達可能なら from からの距離、そうでないなら -1
        vector<int> levels(int from) {
            vector<int> level(graph.size(),-1);
            level[from] = 0;
            queue<int> que; que.push(from);
            while(!que.empty()){
                int v = que.front(); que.pop();
                for (auto& e : graph[v]) {
                    // まだフローが流せて、かつまだ行っていないノードを追加
                    if(e.cap - e.flow > 0 and level[e.to] == -1){
                        level[e.to] = level[v]+1;
                        que.push(e.to);
                    }
                }
            }
            return level;
        }

        // levels で生成された DAG 上を DFS してフローを流す
        Capacity good_path(int from, int to, Capacity flow, vector<int>& iter, const vector<int>& level){
            if(from == to) return flow;
            for(int &i = iter[from]; i < static_cast<int>(graph[from].size()); i++){
                auto& e = graph[from][i];
                if(e.cap - e.flow > 0 && level[from] < level[e.to]){
                    Capacity nf = good_path(e.to, to, min(flow, e.cap - e.flow), iter, level);
                    if(nf > 0){
                        e.flow += nf;
                        graph[e.to][e.rev].flow -= nf;
                        return nf;
                    }
                }
            }
            return 0;
        }
        std::vector<std::vector<Edge>> graph;
    };

FordFulkerson は以下。あまり使わないかも。

.. code-block:: cpp

    // フローを流す処理は O(FM) (F = フローの総量、 M = エッジの数)
    template<typename Capacity>
    struct FordFulkerson : public MaxFlow<Capacity> {
        static_assert(std::is_integral_v<Capacity>, "Capacity must be integral");

        explicit FordFulkerson(size_t number_of_node)
        : graph(number_of_node)
        {

        }

        struct Edge {
            int to; // エッジの行き先
            int rev; // graph[to][rev] が残余エッジ
            Capacity flow; // 現在流れている量
            Capacity cap; // 辺の容量
            bool is_rev_edge; // 残余グラフ用に追加したエッジかどうか、フィルター用
        };

        friend ostream& operator<<(ostream& os, const Edge& e) noexcept
        {
            return os << "{" << e.to << ", " << e.flow << "/" << e.cap << "}";
        }

        // エッジを追加し、そのインデックスを返す。残余グラフがあるため返り値が連続とは限らない
        int add_edge(int from, int to, Capacity cap) override
        {
            assert(from < static_cast<int>(graph.size()));
            assert(to < static_cast<int>(graph.size()));
            graph[from].push_back(Edge { to, static_cast<int>(graph[to].size()), 0, cap, false });
            graph[to].push_back(Edge { from, static_cast<int>(graph[from].size())-1, cap, cap, true });

            return graph[from].size() - 1;
        }

        // from から to まで最大までフローを流し、流量を返す
        Capacity flow(int from, int to) override
        {
            assert(from < static_cast<int>(graph.size()));
            assert(to < static_cast<int>(graph.size()));
            Capacity total_flow = 0;

            // フォードフルカーソンアルゴリズムは繰り返し dfs を行い、残余グラフ含めたグラフにフローを流す
            // 最悪ケースは 1 ずつ流量が増えていく、 dfs は O(エッジの数) であり、この dfs を総量分行うことになる。
            while (true) {
                std::vector<bool> visited(graph.size(), false);
                Capacity f = dfs(from, to, std::numeric_limits<Capacity>::max(), visited);
                if (f == 0) break; // 新しくフローが流せなかった
                total_flow += f;
            }

            return total_flow;
        }

        // from から伸びているエッジを返す、デフォルトで残余グラフはフィルターアウトする
        std::vector<Edge> get_edges(int from, bool filter_rev_edge = true)
        {
            assert(from < static_cast<int>(graph.size()));

            std::vector<Edge> ret;
            for (const auto& e : graph[from]) {
                if (!filter_rev_edge || !e.is_rev_edge)
                    ret.push_back(e);
            }
            return ret;
        }

        // from から伸びているエッジを返す、index は add_edge が返した値
        Edge get_edge(int from, int index)
        {
            return graph[from][index];
        }

    private:
        // from から to まで dfs を行い、その途中の最小の容量を f として保存する
        int dfs(int from, int to, Capacity flow, vector<bool>& visited)
        {
            if (from == to) return flow;
            visited[from] = true;
            for (auto& v : graph[from]) {
                if (v.flow == v.cap) continue; // すでにマックスまで流している
                if (visited[v.to]) continue; // 訪問済み、ループ
                Capacity nf = dfs(v.to, to, min(flow, v.cap - v.flow), visited);
                if (nf >= 1) {
                    // 残余グラフの flow を増やしつつ、 通常グラフの flow は減らす
                    v.flow += nf;
                    graph[v.to][v.rev].flow -= nf;
                    return nf;
                }
            }
            return 0; // どの辺にも流せなかった
        }
        std::vector<std::vector<Edge>> graph;
    };

例

.. code-block:: cpp

    int main(){
        // https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_6_A&lang=ja
        int v, e; cin >> v >> e;
        int source = 0;
        int sink = v-1;

        Dinic<int> maxflow(v);
        for (int i = 0; i < e; i++) {
            int u, v, c; cin >> u >> v >> c;
            maxflow.add_edge(u, v, c);
        }
        cout << maxflow.flow(source, sink) << endl;
        return 0;
    }

燃やす埋める (Project Selection)
=======================================

変数 x1, x2 ... xn があり、それぞれ 0 か 1 のどちらかを選択できる。
xi を 0 にしたときのコストを Fi, 1 にしたときのコストを Ti とする。
さらに xi = 1 かつ xj = 0 の時追加のコスト Cij がかかる。
(俗に 0/1 を燃やす・埋めるにした問題になぞらえて燃やす・埋めると呼ばれているらしい)
https://drken1215.hatenablog.com/entry/2023/11/24/034300

上記の問題を最小カット (グラフの頂点を S 側、 T 側にわけ、 S -> T の辺の重さの最小を求める問題) に帰着する。

.. code-block:: cpp

    // Ref:
    // - https://github.com/drken1215/algorithm/blob/master/GraphNetworkFlow/two_variable_submodular_optimization.cpp
    // - https://drken1215.hatenablog.com/entry/2023/11/24/034300
    // 最小カットに帰着するため以下のようにグラフを構築する。
    // - source を含むカットに含まれる => 選択する (true, 1)
    // - destination を含むカットに含まれる => 選択しない (false, 0)
    // 内部で　Dinic 法を使うため、 O(N^2*ルールの数) になる。
    // TODO: 最小カットの値を求めるだけではなく、カット (どのノードがどちらのグループに含まれるのか) を計算する方法を検討する
    template<typename Cost>
    struct ProjectSelection {
        explicit ProjectSelection(size_t number_of_node, Cost infinity = std::numeric_limits<Cost>::max() / 1000)
        :
        source(static_cast<int>(number_of_node)),
        destination((static_cast<int>(number_of_node) + 1)),
        flow(number_of_node + 2),
        offset(0),
        infinity(infinity)
        {
        }
        // i 番目の要素を選択したときのコストを true_cost, しない時を false_cost
        // ※ 問題が「利得を得る」であった場合は利得に -1 をかけたものを入力し、最後の答えを *-1 する。
        // この関数は選択したときの「損失」であるため
        void add_single_cost(const int i, const Cost true_cost, const Cost false_cost)
        {
            // アイデアとして、
            // - source 側にノードを含める = true にするならば true_cost がカットに含まれる
            // - destination 側にノードを含める = false にするならば false_cost がカットに含まれる
            // ようにグラフを構築する
            assert(0 <= i && i < static_cast<int>(flow.size()));
            if (true_cost >= 0 && false_cost >= 0) {
                // source から i に false_cost を生やすことで、 もし i を true にした場合は false_cost の辺は含まれず、
                // 逆に true_cost の辺がカットに含まれるようになる。
                // (source は true 側であるが、 false_cost の辺を生やすので直感とは逆になるかもしれない)
                flow.add_edge(source, i, false_cost);
                flow.add_edge(i, destination, true_cost);
            } else if (false_cost >= true_cost) {
                // フローを流す際にグラフに負の辺があると困るので、
                // 最終的な答えに +true_cost し、 false にした場合は (false_cost - true_cost) が増えることにする
                flow.add_edge(source, i, false_cost - true_cost);
                offset += true_cost;
            } else {
                // 上に同じ
                flow.add_edge(i, destination, true_cost - false_cost);
                offset += false_cost;
            }
        }
        // どちらかが true, もう片方が false の時のコストを追加する (いわゆる埋める・燃やす)
        void add_true_false_cost(const int true_i, const int false_i, const Cost cost)
        {
            assert(0 <= true_i && true_i < static_cast<int>(flow.size()));
            assert(0 <= false_i && false_i < static_cast<int>(flow.size()));
            assert(cost >= 0);
            // true となるノードから false となるノードに辺を張ることで、カットにその辺が含まれるようになる
            flow.add_edge(true_i, false_i, cost);
        }
        // true_i を選択し、 false_i を選択しないことを許さないようにする
        void ban_true_false(const int true_i, const int false_i)
        {
            // 無限のコストを追加する
            return add_true_false_cost(true_i, false_i, infinity);
        }
        // 両方が true の時の利得を追加する
        // TODO: 未検証
        void add_true_true_profit(const int true_i, const int true_j, const Cost profit)
        {
            // source から i に、 i から j にそれぞれ profit の辺をはり、オフセットに -profit する
            // true, true => 辺はどちらも source 側にあるので -profit
            // true, false => source から i の辺はカウントされないが、 i から j の辺はカット間のあるので、 0
            // false, true => source から i の辺はカウントされ、 i から j の辺はカウントされない (destination 側から source 側への辺なので) 合計 0
            // false, false => source から i の辺はカウントされ、 i から j の辺は両方 destination 側なのでカウントされない、合計 0
            assert(profit >= 0);
            assert(0 <= true_i && true_i < static_cast<int>(flow.size()));
            assert(0 <= true_j && true_j < static_cast<int>(flow.size()));

            flow.add_edge(source, true_i, profit);
            flow.add_edge(true_i, true_j, profit);
            offset += (-profit);
        }
        // 両方が false の時の利得を追加する
        // TODO: 未検証
        void add_false_false_profit(const int false_i, const int false_j, const Cost profit)
        {
            // i から destination に、 j から i にそれぞれ profit の辺をはり、オフセットに -profit する
            // true, true => i から destination の辺はカウントされ、 j から i はカウントされない。合計 0
            // true, false => i から destination の辺はカウントされ、 j から i はカウントされない (destination 側から source 側の辺なので)。合計 0
            // false, true => i から destination の辺はカウントされず、 j から i はカウントされる。 合計 0
            // false, false => i から destination の辺も j から i の辺もカウントされない -profit
            assert(profit >= 0);
            assert(0 <= false_i && false_i < static_cast<int>(flow.size()));
            assert(0 <= false_j && false_j < static_cast<int>(flow.size()));
            flow.add_edge(false_i, destination, profit);
            flow.add_edge(false_j, false_i, profit);
            offset += (-profit);
        }
        // true_is に含まれるものがすべて true の時の利得を追加する
        // TODO: 未検証
        void add_all_true_profit(const vector<int>& true_is, const Cost profit)
        {
            assert(profit >= 0);
            assert(all_of(all(true_is), [&](int i){ 0 <= i && i < static_cast<int>(flow.size());}));
            // 補助頂点 y を追加し、 source から y に profit の辺を張り、
            // y から true になるノード全てに♾️の辺をはる。またオフセットに -profit を設定
            // => y が source 側に存在する時のみ全体のコストが -profit になるが、一方で true_is の一つでも false だと無限のコストが発生する
            const int y = flow.size();
            flow.resize(flow.size() + 1);
            flow.add_edge(source, y, profit);
            for (int i : true_is) {
                flow.add_edge(y, i, infinity);
            }
            offset += (-profit);
        }
        // false_is に含まれるものがすべて false の時の利得を追加する
        // TODO: 未検証
        void add_all_false_profit(const vector<int>& false_is, const Cost profit)
        {
            assert(profit >= 0);
            assert(all_of(all(false_is), [&](int i){ 0 <= i && i < static_cast<int>(flow.size());}));
            // 補助頂点 y を追加し、 y から destination に profit の辺を張り、
            // false になるノード全てから y に♾️の辺をはる。またオフセットに -profit を設定
            // => y が destination 側に存在する時のみ全体のコストが -profit になるが、一方で false_is の一つでも true だと無限のコストが発生する
            const int y = flow.size();
            flow.resize(flow.size() + 1);
            flow.add_edge(y, destination, profit);
            for (int i : false_is) {
                flow.add_edge(i, y, infinity);
            }
            offset += (-profit);
        }
        // より一般的な関係コストの追加
        // TODO: 未検証
        void add_cost_general(int i, int j, Cost true_true_cost, Cost true_false_cost, Cost false_true_cost, Cost false_false_cost)
        {
            assert(0 <= i && i < static_cast<int>(flow.size()));
            assert(0 <= j && j < static_cast<int>(flow.size()));
            assert(false_true_cost + true_false_cost >= true_true_cost + false_false_cost);
            offset += true_true_cost;
            add_single_cost(i, false_false_cost - false_true_cost, 0);
            add_single_cost(j, false_true_cost - true_true_cost, 0);
            add_true_false_cost(i, j, false_false_cost + true_false_cost - true_true_cost - false_false_cost);
        }
        Cost solve()
        {
            return flow.flow(source, destination) + offset;
        }
    private:
        size_t number_of_node;
        int source;
        int destination;
        Dinic<Cost> flow;
        Cost offset;
        Cost infinity;
    };

以下例

.. code-block:: cpp

    // https://atcoder.jp/contests/typical90/tasks/typical90_an
    int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
    {
        // 小数点以下10桁表示にする場合は以下をコメントアウト
        // cout << fixed << setprecision(10);
        int n; ll w; cin >> n >> w;
        ProjectSelection<ll> project_selection(n);
        auto a = input_vector<ll>(n);

        // 選択する = 入る
        // 選択しない = 入らない
        vector<vector<int>> keys(n);
        for (int i = 0; i < n; i++) {
            int k; cin >> k;
            for (int j = 0; j < k; j++) {
                int c; cin >> c;
                c--;
                keys[i].push_back(c);
            }
        }

        for (int i = 0; i < n; i++) {
            project_selection.add_single_cost(i, w - a[i], 0);
            // i を選択しない場合、 その家にある鍵を取れないので入れなくなる
            for (int k : keys[i]) {
                project_selection.ban_true_false(k, i);
            }
        }

        auto ret = -project_selection.solve();
        cout << ret << endl;
        return 0;
    }

****************************************
最小費用流
****************************************

Primal-Dual法による。

.. code-block:: cpp

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


****************************************
無向グラフにおける最小カット
****************************************

Nagamochi-Ibaraki/Storer-Wagnerの方法によってO(V^3)で計算できる。

.. code-block:: cpp

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


****************************************
強連結成分分解
****************************************

.. literalinclude:: cpp/scc.cpp

****************************************
橋の列挙
****************************************

.. literalinclude:: cpp/bridge.cpp

****************************************
Lowest Common Ancestor
****************************************
木において、根から最も遠い、u,vの共通の祖先をLCAと呼ぶ。

.. code-block:: cpp

    // 木上で共通の祖先を高速に求める
    struct LowestCommonAncestor
    {
        // ノードごとの子供を vector で渡す、循環が存在しないこと
        LowestCommonAncestor(const std::vector<std::vector<int>> &children, const int root) :
            children(children), root(root) {
    #ifdef DEBUG
            // 循環が存在しないこと、 ルートを子供に持つ要素がないことをチェックする
            for (int p = 0; p < static_cast<int>(children.size()); p++) {
                for (int child : children[p]) {
                    assert(0 <= child && child < static_cast<int>(children.size()));
                    assert(child != root);
                    assert(find(children[child].begin(), children[child].end(), p) == children[child].end());
                }
            }
    #endif
            assert(0 <= root && root < static_cast<int>(children.size()));
            calc_parent_and_depth();
            calc_parent_pow2();
        };

        // u, v の共通の祖先を求める
        int solve(int u, int v) {
            // make sure depth(u) > depth(v).
            if (depth[u] < depth[v]) swap(u,v);
            for (size_t k = 0; k < parent_pow2.size(); k++){
                if(((depth[u] - depth[v]) >> k) & 1){
                    u = parent_pow2[k][u];
                }
            }

            if (u == v) return u;
            for (int k = static_cast<int>(parent_pow2.size()) - 1; k >=0; k--) {
                if (parent_pow2[k][u] != parent_pow2[k][v]) {
                    u = parent_pow2[k][u];
                    v = parent_pow2[k][v];
                }
            }
            return parent_pow2[0][u];
        }

        // u, v の距離を求める
        int distance(int u, int v)
        {
            int lca = solve(u, v);
            // root から u の距離　+ root から v の距離 - 2 * 共通祖先への距離
            return depth[u] + depth[v] - 2 * depth[lca];
        }

    private:
        void calc_parent_and_depth() {
            parent = vector<int>(children.size(), -1);
            depth = vector<int>(children.size(), -1);
            sub_calc_parent_and_depth(root, -1, 0);
        }
        void sub_calc_parent_and_depth(int cur, int par, int dep){
            parent[cur] = par;
            depth[cur] = dep;
            for (int child : children[cur]) {
                if (child != par) {
                    sub_calc_parent_and_depth(child,cur,dep+1);
                }
            }
        }
        void calc_parent_pow2() {
            // parent_pow2[k][i] = 2^k parent of node i.
            parent_pow2 = vector<vector<int>>(ceil(log(children.size())/log(2)),
                                            vector<int>(children.size(),-1));
            parent_pow2[0] = parent;
            for (size_t k = 0; k + 1 < parent_pow2.size(); k++) {
                for (size_t v = 0; v < children.size(); v++) {
                    if (parent_pow2[k][v] >= 0) {
                        parent_pow2[k+1][v] = parent_pow2[k][parent_pow2[k][v]];
                    }
                }
            }
        }
        std::vector<std::vector<int>> children;
        int root;
        // if root,parent is -1.
        vector<int> parent;
        vector<int> depth;
        vector<vector<int>> parent_pow2;
    };

