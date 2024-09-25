######################
データ構造
######################

****************************************
Union-Find
****************************************

グループを管理するデータ構造、グループの統合や同じグループにいるかの判定が高速に可能

.. code-block:: cpp

    struct UnionFind {
        explicit UnionFind(int n)
        : data(n, -1)
        , number_of_group_var(n) { }
        // 親を探す
        int root(int x) {
            if (data[x] < 0) return x;
            return data[x] = root(data[x]);
        }
        // x,yの含まれる集合を併合する
        // 併合が発生したら true, しなかったら false
        bool unite(int x,int y) {
            x = root(x);
            y = root(y);
            if (x == y) return false; // すでに同じところにいる
            // 大きいほうに追加する。
            if(data[y] < data[x]) swap(x,y);
            data[x] += data[y];
            data[y] = x;
            number_of_group_var--;
            return true;
        }
        // 同じ集合にいるかどうか
        bool is_same_group(int x, int y) {
            return root(x) == root(y);
        }
        [[nodiscard]] int number_of_group() const {
            return number_of_group_var;
        }
        int group_size(int x) {
            return -data[root(x)];
        }
    private:
        // data は親ならはそのグループのサイズに -1 をかけたもの、子なら親の値
        vector<int> data;
        int number_of_group_var;
    };


****************************************
ヒープ
****************************************

.. code-block:: cpp

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

****************************************
bitset
****************************************
限られた大きさのvector\<bool\>を使いたいときに、bitsetを使うことができる。

.. code-block:: cpp

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

****************************************
分数
****************************************
テストちゅう.

.. code-block:: cpp

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

****************************************
セグメント木
****************************************

普通の
====================
RMQ (POJ 3264)

.. literalinclude:: cpp/segment_tree.cpp

Lazy Segment Tree
====================

