#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#define rep(x,n) for(int x=0;x<(n);x++)
#define all(vec) vec.begin(),vec.end()


using namespace std;
template<typename T>
ostream& operator<<(ostream& os,const vector<T>& vec){
    os << "[";
    for(const auto& v : vec){
        os << v << ",";
    }
    os << "]";
    return os;
}

// 重なる長方形がカバーしている面積
int main(){
    int k = 0;
    while(true){
        k++;
        int n; cin >> n;
        if(n == 0) break;
        // (X1,Y1) is upper left.
        // (X1,Y1) is lower right.
        vector<double> X1(n),X2(n),Y1(n),Y2(n);
        rep(i,n){
            double x,y,r;
            cin >> x >> y >> r;
            X1[i] = x-r;
            X2[i] = x+r;
            Y1[i] = y-r;
            Y2[i] = y+r;
        }
        // X,Yは全てのX座標,Y座標
        vector<double> X,Y;
        X.insert(X.end(),all(X1));
        X.insert(X.end(),all(X2));
        Y.insert(Y.end(),all(Y1));
        Y.insert(Y.end(),all(Y2));

        // sortして重複を削除
        sort(all(X));
        sort(all(Y));
        X.erase(unique(all(X)),X.end());
        Y.erase(unique(all(Y)),Y.end());

        // X1,X2,Y1,Y2の値をX,Y中のindexにする。この時点でintにすべきかも。
        rep(i,n){
            X1[i] = lower_bound(all(X),X1[i]) - X.begin();
            X2[i] = lower_bound(all(X),X2[i]) - X.begin();
            Y1[i] = lower_bound(all(Y),Y1[i]) - Y.begin();
            Y2[i] = lower_bound(all(Y),Y2[i]) - Y.begin();
        }
        vector<vector<char>> is_on(X.size(),vector<char>(Y.size()));
        for(int i=0;i<n;i++){
            for(int sx=X1[i];sx<X2[i];sx++){
                for(int sy=Y1[i];sy<Y2[i];sy++){
                    is_on[sx][sy] = true;
                }
            }
        }
        double ans = 0;
        for(int i=0;i<X.size()-1;i++){
            for(int j=0;j<Y.size()-1;j++){
                if(is_on[i][j]){
                    ans += (X[i+1]-X[i]) * (Y[j+1]-Y[j]);
                }
            }
        }
        cout << fixed << setprecision(2);
        cout << k << " " << ans << endl;
    }
}
