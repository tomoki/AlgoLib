# 探索

## 全探索
全部しらべる。

~~~~~~{.cpp}
int linear_search(vector<int> V,int val){
    repeat(i,V.size()){
        if(V[i] == val) return i;
    }
    return -1;
}
~~~~~~

## 二分探索
ある条件を満たす最小のものを探す。ただし単調増加な物にしかつかえない。
叙述関数をPとすると、

~~~~~~{.cpp}
double lower = 0,upper = 1000000;
for(int i=0;i<200;i++){
    double m = (lower+upper) / 2;
    if(P(m)){
        upper = m;
    }else{
        lower = m;
    }
    cout << upper << endl;
}
~~~~~~

とすると、upperに求めたい値がはいる。もしみつからなかった場合には、値は
変わらない。なので、lower,upperには極端な値を設定すること。200という回数
は、すこし多め。100で十分。対象がvectorの場合は以下のように書ける。

~~~~~~{.cpp}
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
~~~~~~

### 合計の重さXを達成するような組み合わせの数え上げ

~~~~~~{.cpp include="cpp/combi_total.cpp"}
~~~~~~
