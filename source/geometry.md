# 幾何

~~~~~~{.cpp}
#include <complex>
using namespace std;

const double EPS = 1e-9;
typedef complex<double> point;
typedef vector<point> vertex;

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

// 外積 (cross product) : |a×b| = |a||b|sinΘ
double cross(point a,point b){
    return imag(conj(a)*b);
}

// a1,a2を端点とする線分(la)とb1,b2を端点(lb)とする線分の交差判定
bool is_intersected_linesegment(point a1,point a2,point b1,point b2) {
    return ( cross(a2-a1, b1-a1) * cross(a2-a1, b2-a1) < EPS ) &&
        ( cross(b2-b1, a1-b1) * cross(b2-b1, a2-b1) < EPS );
}

// a1,a2を端点とする線分(la)とb1,b2を端点とする線分(lb)の交点計算
point intersection_point_linesegment(point a1,point a2,point b1,point b2) {
    if(a1 == b1 or a1 == b2) return a1;
    if(a2 == b1 or a2 == b2) return a2;
    point b = b2-b1;
    double d1 = abs(cross(b, a1-b1));
    double d2 = abs(cross(b, a2-b1));
    double t = d1 / (d1 + d2);
    return a1 + (a2-a1) * t;
}


// 2直線の直交判定 : a⊥b <=> dot(a, b) = 0
// みけんしょう
bool is_orthogonal(point a1,point a2,point b1,point b2) {
    return dot(a1-a2,b1-b2) < EPS;
}

// 2直線の平行判定 : a//b <=> cross(a, b) = 0
bool is_parallel(point a1,point a2,point b1,point b2) {
    return abs(cross(a2-a1,b2-b1)) < EPS;
}

// a1,a2を通る直線とb1,b2を通る直線の交差判定
bool is_intersected_line(point a1,point a2,point b1,point b2) {
    return not is_parallel(a1,a2,b1,b2);
}

// a1,a2を通る直線とb1,b2を通る直線の交点計算
point intersection_line(point a1,point a2,point b1,point b2) {
    point a = a2 - a1,b = b2 - b1;
    return a1 + a * cross(b, b1-a1) / cross(b, a);
}

// 直線と点との距離
double dist_line_and_point(point a1,point a2,point b){
    return abs(cross(a2-a1,b-a1)) / abs(a2-a1);
}

// 線分と点との距離
double dist_linesegment_and_point(point a1,point a2,point b){
    if(dot(a2-a1,b-a1) < EPS) return abs(b-a1);
    if(dot(a1-a2,b-a2) < EPS) return abs(b-a2);
    return dist_line_and_point(a1,a2,b);
}


// 円と線分の交差判定
bool is_cross_linesegment_and_circle(point c,double r,point a1,point a2){
    return (dist_linesegment_and_point(a1,a2,c) < r+EPS and
             (r < abs(c-a1) + EPS or r < abs(c-a2) + EPS));
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

// 点が多角形の中にはいっているか
bool is_inner_point_vertex(vector<point> ps,point a){
    int cc = ccw(ps[0],ps[1],a);
    if(not(cc == 1 or cc == -1)) return false;
    for(int i=0;i<ps.size();i++){
        if(cc != ccw(ps[i],ps[(i+1)%ps.size()],a)) return false;
    }
    return true;
}


// clockwiseだと負
double triangle_area(point a,point b,point c){
    return cross(b-a,c-a)/2;
}

// clockwiseだと負
double vertex_area(vector<point> v){
    double ret = 0;
    for(int i=1;i<v.size()-1;i++){
        ret += triangle_area(v[0],v[i],v[i+1]);
    }
    return ret;
}

~~~~~~