.. code-block:: cpp

    // T = 各要素および区間取得の結果の型
    // op = T(T, T), 区間取得を行う関数 (例えば RMQ なら min)
    // e = T(), op における単位元
    // F = "Lazy" 遅延している操作の型
    // mapping = T(F, T) 遅延している操作 F を区間取得の結果 T に対し行う関数
    // composition = F(F f, F g)、遅延している操作 g に対し、追加で f を行った時に生成する遅延操作を生成する
    //   f(g(x)) となるので g がさきに行われる処理
    // id = F(), composition の恒等写像 (遅延している操作の単位元)
    // F(op(x, y)) = op(F(x), F(y)) を満たす必要がある
    template<typename T, auto op, auto e, typename F, auto mapping, auto composition, auto id>
    class LazySegmentTree {
        static_assert(std::is_convertible_v<decltype(op), std::function<T(T, T)>>, "op should be T(T, T)");
        static_assert(std::is_convertible_v<decltype(e), std::function<T()>>, "e should be T()");
        static_assert(std::is_convertible_v<decltype(mapping), std::function<T(F, T)>>, "mapping should be T(F, T)");
        static_assert(std::is_convertible_v<decltype(composition), std::function<F(F, F)>>, "composition should be F(F, F)");
        static_assert(std::is_convertible_v<decltype(id), std::function<F()>>, "id should be F()");

    public:
        explicit LazySegmentTree(int _n)
        {
            n = 1;
            while (n < _n) n *= 2;
            data = vector<T>(2 * n - 1, e());
            lazy = vector<F>(2 * n - 1, id());

        }
        template<class InputIterator>
        explicit LazySegmentTree(InputIterator first, InputIterator last) : LazySegmentTree(distance(first, last))
        {
            size_t i = 0;
            for (auto it = first; it != last; ++it, i++) {
                update(i, *it);
            }
        }
        void update(int i, T v)
        {
            int k = i + n - 1;
            data[k] = v;
            while (k > 0) {
                k = get_parent_index(k);
                auto left = data[get_left_index(k)];
                auto righ = data[get_right_index(k)];
                data[k] = op(left, righ);
            }
        }
        // [a, b) に操作 f を行う
        void apply(int a, int b, F f)
        {
            assert(0 <= a && a <= b && b <= n);
            return apply(a, b, f, 0, 0, n);
        }

        // [a, b) 半開区間の値を取得する
        [[nodiscard]] T get(int a, int b)
        {
            assert(0 <= a && a < b && b <= n); // FIXME: should check the original n?
            // ノード 0 の管理区間は [0, n)
            return get(a, b, 0, 0, n);
        }
        [[nodiscard]] size_t size() const { return n; }
    private:
        static int get_left_index(int k) { return 2 * k + 1; }
        static int get_right_index(int k) { return 2 * k + 2; };
        static int get_parent_index(int k) { return (k - 1) / 2; }
        // k 番目のノードの遅延操作を反映する
        void apply_lazy(int k, int l, int r)
        {
            if (lazy[k] == id()) return;
            data[k] = mapping(lazy[k], data[k]);
            // 子供に遅延評価を伝搬する
            if (r - l > 1) {
                const int left_index = get_left_index(k);
                const int right_index = get_right_index(k);
                lazy[left_index] = composition(lazy[k], lazy[left_index]);
                lazy[right_index] = composition(lazy[k], lazy[right_index]);
            }
            lazy[k] = id();
        }

        // l, r はノード k の管理する区間
        [[nodiscard]] T get(int a, int b, int k, int l, int r)
        {
            if (r <= a || b <= l) return e();
            apply_lazy(k, l, r);
            if (a <= l && r <= b) return data[k];
            const int left_index = get_left_index(k);
            const int right_index = get_right_index(k);
            const auto left = get(a, b, left_index, l, (l+r)/2);
            const auto right = get(a, b, right_index, (l+r)/2, r);
            return op(left, right);
        }
        void apply(int a, int b, F f, int k, int l, int r)
        {
            apply_lazy(k, l, r);
            if (r <= a || b <= l) return;
            if (a <= l && r <= b) {
                lazy[k] = composition(f, lazy[k]);
                apply_lazy(k, l, r);
            } else {
                apply(a, b, f, get_left_index(k), l, (l+r)/2);
                apply(a, b, f, get_right_index(k), (l+r)/2, r);
                data[k] = op(data[get_left_index(k)], data[get_right_index(k)]);
            }
        }

        int n = 0;
        vector<T> data;
        vector<F> lazy;
    };

範囲セット、範囲の和の計算をするセグメント木
----------------------------------------------------

.. code-block:: cpp

    // https://atcoder.jp/contests/abc371/tasks/abc371_f
    struct Group {
        long long value; // ノードの合計値
        long long size; // ノードに含まれる要素の数
    };

    ostream& operator<<(ostream& os, const Group& g)
    {
        return os << "{" << "," << g.value << "," << g.size << "}";
    }

    using T = Group;
    auto op = [](const T& a, const T& b) {
        return T {a.value + b.value, a.size + b.size};
    };
    auto e = []() { return T {0, 0}; };
    using F = optional<long long>;

    auto mapping = [](const F& f, const T& g) {
        if (!f.has_value()) return g;
        // ノードに対するアップデートであるため、その範囲の合計値計算にサイズが必要
        return T {f.value() * g.size, g.size };
    };
    auto composition = [](const F& f, const F& g) {
        return f;
    };

    auto id = []() -> F { return std::nullopt; };
    LazySegmentTree<T, op, e, F, mapping, composition, id> lst(n);

    // 初期化はサイズ 1 の要素アップデートとして行う
    for (int i = 0; i < n; i++) lst.update(i, Group {x[i], 1});


