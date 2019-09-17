#include <vector>
using namespace std;

struct SegmentTree{
    // edit this things! Range Sum Query
    static const int INIT_VALUE = 0;
    inline int calc(int a,int b){
        return a+b;
    }

    // Range Minimum Query
    // static const int INIT_VALUE = 1 << 30;
    // inline int calc(int a,int b){
    //     return min(a,b);
    // }


    vector<int> data;
    int n;
    explicit SegmentTree(int _n){
        n = 1;
        while(n < _n) n *= 2;
        data = vector<int>(2*n-1,INIT_VALUE);
    }

    void update(int i,int v){
        int k = i + n-1;
        data[k] = v;
        while(k > 0){
            k = parent(k);
            data[k] = calc(data[get_left(k)],data[get_right(k)]);
        }
    }

    // return data of [a,b).
    inline int get(int a,int b){
        return get(a,b,0,0,n);
    }
private:
    // l,r is segment of node k.
    int get(int a,int b,int k,int l,int r){
        if(r <= a or b <= l) return INIT_VALUE;
        if(a <= l and r <= b) return data[k];
        int vl = get(a,b,get_left(k),l,(l+r)/2);
        int vr = get(a,b,get_right(k),(l+r)/2,r);
        return calc(vl,vr);
    }

    inline int get_left(int x){
        return 2*x+1;
    }
    inline int get_right(int x){
        return 2*x+2;
    }
    inline int parent(int x){
        return (x-1)/2;
    }
};
