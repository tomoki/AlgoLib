# ゲーム

## Nim

いくつかのコインの山がある。この中からプレイヤーは山を一つ選び、1個以上の任意の
数のコインを取る。最後のコインを取ったプレイヤーが勝ちである。この問題に
対しては以下のことが知られている。すべての山のxorをとったとき、それが0で
あるとき、後攻の勝ち、それ以外のときは先攻の勝ち。

~~~~~~{.cpp}
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

~~~~~~