範囲への足し算、範囲の和の計算をするセグメント木
----------------------------------------------------
.. code-block:: cpp

    // https://atcoder.jp/contests/abc371/tasks/abc371_f
    struct Group {
        long long value; // ノードの合計値
        long long size; // ノードに含まれる要素の数
    };

    using T = Group;
    auto op = [](const T& a, const T& b) {
        return T {a.value + b.value, a.size + b.size};
    };
    auto e = []() { return T {0, 0}; };
    using F = optional<long long>;

    auto mapping = [](const F& f, const T& g) {
        if (!f.has_value()) return g;
        // ノードに対するアップデートであるため、その範囲の合計値計算にサイズが必要
        return T {f.value() * g.size + g.value, g.size };
    };
    auto composition = [](const F& f, const F& g) {
        if (g.has_value())
            return f.value() + g.value();
        return f.value();
    };
    auto id = []() -> F { return std::nullopt; };
    LazySegmentTree<T, op, e, F, mapping, composition, id> lst(n);

    // 初期化はサイズ 1 の要素アップデートとして行う
    for (int i = 0; i < n; i++) lst.update(i, Group {x[i], 1});


****************************************
Binary-Indexed Tree (FenwickTree)
****************************************

オペレーションとしては SegmentTree の下位互換だが、高速な構造。

.. code-block:: cpp

    // SegmentTree はモノイドであれば良かったが、それに加えて可換であることと逆元 inv が必要になる。
    // 例: 値への加算と部分集合の和を計算する
    // const auto op = [](int a, int b) {return a+b; };
    // const auto e = []() { return 0; };
    // const auto inv = [](int a) { return -a;};
    // FenwickTree<int, op, e, inv> ft(5);
    template<typename T, auto op, auto e, auto inv>
    class FenwickTree {
        static_assert(std::is_convertible_v<decltype(op), std::function<T(T, T)>>, "op must work as T(T, T)");
        static_assert(std::is_convertible_v<decltype(e), std::function<T()>>, "e should be T()");
        static_assert(std::is_convertible_v<decltype(inv), std::function<T(T)>>, "inv must work as T(T)");
    public:
        explicit FenwickTree(int _n) : original_n(_n) { data = vector<T>(_n + 1); }

        // a[i] に　v を加算 (op) する。 (a[i] = op(a[i], v)
        void add(int i, T v)
        {
            assert(0 <= i && i < original_n);
            i++; // 1-indexed に変換
            for (size_t j = i; j < data.size(); j += (j & -j)) {
                data[j] = op(data[j], v);
            }
        }
        // [a, b) 半開区間の値を取得する、 a == b なら e() を返す
        [[nodiscard]] T get(int a, int b) const
        {
            assert(0 <= a && a <= b && b <= original_n);
            return op(get(b), inv(get(a)));
        }
        // [0, b) 半開区間の値を取得する、 b == original_n なら　e() を返す
        [[nodiscard]] T get(int b) const
        {
            assert(b <= original_n);
            int a = 1; // a を 1-indexed に。
            // b++, b--; b を 1-indexed に、 さらに閉区間に。
            T ret = e();
            for (int j = b; j > 0; j -= (j & -j)) {
                ret = op(ret, data[j]);
            }
            return ret;
        }
        vector<T> data;
        size_t original_n;
    };

****************************************
定数個のみを保持する priority_queue
****************************************
ビームサーチとかに使える？

.. literalinclude:: cpp/lens_queue.cpp

****************************************
Set で区間を管理するテクニック
****************************************

