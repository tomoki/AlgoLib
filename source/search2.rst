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
    template<typename F,typename T>
    T ternary_search(F f,T left,T right){
        for(int i=0;i<1000;i++){
            T l = (2*left + right) / 3;
            T r = (left + 2*right) / 3;
            if(f(l) < f(r)){
                left = l;
            }else{
                right = r;
            }
        }
        return (left+right)/2;
    }

    // 凹関数の極小な・を求める
    template<typename F,typename T>
    T ternary_search_concave(F f,T left,T right){
        return ternary_search([f](T x){return -f(x);},left,right);
    }

合計の重さXを達成するような組み合わせの数え上げ
====================

.. literalinclude:: cpp/combi_total.cpp

*********************
ぐるぐる
*********************
いつかつかうかも。 http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1189

.. literalinclude:: cpp/guruguru.cpp
