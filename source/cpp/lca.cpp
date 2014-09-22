#include <vector>
#include <cmath>
using namespace std;

// lowest common ancestor.
//  queue version.
struct LCASolver{
    vector<vector<int>> children;
    int root;
    // if root,parent is -1.
    vector<int> parent;
    vector<int> depth;

    vector<vector<int>> parent_pow2;

    LCASolver(vector<vector<int>> children_,int root_)
        : children(children_),root(root_){
        calc_parent_and_depth();
        calc_parent_pow2();
    };
    
    int lca(int u,int v){
        // make sure depth(u) > depth(v).
        if(depth[u] < depth[v]) swap(u,v);
        for(size_t k=0;k<parent_pow2.size();k++){
            if(((depth[u] - depth[v]) >> k) & 1){
                u = parent_pow2[k][u];
            }
        }

        if(u == v) return u;
        for(int k=(int)parent_pow2.size()-1;k>=0;k--){
            if(parent_pow2[k][u] != parent_pow2[k][v]){
                u = parent_pow2[k][u];
                v = parent_pow2[k][v];
            }
        }
        return parent_pow2[0][u];
    }

private:
    void calc_parent_and_depth(){
        parent = vector<int>(children.size(),-1);
        depth = vector<int>(children.size(),-1);
        sub_calc_parent_and_depth(root,-1,0);
    }
    void sub_calc_parent_and_depth(int cur,int par,int dep){
        parent[cur] = par;
        depth[cur] = dep;
        for(int child : children[cur]){
            if(child != par){
                sub_calc_parent_and_depth(child,cur,dep+1);
            }
        }
    }
    void calc_parent_pow2(){
        // parent_pow2[k][i] = 2^k parent of node i.
        parent_pow2 = vector<vector<int>>(ceil(log(children.size())/log(2)),
                                          vector<int>(children.size(),-1));
        parent_pow2[0] = parent;
        for(size_t k=0;k+1<parent_pow2.size();k++){
            for(size_t v=0;v<children.size();v++){
                if(parent_pow2[k][v] >= 0){
                    parent_pow2[k+1][v] = parent_pow2[k][parent_pow2[k][v]];
                }
            }
        }
    }
};