.. code-block:: cpp

    // 「区間を set で管理するテクニック」
    // 半開区間 [a, b) を pair(a, b) として set にいれる。
    // ただし set の中の区間は重複を持たないよう管理する
    // https://codeforces.com/contest/915/problem/E
    struct RangeSet {
        using T = int64_t;
        RangeSet()
        {
            // 番兵
            segs.emplace(std::numeric_limits<T>::min(), std::numeric_limits<T>::min());
            segs.emplace(std::numeric_limits<T>::max(), std::numeric_limits<T>::max());
        }
        // [l, r) を完全に含む区間が存在するならその区間を返す
        std::optional<std::pair<T, T>> containedBy(T l, T r)
        {
            // assert(l <= r);
            // [l, r) が含まれているとしたら一つ前のもの
            auto it = std::upper_bound(segs.begin(), segs.end(), make_pair(l, r));
            --it;
            if (it->first <= l && r <= it->second) return *it;
            return std::nullopt;
        }
        // 区間の数を返す
        size_t size()
        {
            return segs.size() - 2; // 番兵のぶんをひく
        }
        // 区間 [l, r) を追加する。区間のうち、新規に追加された要素を返す
        T insert(T l, T r)
        {
            // assert(l <= r);
            auto it = segs.upper_bound(make_pair(l, r));
            --it;
            // カバーされているので 0
            if (it->first <= l && r <= it->second) return 0;
            // 以下では l, r を書き換えていくが、この時既存の要素でカバーされていたものを "erased" とする
            // そして最後に追加された要素を (l-r) - erased で計算。
            T erased = 0;
            // 左の区間と一部被っている場合は合体する
            if (it->first <= l && l <= it->second) {
                erased += it->second - it->first;
                l = it->first;
                it = segs.erase(it);
            } else {
                ++it; // 左の区間とは被っていないので次へ
            }
            // 右の区間と共通部分があれば合体していく (it = upper_bound 相当)
            while (it->first <= r) {
                if (it->second <= r) {
                    // 新しくできた要素で完全にカバーされている
                    erased += it->second - it->first;
                    it = segs.erase(it);
                } else {
                    // l <= it->first <= r < it->second
                    erased += it->second - it->first;
                    r = it->second;
                    segs.erase(it);
                    // 既存の要素の方が r が長いのであればそれ以上に長い要素は存在しない
                    break;
                }
            }
            segs.emplace(l, r);
            return (r - l) - erased;
        }
        // 区間 [l, r) を削除する。区間のうち、削除された要素を返す
        T erase(T l, T r)
        {
            auto it = segs.upper_bound(make_pair(l, r));
            --it;
            // 完全にカバーされている場合はその区間を分割する
            if (it->first <= l && r <= it->second) {
                if (it->first != l)
                    segs.emplace(it->first, l);
                if (r != it->second)
                    segs.emplace(r, it->second);
                segs.erase(it);
                return r - l;
            }
            T erased = 0;
            // 左の区間と一部被っている場合は左の区間の削除を行う
            if (it->first <= l && l < it->second) {
                erased += it->second - l;
                if (it->first != l) {
                    segs.emplace(it->first, l);
                }
                it = segs.erase(it);
            } else {
                ++it;
            }
            // 右の区間と共通部分があれば削除していく (it = upper_bound 相当)
            while (it->first < r) {
                if (it->second <= r) {
                    // 完全に削除される
                    erased += it->second - it->first;
                    it = segs.erase(it);
                } else {
                    // l <= it->first <= r < it->second
                    erased += r - it->first;
                    segs.emplace(r, it->second);
                    segs.erase(it);
                    break;
                }
            }
            return erased;
        }
        // x 以上のカバーされていない最小の値を返す
        T mex(T x)
        {
            auto it = segs.lower_bound(make_pair(x, x+1));
            if (it->first <= x && x <= it->second) return it->second;
            return x;
        }
    private:
        std::set<std::pair<T, T>> segs;
        friend ostream& operator<<(ostream&, const RangeSet&);
    };
    ostream& operator<<(ostream& os, const RangeSet& v) {
        bool first = true;
        os << "{";
        for (auto it = v.segs.begin(); it != v.segs.end(); ++it) {
            // 最初と最後は番兵なので skip
            if (it == v.segs.begin() || it == (std::prev(v.segs.end()))) continue;
            if (!first) {
                os << ", ";
            }
            os << *it;
            first = false;
        }
        return os << "}";
    }

****************************************
ZobristHash
****************************************

集合をハッシュで管理する構造。 要素を hash 化して集合をその hash の xor で扱う。
(xor で扱う場合は集合に重複する要素を許さない。許したい場合は演算を + で行う)

