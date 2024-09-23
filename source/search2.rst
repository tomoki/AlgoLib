######################
探索
######################

*********************
全探索
*********************
全部しらべる。

.. code-block:: cpp

    int linear_search(vector<int> V,int val){
        repeat(i,V.size()){
            if(V[i] == val) return i;
        }
        return -1;
    }

*********************
二分探索
*********************

単調なものに関してある条件を満たす点を探す。

.. code-block:: cpp

    template<typename T>
    T binary_method(const std::function<bool(T)>& pred, T ok, T fail)
    {
        static_assert(std::is_integral_v<T> || std::is_floating_point_v<T>);
        assert(pred(ok) == true);
        assert(pred(fail) == false);
        T EPS;
        if constexpr (std::is_integral_v<T>) EPS = 1;
        else if (std::is_floating_point_v<T>) EPS = 1e-9;

        while (abs(ok - fail) > EPS) {
            T mid = std::midpoint(ok, fail);
            if (pred(mid)) ok = mid;
            else fail = mid;
        }

        return ok;
    }

対象がvectorの場合は以下のように書ける。 lower_bound, upper_bound を使える。

.. code-block:: cpp

    #include <iostream>
    #include <vector>
    #include <algorithm>

    using namespace std;

    int main(){
        vector<int> v = {1,2,3,1,2,3,1,2,3};
        // ソートする必要あり。
        //  i  0 1 2 3 4 5 6 7 8
        // -> [1,1,1,2,2,2,3,3,3]
        sort(v.begin(),v.end());
        // 2以上の数値が初めて現れる場所 -> 3
        int lower = lower_bound(v.begin(),v.end(),2) - v.begin();
        // 2より大きい数値が初めて表われる場所 -> 6
        int upper = upper_bound(v.begin(),v.end(),2) - v.begin();
        // 2の個数
        int number_of_2 = upper - lower;
    }

*********************
三分探索
*********************
凸関数の極値を求める

.. code-block:: cpp

    // 凸関数の極大な点をもとめる
    template<typename T, typename F>
    T ternary_search(function<F(T)> f, T left, T right)
    {
        static_assert(std::is_integral_v<T> || std::is_floating_point_v<T>);
        assert(left <= right);
        // 整数の場合は候補を 3 つまでに絞る
        constexpr T EPS = (std::is_integral_v<T>) ? 2 : 1e-9;
        while (abs(left - right) > EPS) {
            T l = (2*left + right) / 3;
            T r = (left + 2*right) / 3;
            if (f(l) < f(r))
                left = l;
            else
                right = r;
        }
        if constexpr (std::is_integral_v<T>) {
            // 3 つの候補のうち一番大きいものを返す
            auto max_index = left;
            auto max_value = f(left);
            for (auto i = left + 1; i <= right; i++) {
                auto iv = f(i);
                if (iv > max_value) {
                    max_value = iv;
                    max_index = i;
                }
            }
            return max_index;
        } else
            return (left + right) / 2;
    }

    // 凹関数の極小な・を求める
    template<typename F,typename T>
    T ternary_search_concave(F f,T left,T right){
        return ternary_search([f](T x){return -f(x);},left,right);
    }

*******************
黄金分割探索
*******************

三分探索と同様に凸関数の極大値を求めるが、 分割点を使い回すことによって f の呼び出し回数が少ない。

.. code-block:: cpp

    // 黄金分割探索、凸関数の極大な点をもとめる
    // f の呼び出し回数が少ないので、 f の呼び出しが重い時はこちらを使う
    // この関数は定義域が実数の場合 (T が double など) のみ使える
    // Ref: https://qiita.com/tanaka-a/items/f380257328da421c6584
    template<typename T, typename F>
    T golden_selection_search(function<F(T)> f, T left, T right)
    {
        static_assert(std::is_floating_point_v<T>);
        assert(left <= right);

        double golden_ratio = (3 - std::sqrt(5)) / 2.0;
        constexpr T EPS = 1e-9;

        // 初期分割点
        T l = left + (right - left) * golden_ratio;
        T r = right - (right - left) * golden_ratio;
        T fl = f(l);
        T fr = f(r);
        while (abs(left - right) > EPS) {
            // 分割を更新する際に l, r を使い回す
            if (fl < fr) {
                left = l;
                l = r;
                fl = fr;
                r = right - (right - left) * golden_ratio;
                fr = f(r);
            } else {
                right = r;
                r = l;
                fr = fl;
                l = left + (right - left) * golden_ratio;
                fl = f(l);
            }
        }
        return (left + right) / 2;
    }

*******************
フィボナッチ探索
*******************

黄金分割探索の定義域が整数版。

.. code-block:: cpp

    // フィボナッチ探索、凸関数の極大な点をもとめる
    // f の呼び出し回数が少ないので、 f の呼び出しが重い時はこちらを使う (内部でメモする)
    // この関数は定義域が整数の場合 (T が int など) のみ使える
    // Ref: https://qiita.com/tanaka-a/items/f380257328da421c6584
    // https://atcoder.jp/contests/typical90/tasks/typical90_ba
    template<typename T, typename F>
    T fibonacci_search(function<F(T)> f, T left, T right, const F MINF = std::numeric_limits<F>::min())
    {
        static_assert(std::is_integral_v<T>);

        const T original_left = left;
        const T original_right = right;
        const T extended_left = left - 1;
        T extended_right = right;
        vector<T> fib;
        fib.push_back(1);
        fib.push_back(1);
        fib.push_back(2);
        fib.push_back(3);
        // 間の数をフィボナッチ数にしたいので、弊区間の長さはフィボナッチ数　+ 1 に調整
        while (fib.back() + 1 < (extended_right - extended_left + 1)) {
            fib.push_back(fib[fib.size() - 1] + fib[fib.size() - 2]);
        }
        extended_right = (fib.back() + 1) + extended_left - 1;
        assert(extended_right - extended_left + 1 == fib.back() + 1);

        map<T, F> memo; // f の呼び出しが遅い可能性があるのでメモしておく。
        auto extended_f = [&](T x) {
            if (memo.contains(x))
                return memo[x];
            if (x < original_left) {
                // 単調増加にするため、 extended_left で MINF にするように調整
                return memo[x] = MINF + (x - extended_left);
            } else if (original_right < x) {
                // 単調減少にするために extended_right で MINF にするように調整
                return memo[x] = MINF + (extended_right - x);
            } else {
                return memo[x] = f(x);
            }
        };

        left = extended_left;
        right = extended_right;

        // 初期分割点
        int cur_fib_index = fib.size() - 1;
        T l = left + fib[cur_fib_index - 2];
        T r = right - fib[cur_fib_index - 2];
        while (cur_fib_index >= 3) {
            // 分割を更新する際に l, r を使い回す
            --cur_fib_index;
            if (extended_f(l) < extended_f(r)) {
                left = l;
                l = r;
                r = right - fib[cur_fib_index - 2];
            } else {
                right = r;
                r = l;
                l = left + fib[cur_fib_index - 2];
            }
        }
        {
            // 最後には 3 つの要素が残っているはず。
            auto max_index = left;
            auto max_value = extended_f(max_index);
            for (auto i = left + 1; i <= right; ++i) {
                auto iv = extended_f(i);
                if (iv > max_value) {
                    max_value = iv;
                    max_index = i;
                }
            }
            return max_index;
        }
    }

合計の重さXを達成するような組み合わせの数え上げ
================================================

.. literalinclude:: cpp/combi_total.cpp

*********************
ぐるぐる
*********************
いつかつかうかも。 http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1189

.. literalinclude:: cpp/guruguru.cpp
