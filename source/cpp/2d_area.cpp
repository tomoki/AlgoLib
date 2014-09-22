#include <vector>
#include <iostream>

using namespace std;

// ret[i][j] = sum of all field in [0,i) x [0,j).
template<typename T>
vector<vector<T>> calc_sum(const vector<vector<T>>& field){
    int h = field.size();
    int w = field[0].size();
    vector<vector<T>> ret(h+1,vector<T>(w+1));
    for(int y=1;y<=h;y++){
        for(int x=1;x<=w;x++){
            ret[y][x] = ret[y][x-1] + field[y-1][x-1];
        }
    }
    for(int x=0;x<=w;x++){
        for(int y=1;y<=h;y++){
            ret[y][x] = ret[y][x] + ret[y-1][x];
        }
    }
    return ret;
}

// calc [sy,gy] x [sx,gx]
template<typename T>
T calc(const vector<vector<T>>& v,int sy,int sx,int gy,int gx){
    // for close section
    gy++;gx++;
    return v[gy][gx] - v[sy][gx] - v[gy][sx] + v[sy][sx];
}

int main(){
    vector<vector<int>> field = {{0,1,2},
                                 {3,4,5},
                                 {6,7,8}};
    auto s = calc_sum(field);
    // calc 3+5+6 + 6+7+8
    cout << calc(s,1,0,2,2) << endl;

    return 0;
}

// 座標圧縮
int main(){
    int n,m;cin >> n >> m;

    vector<int> index_of_x(n),index_of_y(n);
    for(int i=0;i<n;i++){
        cin >> index_of_x[i] >> index_of_y[i];
    }
    vector<int> value_of_x = index_of_x,
                value_of_y = index_of_y;

    sort(all(value_of_x));
    sort(all(value_of_y));
    value_of_x.erase(unique(all(value_of_x)),value_of_x.end());
    value_of_y.erase(unique(all(value_of_y)),value_of_y.end());
    for(int i=0;i<n;i++){
        index_of_x[i] = lower_bound(all(value_of_x),index_of_x[i]) - value_of_x.begin();
        index_of_y[i] = lower_bound(all(value_of_y),index_of_y[i]) - value_of_y.begin();
    }
    vector<vector<int>> p(value_of_y.size(),vector<int>(value_of_x.size()));
    for(int i=0;i<n;i++){
        p[index_of_y[i]][index_of_x[i]] += 1;
    }
    auto v = calc_sum(p);

    for(int i=0;i<m;i++){
        int x1,y1,x2,y2;
        cin >> x1 >> y1 >> x2 >> y2;

        int sx = lower_bound(all(value_of_x),x1) - value_of_x.begin();
        int gx = upper_bound(all(value_of_x),x2) - value_of_x.begin();
        int sy = lower_bound(all(value_of_y),y1) - value_of_y.begin();
        int gy = upper_bound(all(value_of_y),y2) - value_of_y.begin();

        int ans = v[gy][gx] - v[sy][gx] - v[gy][sx] + v[sy][sx];
        cout << ans << endl;
    }

    return 0;
}
