#include <complex>
using namespace std;

const double EPS = 1e-9;
typedef complex<double> point;
typedef pair<point,point> line;

namespace std{
    bool operator < (const point &lhs,const point &rhs){
        if(real(lhs) == real(rhs)){
            return imag(lhs) < imag(rhs);
        }else{
            return real(lhs) < real(rhs);
        }
    }
}

// ベクタの長さ
double vector_length(point a){
    return abs(a);
}

// 二点間距離
double point_distance(point a,point b){
    return abs(a-b);
}

// 単位ベクトル
point unit_vector(point a){
    return a / abs(a);
}

// 法線ベクトル
pair<point,point> normal_vector(point a){
    point n1 = a * point(0,1);
    point n2 = a * point(0,-1);
    return make_pair(n1,n2);
}

// 点が一緒かどうか
bool point_eq(point a,point b){
    return abs(a-b) < EPS;
}
// 内積 (dot product) : a・b = |a||b|cosΘ
double dot(point a,point b){
    return real(conj(a)*b);
}

// 外積 (cross product) : a×b = |a||b|sinΘ
double cross(point a,point b){
    return imag(conj(a)*b);
}

// a1,a2を端点とする線分(la)とb1,b2を端点(lb)とする線分の交差判定
bool is_intersected_linesegment(line la,line lb) {
    point a1=la.first,a2=la.second;
    point b1=lb.first,b2=lb.second;
    return ( cross(a2-a1, b1-a1) * cross(a2-a1, b2-a1) < EPS ) &&
        ( cross(b2-b1, a1-b1) * cross(b2-b1, a2-b1) < EPS );
}


// 2直線の直交判定 : a⊥b <=> dot(a, b) = 0
// みけんしょう
bool is_orthogonal(line a,line b) {
  return dot(a.first-a.second, b.first-b.second) < EPS;
}

// 2直線の平行判定 : a//b <=> cross(a, b) = 0
// みけんしょう
bool is_parallel(line a,line b) {
    return cross(a.first-a.second,b.first-b.second) < EPS;
}

// a1,a2を端点とする線分(la)とb1,b2を端点とする線分(lb)の交点計算
point intersection_point_linesegment(line la,line lb) {
    point a1 = la.first,a2=la.second;
    point b1 = lb.first,b2=lb.second;
    if(a1 == b1 or a1 == b2) return a1;
    if(a2 == b1 or a2 == b2) return a2;
    point b = b2-b1;
    double d1 = abs(cross(b, a1-b1));
    double d2 = abs(cross(b, a2-b1));
    double t = d1 / (d1 + d2);
    return a1 + (a2-a1) * t;
}

// a1,a2を通る直線とb1,b2を通る直線の交差判定
bool is_intersected_l(line la,line lb) {
    return not is_parallel(la,lb);
}

// a1,a2を通る直線とb1,b2を通る直線の交点計算
point intersection_l(line la,line lb) {
    point a1 = la.first,a2=la.second;
    point b1 = lb.first,b2=lb.second;
    point a = a2 - a1,b = b2 - b1;
    return a1 + a * cross(b, b1-a1) / cross(b, a);
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
