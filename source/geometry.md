# 幾何

## 基本要素

~~~~~~{.cpp}
#include <complex>
using namespace std;

typedef complex<double> point;
bool operator < (const point &lhs,const point &rhs){
    if(real(lhs) == real(rhs)){
        return imag(lhs) < imag(rhs);
    }else{
        return real(lhs) < real(rhs);
    }
}

double cross(const point &a,const point &b){
    return imag(conj(a)*b);
}
double dot(const point &a,const point &b){
    return real(conj(a)*b);
}

// 点の進行方向
int ccw(point a,point b,point c){
    b -= a;c -= a;
    if(cross(b,c) > 0) return +1;    // counter clockwise
    if(cross(b,c) < 0) return -1;    // clockwise
    if(dot(b,c) < 0) return +2;      // c -- a -- b
    if(norm(b) < norm(c)) return -2; // a -- b -- c
    return 0;
}

struct line : vector<point> {
    line(const point &a,const point &b){
        push_back(a);push_back(b);
    }
};

struct circle {
    point center;double r;
    circle(const point &center,double r) : center(center),r(r) {}
};
~~~~~~