######################
ゲーム
######################

****************************************
Nim
****************************************

いくつかのコインの山がある。この中からプレイヤーは山を一つ選び、1個以上の任意の数のコインを取る。
最後のコインを取ったプレイヤーが勝ちである。この問題に対しては以下のことが知られている。
すべての山のxorをとったとき、それが0であるとき、後攻の勝ち、それ以外のときは先攻の勝ち。
これは次のことから言える。

- 全ての山が 0 の時、 xor は 0 である
- 現在の状態が xor 0 の時、 次の操作で xor を 0 にすることはできない
- 現在の状態が xor 0 以外の時、 次の操作で xor を 0 にすることができる

.. code-block:: cpp

    #include <iostream>
    #include <vector>

    using namespace std;

    int main(){
        int xor_sum = 0;
        vector<int> coins = {1,2,4,5,6};
        for(size_t i=0;i<coins.size();i++){
            xor_sum = xor_sum ^ coins[i];
        }
        if(xor_sum != 0) cout << "First Player Win" << endl;
        else cout << "Second Player Win" << endl;
    }

*****************************************
Grundy 数
*****************************************

Nim を拡張し、コインの山ではなくなんらかのゲームがいくつかあり、その一つを選び操作をする。
例えば「A1, A2 , ... An が与えられ、どれかを選んでそれ自身ではないその約数に変更する。最初に操作が行えなくなった法の負け」
- https://atcoder.jp/contests/abc368/tasks/abc368_f

このような問題を解く際には一つのゲームの盤面を Grundy 数を使ってエンコードする。この Grundy 数は以下の性質を持つ。

- その状態からどこにも遷移できない (負けの状態) のなら 0
- 遷移できる状態の Grundy 数を x1, x2, ... xn としたとき、 それに含まれない最小の非負整数

いくつかあるゲームの Grundy 数を全て求めたらその xor 和が 0 なら初手が負ける、それ以外なら初手が勝てる。

- そのほかの例: https://atcoder.jp/contests/typical90/tasks/typical90_ae

実装面では多くの場合は grundy 数を再帰的な関数か、 DP などで計算することになる。


.. code-block:: cpp

    // https://atcoder.jp/contests/abc368/tasks/abc368_f
    // A -> A そのものではないその約数に変更する
    int n; cin >> n;
    vector<ll> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    vector<int> grundy(*max_element(all(a)) + 1);
    grundy[0] = grundy[1] = 0; // 遷移できない
    for (int i = 2; i < grundy.size(); i++) {
        // 遷移できる先を計算する
        // 少なくともそれ自身で破ることで 1 (grundy 数 0) に遷移できる
        set<int> transition = { 0 };
        for (int c = 2; c*c <= i; c++) {
            // i が c で割れるので c に遷移できるし、 i/c にも遷移できる
            if (i % c == 0) {
                transition.insert(grundy[c]);
                transition.insert(grundy[i/c]);
            }
        }
        // 遷移先に現れない最小の値を計算する
        int r = 0;
        bool filled = false;
        for (auto t : transition) {
            if (t != r) {
                grundy[i] = r;
                filled = true;
                break;
            }
            r++;
        }
        if (!filled) {
            grundy[i] = r;
        }
    }

    // grundy 数の xor をとって、もし 0 なら初手が負ける
    int grundy_xor_sum = 0;
    for (int i = 0; i < n; i++) {
        grundy_xor_sum ^= grundy[a[i]];
    }
    bool first_lose = grundy_xor_sum == 0;

    if (first_lose) {
        cout << "Bruno" << endl;
    } else {
        cout << "Anna" << endl;
    }


****************************************
ゲーム木探索
****************************************

TODO

******************
連結性判定
******************

大域的な連続判定はもう少し複雑だが、 3x3 のローカルグリッドにおいて連結性が変わるかどうかを判定するのは容易に可能
https://www.terry-u16.net/entry/ahc039#連結性判定
(テスト中)

.. code-block:: cpp

    // 3x3 のグリッドにおいて、中心の点を変更しても連結状況が変わらないかを返す
    // grid は 9 bit で表すことにする 例: 0b00111101 = xxo ooo oxo
    // ref: https://www.terry-u16.net/entry/ahc039#連結性判定
    bool can_flip_center_without_loosing_connection(uint32_t grid)
    {
        static bitset<(1 << 9)> res;
        static std::once_flag once;
        constexpr int CENTER = 4;

        auto can_reach = [](uint32_t grid, int index) -> uint32_t {
            static int dx[] = {-1, 0, 1, 0};
            static int dy[] = {0, 1, 0, -1};

            uint32_t res = (1 << index);
            stack<int> sta;
            sta.push(index);

            while (!sta.empty()) {
                const int cur = sta.top();
                const int y = cur / 3;
                const int x = cur % 3;
                sta.pop();

                for (int i = 0; i < 4; i++) {
                    const int ny = y + dy[i];
                    const int nx = x + dx[i];
                    if (0 <= ny && ny < 3 && 0 <= nx && nx < 3) {
                        const int ni = 3 * ny + nx;
                        bool have_mark = grid & (1 << ni);
                        bool not_used_yet = (res & (1 << ni)) == 0;
                        if (have_mark && not_used_yet) {
                            res |= (1 << ni);
                            sta.push(ni);
                        }
                    }
                }
            }
            return res;
        };
        std::call_once(once, [&]() {
            for (uint32_t g = 0; g < (1 << 9); g++) {
                bool ok = true;
                const uint32_t flipped = g ^ (1 << CENTER);
                for (int i = 0; i < 9; i++) {
                    if (i == CENTER) continue;
                    if (!(g & (1 << i))) continue;
                    auto a = can_reach(g, i);
                    auto b = can_reach(flipped, i);
                    a &= ~(1u << CENTER);
                    b &= ~(1u << CENTER);
                    if (a != b) ok = false;
                }
                res[g] = ok;
            }
        });

        return res[grid];
    }

    bool can_flip_center_without_loosing_connection(const vector<vector<bool>>& grid)
    {
        assert(grid.size() == 3);
        assert(all_of(all(grid), [](const vector<bool>& row) {return row.size() == 3;}));

        uint32_t bits = 0;
        for (int r = 0; r < 3; r++) {
            for (int c = 0; c < 3; c++) {
                if (grid[r][c]) {
                    bits |= (1 << (3 * r + c));
                }
            }
        }
        return can_flip_center_without_loosing_connection(bits);
    }