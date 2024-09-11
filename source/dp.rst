###################################################
動的計画法的なやつ
###################################################

****************************************
Longest Common Sequence (LCS)
****************************************
O(NM)

.. code-block:: cpp

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

****************************************
Longest Increasing Subsequence (LIS)
****************************************

与えられた数列の最長増加増加部分列 (部分列であって、その要素が単調に増加するもの) を計算する。

.. code-block:: cpp

    // https://atcoder.jp/contests/abc354/editorial/10027
    // O(n log n) で最長増加部分列を求める。 strict = true なら同じ値は許さない、 false なら同じ値も許す
    template<typename T>
    std::pair<size_t, std::vector<size_t>> longest_increasing_sequence(const std::vector<T>& v, const bool strict=true) {
        static_assert(std::is_signed_v<T> , "T should be signed value"); // 符号付きだと dp 表の番兵と被る可能性があるので許さないことにしておく
    #ifdef DEBUG
        for (const auto& t : v) assert(t != std::numeric_limits<T>::min());
    #endif

        // 長さの計算と一緒に、その要素がどの長さの場合を更新したかを覚えていく
        std::vector<size_t> updated_index(v.size());
        // dp[x] = x の長さを実現した際の数列の最後尾の値
        std::vector<T> dp(v.size()+1, std::numeric_limits<T>::max());
        dp[0] = std::numeric_limits<T>::min();
        for (int i = 0; i < v.size(); i++) {
            auto it = strict ? std::lower_bound(all(dp), v[i]) : std::upper_bound(all(dp), v[i]);
            *it = std::min(v[i], *it);
            updated_index[i] = it - dp.begin();
        }
        size_t length = std::lower_bound(all(dp), std::numeric_limits<T>::max()) - dp.begin() - 1;
        return {length, updated_index };
    }

    void lis_example()
    {
        std::vector<int> v = {1, 3, 4, 2, 1, 5};
        auto [length, updated_index] = longest_increasing_sequence(v, false);
        // 最長増加部分列を実現する index
        vector<int> lis_index;
        {
            int cur_len = length;
            for (int i = static_cast<int>(updated_index.size()) -1; i >= 0; i--) {
                if (cur_len == updated_index[i]) {
                    lis_index.push_back(i);
                    cur_len--;
                }
            }
            reverse(all(lis_index));
        }
    }

****************************************
巡回セールスマン問題
****************************************

https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DPL_2_A

ある点からスタートして、全ての点を辿って最初の点に戻る時の最短距離を計算する。

.. code-block:: cpp

    // dp[a][b] = 現在 b にいて、 集合 a をすでに辿っている時の最短距離
    vector<vector<int>> dp(1 << v, vector<int>(v, INF));
    // スタート地点にいて、かつ辿った集合なしの状態を距離 0 にする。
    // この時辿った集合に s を入れないことによって最後に s に戻ってくるようにする。
    dp[0][s] = 0;
    for (uint32_t g = 0; g < (1 << v); g++) {
        // dp[g][p] から配る DP で遷移
        for (int p = 0; p < v; p++) {
            for (const auto& e : graph[p]) {
                if (g & (1 << e.to)) continue; // すでに遷移済みならスキップ
                dp[g | (1 << e.to)][e.to] = min(dp[g | (1 << e.to)][e.to], dp[g][p] + e.value);
            }
        }
    }
    int ret = dp[(1 << v)-1][s];

****************************************
部分集合の列挙
****************************************

https://atcoder.jp/contests/typical90/tasks/typical90_as より。

bit で集合を表すことにしたとき、 その空集合を除く部分集合の列挙は以下のような for 文で行うことができる。

.. code-block:: cpp

    uint32_t g = 0b0110111;
    for (uint32_t a = g; a != 0; a = (a - 1) & g) {
        for (int i = 0; i < 32; i++) {
            cout << ((a & (1 << i)) ? 1 : 0);
        }
        cout << endl;
    }

これを利用して、部分集合の部分集合を列挙して行って bit DP を行うことがある。 以下は全体を k 個の部分集合に分割するDP。 O(k*3^n)。

.. code-block:: cpp

    vector<vector<ll>> dp(k + 1, vector<ll>(1 << n, 1ll << 62));
    dp[0][0] = 0;

    for (int cnt = 1; cnt <= k; cnt++) {
        for (int g = 1; g < (1 << n); g++) {
            for (int a = g; a != 0; a = (a - 1) & g) {
                chmin(dp[cnt][g], max(dp[cnt-1][g-a], max_dist[a]));
            }
        }
    }

****************************************
ナップサック問題
****************************************
個数制限なしのとき

.. code-block:: cpp

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

****************************************
ヒストグラム内の最大長方形のサイズ
****************************************

.. literalinclude:: cpp/hist.cpp

****************************************
なんか長方形の和をもとめるやつ
****************************************

.. literalinclude:: cpp/2d_area.cpp

****************************************
累積和
****************************************


一次元
===============

.. code-block:: cpp

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

二次元
===============

.. code-block:: cpp

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

ダブリング
==================

.. code-block:: cpp

    // one に対し、 x で表される binary_op を n 回適用する
    template<typename T>
    T apply_doubling(const T& x, const uint64_t n, const std::function<T(T, T)>& binary_op, const T& one)
    {
        if (n == 0) return one;
        T ret = apply_doubling(binary_op(x, x), n/2, binary_op, one);
        if (n % 2 == 1) ret = binary_op(ret, x);
        return ret;
    }

    // trans で表されるような遷移を n 回適用する
    // https://atcoder.jp/contests/abc167/tasks/abc167_d
    std::vector<int> apply_trans_vector(const uint64_t n, const std::vector<int>& trans)
    {
        std::vector<int> init(trans.size());
        iota(init.begin(), init.end(), 0);
        return apply_doubling<vector<int>>(trans, n, [n = trans.size()](auto x, auto y) {
            vector<int> ret(n);
            for (size_t i = 0; i < n; i++)
                ret[i] = x[y[i]];
            return ret;
        }, init);
    }

    // n 回 trans で遷移した時の場所とコストを計算する
    // trans[0].first = 行き先、 trans[0].second = コスト
    // https://atcoder.jp/contests/abc370/tasks/abc370_f
    template<typename Cost>
    std::vector<pair<int, Cost>> apply_trans_vector2(const uint64_t n, std::vector<pair<int, Cost>> trans)
    {
        std::vector<pair<int, Cost>> init(trans.size());
        for (size_t i = 0; i < init.size(); i++) {
            init[i].first = i;
            init[i].second = 0;
        }

        return apply_doubling<std::vector<pair<int, Cost>>>(trans, n, [n = trans.size()](const auto& f, const auto& x) {
            std::vector<pair<int, Cost>> ret(n);
            for (size_t i = 0; i < n; i++) {
                ret[i].first = f[x[i].first].first;
                // ここの式がややこしいが、 現在の値 x を f で移すイメージがわかりやすい
                // 現在値 x[i].first から生えているエッジが f[x[i].first] なのでそのコストを現在の値に追加する
                ret[i].second = x[i].second + f[x[i].first].second;
            }
            return ret;
        }, init);
    }

    // x を n 乗する
    template<typename T, typename R>
    constexpr T powi2(T x, R n, T one = 1) {
        return apply_doubling(x, n, std::multiplies<T>(), one);
    }