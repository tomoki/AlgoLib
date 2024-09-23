######################
文字列操作
######################

stringをincludeする。cctypeもいるかも。

*********************
std::string
*********************

charをラップしたテンプレートクラス。 基本的な使い方について

部分列
====================

.. code-block:: cpp

              //0123456789
    string str("abcdefghij");
    // 5番目以降
    str.substr(5);    // "fghij"
    // 5番目から3つ
    str.substr(5,3); // "fgh"
    //全部小文字にする
    transform(s.begin(),s.end(),s.begin(),::tolower);

substrは一つの引数でそこから先全部、二つの場合は第一引数の位置から、第二
引数の数だけ持ってくる。

検索
====================

stringには、いくつかのfindが定義されている。線形検索なので、早い検索が必
要なときには後述するKMP法やBM法を用いること。

-   find 引数が最初に現れる位置

-   rfind 引数が最後に表われる位置

-   find_first_of 引数の文字のどれかが最初に表われる位置

-   find_last_of 引数の文字のどれかが最後に表われる位置

-   find_first_not_of
    引数の文字のどれかではない文字が最初に表われる位置

-   find_first_not_of
    引数の文字のどれかではない文字が最後に表われる位置

第二引数として探すための最初の位置を指定できる。

Boyer Moore法
------------------

TBD

KMP法
--------------------

TBD

*********************
stringstream
*********************

cinやcoutのようなstreamをstringを元に作成したりする。基本的には、string
をフォーマットしたり、intやlongに、intやlongから変換するために使用する。

.. code-block:: cpp

    stringstream ss;
    ss << 102;
    string s;
    ss >> s;

*********************
再帰下降構文解析
*********************

BNFを書いて、それにしたがっていく。左再帰除去を忘れずに。

.. literalinclude:: cpp/parse.cpp

**************************************
LL1文法(== 再帰下降で書けるかどうか)
**************************************

予測型構文解析表を埋めて重複をチュックする。テストしたい。
一般に、"X -> Xb,X -> a"なる生成規則は、"X->aX',X'->bX',X'->"とできる。
これは前者がab*なる形の文字列を導出するからである。

.. literalinclude:: cpp/check_ll1.cpp

*********************
Z algorithm
*********************

文字列のすべての部分文字列に対し、それと全体の共通接頭文字列の長さのテーブルを作る。 O(N)

.. literalinclude:: cpp/zalgorithm.cpp

*********************
Rolling Hash
*********************

.. code-block:: cpp

    // TODO: string 以外にも対応する
    struct RollingHash {
        // ハッシュを外から与えるコンストラクタ、複数の文字列でハッシュを共有したい時はこちら
        // RollingHash(t, other_rollinghash.r()) のように使う
        RollingHash(std::string s, const uint64_t exp_r)
        : array(std::move(s)),
        length(array.size()),
        exp_r(exp_r)
        {
            calc_pows();
            calc_hash();
        }
        // ハッシュを自動的に決定するコンストラクタ、一つの文字列内で部分文字列のハッシュを比較する場合はこちら
        explicit RollingHash(std::string s)
            : RollingHash(std::move(s), decide_exp_r()) {}

        // 半開区間 [l, r) のハッシュ値を求める
        uint64_t hash(int l, int r)
        {
            assert(l <= r);
            assert(0 <= l && l <= static_cast<int>(length));
            assert(0 <= r && r <= static_cast<int>(length));
            // return pre_hash[r] - pre_hash[l];
            uint64_t sub = (pows[r-l] * pre_hash[l]) % MOD;
            if (pre_hash[r] >= sub) {
                return (pre_hash[r] - sub) % MOD;
            } else {
                return (pre_hash[r] + MOD - sub) % MOD;
            }
        }
        // [l1, r1) == [l2, r) かどうかチェックする
        // ハッシュしかチェックしないので外す可能性もある。
        uint64_t probably_equal(int l1, int r1, int l2, int r2)
        {
            assert(l1 <= r1 && l2 <= r2);
            if (r1 - l1 != r2 - l2) return false; // 長さが違ったら false
            if (l1 == l2 && r1 == r2) return true;
            if (hash(l1, r1) != hash(l2, r2)) return false;
            return true;
        }
        // [l1, r1) == [l2, r) かどうかチェックする
        // ハッシュが一致した時に完全な文字列比較を行う
        uint64_t exactly_qual(int l1, int r1, int l2, int r2)
        {
            if (!probably_equal(l1, r1, l2, r2)) return false;
            for (int i = 0; i < (r1-l1); i++) {
                if (array[l1+i] != array[l2+i]) return false;
            }
            return true;
        }
        uint64_t r() { return exp_r; }
    private:
        void calc_pows()
        {
            pows.resize(length+1);
            pows[0] = 1;
            for (size_t i = 1; i <= length; i++) {
                pows[i] = (pows[i-1] * exp_r) % MOD;
            }
        }
        void calc_hash()
        {
            pre_hash.resize(length+1);
            pre_hash[0] = 0;
            for (size_t i = 1; i <= length; i++) {
                pre_hash[i] = ((exp_r * pre_hash[i-1]) % MOD + to_num(array[i-1])) % MOD;
            }
        }
        uint64_t to_num(char c)
        {
            return c;
        }
        // ハックを防ぐために exp_r は乱数にしておく
        static uint64_t decide_exp_r()
        {
            std::random_device seed_gen;
            std::mt19937_64 mt64(seed_gen());
            std::uniform_int_distribution<uint64_t> get_rand_uni_int(2, MOD-2);
            return get_rand_uni_int(mt64);
        }
        static constexpr uint64_t MOD = 1000000007;
        static_assert(MOD < (1ull << 32));
        // hash[i] = [0..i) の hash = array[0] * r^(i-1) + array[1] * r^(i-2) + ... array[i-1] * r^0.
        std::vector<uint64_t> pre_hash;
        // pows[i] = r^i
        std::vector<uint64_t> pows;
        // ハッシュ衝突時チェック用にコピーしておく
        std::string array;
        size_t length;
        uint64_t exp_r;
    };

