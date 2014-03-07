#include <vector>

using namespace std;

struct UnionFind{
    const int is_root = -1;
    vector<int> data;
    UnionFind(int n){
        data = vector<int>(n,-1);
    }
    // 親を探す
    int root(int x){
        if(data[x] < 0){
            return x;
        }else{
            return data[x] = root(data[x]);
        }
    }
    // x,yの含まれる集合を併合
    bool unite(int x,int y){
        x = root(x);
        y = root(y);
        if(x != y){
            // 大きいほうに追加する。
            if(data[y] < data[x]) swap(x,y);
            data[x] += data[y];
            data[y] = x;
            return true;
        }else{
            return false;
        }
    }
    // 同じ集合にいるかどうか
    bool same(int x,int y){
        return root(x) == root(y);
    }
    int size(int x){
        return -data[root(x)];
    }
};
