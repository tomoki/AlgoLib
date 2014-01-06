#define repeat(i,n) for(int i=0;i<static_cast<int>(n);i++)
typedef vector<int> vi;
typedef vector<vi> vvi;

// see http://algorithms.blog55.fc2.com/blog-entry-132.html
//  max rectangle area in histogram.
template<typename T>
T max_rectangle_size_in_histogram(const vector<T>& v){
    // where,height. in this function,use index as where.
    typedef pair<int,T> piT;
    stack<piT> sta;
    T ret = 0;
    for(int i=0;i<v.size();i++){
        if(sta.empty() or sta.top().second < v[i]){
            sta.push(make_pair(i,v[i]));
        }else if(sta.top().second == v[i]){
            // pass
        }else if(sta.top().second > v[i]){
            piT t;
            while(not sta.empty() and sta.top().second >= v[i]){
                t = sta.top();
                ret = max(ret,t.second*(i-t.first));
                sta.pop();
            }
            sta.push(make_pair(t.first,v[i]));
        }
    }
    int total_width = v.size();
    while(not sta.empty()){
        piT t = sta.top();
        sta.pop();
        ret = max(ret,t.second*(total_width-t.first));
    }
    return ret;
}

// square area 
int solve(const vvi &matrix){
    int h = matrix.size();
    int w = matrix[0].size();
    vvi up(h,vi(w));

    {
        for(int i=0;i<h;i++){
            for(int j=0;j<w;j++){
                if(i == 0){
                    up[i][j] = matrix[i][j] == 1;
                }else {
                    if(matrix[i][j] == 0){
                        up[i][j] = 0;
                    }else{
                        up[i][j] = up[i-1][j]+1;
                    }
                }
            }
        }
    }

    int ret = 0;
    for(int y=0;y<h;y++){
        ret = max(ret,max_rectangle_size_in_histogram(up[y]));
    }
    return ret;
}