.. code-block:: cpp

    // https://atcoder.jp/contests/tessoku-book/tasks/tessoku_book_bd
    int my_main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
    {
        int n, q; cin >> n >> q;
        string s; cin >> s;

        std::array<RollingHash, 2> hash = { RollingHash(s), RollingHash(s)};

        rep(_, q)
        {
            int a, b, c, d; cin >> a >> b >> c >> d;
            a--; b--; c--; d--;
            bool all_match = true;
            repv(h, hash) all_match &= h.probably_equal(a, b+1, c, d+1);
            cout << (all_match ? "Yes" : "No") << endl;
        }
        return 0;
    }


*******************
Suffix Array
*******************

.. code-block:: cpp

    // 接尾辞をソートして保持する
    // 空文字列を含む
    // 実装は Manber & Myers の O(n logn^2) アルゴリズム
    // 例: missisippi
    // sa[0]  = 10 φ
    // sa[1]  = 9 i
    // sa[2]  = 6 ippi
    // sa[3]  = 4 isippi
    // sa[4]  = 1 issisippi
    // sa[5]  = 0 missisippi
    // sa[6]  = 8 pi
    // sa[7]  = 7 ppi
    // sa[8]  = 5 sippi
    // sa[9]  = 3 sisippi
    // sa[10] = 2 ssisippi
    struct SuffixArray {
        explicit SuffixArray(std::string s)
            : m_string(std::move(s)),
              m_size(m_string.size()),
              m_rank(m_size+1),
              m_prefix_index(m_size+1)
        {
            const int n = static_cast<int>(m_size);
            m_rank[n] = -1; // n のケースは空文字列扱いで一番早いことにする
            m_prefix_index[n] = n;
            for (int i = 0; i < n; i++) {
                m_prefix_index[i] = i;
                m_rank[i] = static_cast<int>(m_string[i]); // 文字コードをランクにする
            }
            // k 文字のランクが計算されていたとして、　k*2 文字のランクを計算する
            for (int k = 1; k <= n; k *= 2) {
                // (rank[i], rank[i+k]), (rank[j], rank[j+k]) から k*2 文字の比較を行う関数
                auto compare = [this, k, n](int i, int j) {
                    // 最初の k 文字ですでに順序がつく場合はそれを採用
                    if (m_rank[i] != m_rank[j]) return m_rank[i] < m_rank[j];
                    // 次の k 文字で比較する。もしオーバーしている (空文字列) ならそれを優先する。
                    const int ri = (i+k) <= n ? m_rank[i+k] : -1;
                    const int rj = (j+k) <= n ? m_rank[j+k] : -1;
                    return ri < rj;
                };
                std::sort(m_prefix_index.begin(), m_prefix_index.end(), compare);
                std::vector<int> tmp_rank(n+1);
                // 空文字列のランクは 0 にして
                tmp_rank[m_prefix_index[0]] = 0;
                // それを基準に一致していたら +0、文字列が一致していなかったら +1 していく。
                // suffix_array はソート済みのため、 compare が false を返すケースは一致しているケース
                for (int i = 1; i <= n; i++) {
                    tmp_rank[m_prefix_index[i]] = tmp_rank[m_prefix_index[i-1]] + (compare(m_prefix_index[i-1], m_prefix_index[i]) ? 1 : 0);
                }
                swap(m_rank, tmp_rank); // m_rank に中身を移す
            }
        }
        // s が文字列の中に何度現れるかを返す。 O(logn)
        // https://atcoder.jp/contests/abc362/tasks/abc362_g
        int count(const std::string& s)
        {
            if (s.size() == 0) return 0;
            return upper_bound(s) - lower_bound(s);
        }
        // 接尾時の先頭 s.size() 文字を見て、辞書式順で s 以上のものが初めて現れる場所を返す
        int lower_bound(const std::string& s)
        {
            if (s.size() == 0) return 0;
            // fail = 0 は空文字列であり、必ず s より辞書順で小さい。
            // succ = m_size+1
            int fail = 0, succ = m_size+1;
            while (abs(fail - succ) > 1) {
                int mid = (fail + succ) / 2;
                if (s <= suffix(mid).substr(0, min(s.size(), suffix(mid).size()))) succ = mid;
                else fail = mid;
            }

            return succ;
        }
        // 接尾時の先頭 s.size() 文字を見て、辞書式順で s より大きいものが初めて現れる場所を返す
        int upper_bound(const std::string& s)
        {
            if (s.size() == 0) return 1;
            // fail = 0 は空文字列であり、必ず s より辞書順で小さい。
            // succ = m_size+1
            int fail = 0, succ = m_size+1;
            while (abs(fail - succ) > 1) {
                int mid = (fail + succ) / 2;
                if (s < suffix(mid).substr(0, min(s.size(), suffix(mid).size()))) succ = mid;
                else fail = mid;
            }

            return succ;
        }
        [[nodiscard]] size_t size() const { return m_size+1; }
        size_t operator[](int i) const { return m_prefix_index[i]; }
        [[nodiscard]] std::string_view suffix(int i) const { return std::string_view(m_string).substr(m_prefix_index[i]); }
        std::vector<int>& ref() { return m_prefix_index; }

    private:
        // 念の為コピーしておく
        std::string m_string;
        size_t m_size;
        std::vector<int> m_rank;
        // その文字列が何文字目から始まっているか
        std::vector<int> m_prefix_index;
    };


