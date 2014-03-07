#include <vector>
using namespace std;

// if we have objects that each has w_{i} weight.
//  we calc how many combination that has total weight X.
//  O(2^(N/2) logN)?
template<typename T>
int howmany_combination_such_that_total_weight_is_X(vector<T> W,int X){
    // sort(all(W));
    int N = W.size();
    vector<vector<T>> parts(2);
    copy(W.begin(),W.begin()+N/2,back_inserter(parts[0]));
    copy(W.begin()+N/2,W.end(),back_inserter(parts[1]));

    int ret = 0;
    vector<vector<T>> sums(2,{0});
    for(size_t p=0;p<parts.size();p++){
        for(size_t i=0;i<parts[p].size();i++){
            vector<T> nex;
            for(size_t j=0;j<sums[p].size();j++){
                nex.push_back(sums[p][j]);
                nex.push_back(sums[p][j]+parts[p][i]);
            }
            sums[p].swap(nex);
        }
    }
    for(vector<T>& sum : sums){
        sort(sum.begin(),sum.end());
    }

    for(T s : sums[0]){
        ret += upper_bound(sums[1].begin(),sums[1].end(),X-s)
             - lower_bound(sums[1].begin(),sums[1].end(),X-s);
    }
    return ret;
}