- https://atcoder.jp/contests/abc250/tasks/abc250_e
- https://atcoder.jp/contests/abc367/tasks/abc367_f

なお、アップデートが必要であったりする場合は vector に関する問題なら SegmentTree を使うこともできる。 (コード参照)

.. code-block:: cpp

    // 集合の一致判定を確率的に行う
    // ローリングハッシュのように vector を受け取って、その一部分の集合のハッシュを計算する
    // ハッシュに xor を使う場合は多重集合が扱えないため、 関数を + にする必要がある。
    // 指定するハッシュ関数は T によって変わる。 T が文字列などの場合は std::hash も使えるが、 long long などの場合は自分で定義する必要がある (乱数を割り当てれば良い)
    template<typename T, bool MULTISET=false>
    struct ZobristHash {
        explicit ZobristHash(const std::vector<T>& arr, const std::function<uint64_t(T)>& myhash) : hashes(arr.size()), hashes_acc(arr.size() + 1)
        {
            for (size_t i = 0; i < arr.size(); i++) {
                hashes[i] = myhash(arr[i]);
            }
    #if DEBUG
            {
                // MULTISET ではないなら hash は (できるだけ) 全て違う必要がある。
                if (!MULTISET) {
                    assert(std::set(all(hashes)).size() == arr.size());
                }
            }
    #endif
            hashes_acc[0] = zero();
            for (size_t i = 1; i <= hashes.size(); i++) {
                hashes_acc[i] = op(hashes_acc[i-1], hashes[i-1]);
            }
        }
        // [l, r) のハッシュ値を返す
        [[nodiscard]] uint64_t hash(const size_t l, const size_t r) const
        {
            assert(l <= r);
            return op2(hashes_acc[r], hashes_acc[l]);
        }
        static uint64_t op(const uint64_t l, const uint64_t r)
        {
            if (MULTISET) return l + r;
            else          return l ^ r;
        }
        static uint64_t op2(const uint64_t l, const uint64_t r)
        {
            if (MULTISET) return l - r;
            else return l ^ r;
        }
        static uint64_t zero()
        {
            return 0;
        }
        std::vector<uint64_t> hashes;
        // i 番目の要素は 集合 [0, i) のハッシュ
        std::vector<uint64_t> hashes_acc;
    };
    template<typename T>
    using MultiZobristHash = ZobristHash<T, true>;

    int main(int argc, char **argv) {
        auto a = input_vector<int>(n);
        auto b = input_vector<int>(n);

        // 対応表になる unordered_map を作り乱数を入れておく
        std::random_device seed_gen;
        std::mt19937_64 engine(seed_gen());
        std::uniform_int_distribution<uint64_t> distribution;
        std::unordered_map<int, uint64_t> hash_map;
        auto myhash = [&hash_map, &distribution, &engine](int x) -> uint64_t {
            if (hash_map.contains(x)) return hash_map[x];
            return hash_map[x] = distribution(engine);
        };
        MultiZobristHash<int> ah(a, myhash);
        MultiZobristHash<int> bh(b, myhash);

        bool probably_same = ah.hash(la, ra+1) == bh.hash(lb, rb+1);

        // セグメントツリーを使う版。重複を許さない場合は + ではなく xor を使う。
        auto op = [](uint64_t left, uint64_t right) { return left + right; };
        auto e = []() -> uint64_t { return 0;};
        SegmentTree<uint64_t, op, e> seg_a(n);
        for (int i = 0; i < n; i++) {
            seg_a.update(i, myhash(a[i]));
        }
        SegmentTree<uint64_t, op, e> seg_b(n);
        for (int i = 0; i < n; i++) {
            seg_b.update(i, myhash(b[i]));
        }
        bool probably_same = seg_a.get(la, ra+1) == seg_b.get(lb, rb+1);
        // セグメントツリー使う版ここまで。

    }