*******************
ST = TS の条件
*******************

文字列 S と t が与えられる。長さ t の文字列 T であって、 ST = TS を満たすものを出力せよ。
ただし T がない可能性もある。 参考: https://atcoder.jp/contests/arc181/tasks/arc181_b

\|S\| >= 1 かつ \|T\| >= 1 の時、 ST = TS ならば g = gcd(\|S\|, \|T\|) として S および T は g の長さのパターンで構成される。

例

* \|S\| = 2, \|T\| = 3 なら gcd(2, 3) = 1 なので \|S\| および \|T\| は 1 文字の繰り返しで構成される。 S = aa T = a など。
* \|S\| = 4, \|T\| = 2 なら gcd(2, 4) = 2 なので \|S\| および \|T\| は 2 文字の繰り返しで構成される。 S = abab, T = ab など。

.. code-block:: cpp

    // 長さ t の文字列 T であり、 ST = TS を満たす文字列を返す。
    // なかったら nullopt
    // 例:
    // "A", 1 -> "A",
    // "A", 4 -> "AAAA"
    // "ab", 1 -> nullopt
    // "ab", 2 -> "ab"
    // "ab", 3 -> nullopt
    // "ab", 4 -> "abab"
    // "abcabc", 3 -> "abc"
    // "abcabc", 2 -> nullopt
    std::optional<std::string> solve_ST_eq_TS(const std::string& s, size_t t)
    {
        // ST = TS なる S および T は必ず gcd(|S|, |T|) の長さのパターンの繰り返しである
        int g = std::gcd(s.size(), t);
        // もしパターンの条件を S が満たさないなら nullopt
        for (int i = 0; i + g < s.size(); i++) {
            if (s[i] != s[i+g]) return std::nullopt;
        }
        const std::string pattern = s.substr(0, g);
        std::string ret;
        while (ret.size() < t) ret += pattern;
        return ret;
    }


説明:
\|S\| <= \|T\| とする。この時 T の先頭 \|S\| 文字は S と一致するので、 T = S + A 書くことができる。

* ST = SSA
* TS = SAS

以上で SA = AS なる似た問題が出てくる。 もし \|A\| >= \|S\| なら同様に A = S + B とかくことができる。

* ST = SSA = SSSB
* TS = SAS = SSBS

この手続きを \|S\| > \|C\| となるまで繰り返す。

* ST = SSSSSSC
* TS = SSSSSCS

さらに SC = CS という似た問題で S = C + D とかく、ことを繰り返す。

以上の変形を数式で表すことにすると T = q * S + C であった。 q は \|T\| / \|S\|、 \|C\| は　\|T\| % \|S\| である。
この時 \|S\| と \|T\| の最大公約数を g とし \|X\| と \|Y\| が互いに素な文字列 X, Y, Z があると

* T = g * X
* S = g * Y
* C = g * Z (※)

と表すことができる。 つまり T および S は長さ g = gcd(\|S\|, \|T\|) のパターンの繰り返しである。

(※) \|T\| - \|S\| = g\|X\| - g\|Y\| = g(\|X\| - \|Y\|) となるが、 この時 (\|X\| - \|Y\|) と \|X\| は互いに素であるため \|T\| および \|S\| と、 \|T\| - \|S\| は最大公約数 g を持つ。
これを繰り返すと　\|T\| - \|S\| - \|S\| = g (x - y - y) は \|T\| - \|S\| と最大公約数 g を持つので、結局 \|T\| - q \|S\| = \|C\| は \|S\| および \|T\| と最大公約数　g を持つ。

以上の計算はユークリッドの互除法と同じことをしている。
