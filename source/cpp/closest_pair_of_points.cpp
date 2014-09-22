#include <complex>
#include <algorithm>
#include <iostream>
#include <vector>


using namespace std;

const double INF = 1e9;
typedef complex<double> point;
double sub_closest_pair_in_field(const vector<point> &ps){
    int N = ps.size();
    if(N <= 3){
        double ret = INF;
        for(size_t i=0;i<ps.size();i++){
            for(size_t j=i+1;j<ps.size();j++){
                ret = min(ret,abs(ps[i]-ps[j]));
            }
        }
        return ret;
    }

    point middle = ps[N/2];
    vector<point> left(ps.begin(),ps.begin()+N/2);
    vector<point> right(ps.begin()+N/2+1,ps.end());

    double d = min(sub_closest_pair_in_field(left),
                   sub_closest_pair_in_field(right));

    vector<point> strip;
    for(const point& p : ps){
        if(abs(middle.real()-p.real()) < d){
            strip.push_back(p);
        }
    }
    sort(strip.begin(),strip.end(),[](const complex<double>& lhs,
                                      const complex<double>& rhs){
                                          return lhs.imag() < rhs.imag();
                                      });

    for(size_t i=0;i<strip.size();i++){
        for(size_t j=i+1;j<strip.size() and
                         (strip[j].imag() - strip[i].imag()) < d;j++){
            d = min(d,abs(strip[i]-strip[j]));
        }
    }
    return d;
}

// find closest pair in field.return distance.
//  O(nlogn) http://www.geeksforgeeks.org/closest-pair-of-points/
double closest_pair_in_field(vector<point> ps){
    sort(ps.begin(),ps.end(),[](const complex<double>& lhs,
                                const complex<double>& rhs){
                                    return lhs.real() < rhs.real();
                                });
    return sub_closest_pair_in_field(ps);
}

int main(){
    int n;cin >> n;
    vector<point> ps(n);
    repeat(i,n){
        double x,y;
        cin >> x >> y;
        ps[i].real(x);
        ps[i].imag(y);
    }

    cout << fixed << setprecision(10);
    cout << closest_pair_in_field(ps) << endl;
    return 0;
}
