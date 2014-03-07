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
bool is_intersected_linesegment(point a1,point a2,point b1,point b2){
    if(max(a1.real(),a2.real()) + EPS < min(b1.real(),b2.real())) return false;
    if(max(b1.real(),b2.real()) + EPS < min(a1.real(),a2.real())) return false;
    if(max(a1.imag(),a2.imag()) + EPS < min(b1.imag(),b2.imag())) return false;
    if(max(b1.imag(),b2.imag()) + EPS < min(a1.imag(),a2.imag())) return false;
    return (cross(a2-a1,b1-a1)*cross(a2-a1,b2-a1) < EPS) and
           (cross(b2-b1,a1-b1)*cross(b2-b1,a2-b1) < EPS);

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

// 線分同士の最短距離
double dist_linesegment_and_linesegment(point a1,point a2,point b1,point b2){
    if(is_intersected_linesegment(a1,a2,b1,b2)){
        return 0;
    }
    return min(min(dist_linesegment_and_point(a1,a2,b1),
               dist_linesegment_and_point(a1,a2,b2)),
           min(dist_linesegment_and_point(b1,b2,a1),
               dist_linesegment_and_point(b1,b2,a2)));
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

// 直線と点の最短距離を実現する直線の点(すいせんの足)(みけんしょう)
point nearest_point_line_and_point(point a1,point a2,point b){
    return a1 + (a2-a1) * dot((a2-a1),(b-a1)) / norm(a2-a1);
}

// 線分と点の最短距離を実現する線分嬢の点(みけんしょう)
point nearest_point_linesegment_and_point(point a1,point a2,point b){
    if(dot(a2-a1,b-a1) < EPS) return a1;
    if(dot(a1-a2,b-a2) < EPS) return a2;
    return nearest_point_line_and_point(a1,a2,b);
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

// 点が真に多角形(凸?)の中にはいっているか
bool is_inner_point_vertex(const vector<point> &ps,point a){
    int cc = ccw(ps[0],ps[1],a);
    if(not(cc == 1 or cc == -1)) return false;
    for(size_t i=0;i<ps.size();i++){
        if(cc != ccw(ps[i],ps[(i+1)%ps.size()],a)) return false;
    }
    return true;
}

// 点が辺上、もしくは内部にある。(未検証)
bool is_inner_point_vertex_or_line(const vector<point> &ps,point a){
    for(size_t i=0;i<ps.size();i++){
        if(dist_linesegment_and_point(ps[i],ps[(i+1)%ps.size()],a) < EPS){
            return true;
        }
    }
    return is_inner_point_vertex(ps,a);
}


// 凸包 (UVA 109)
vector<point> convex_hull(vector<point> ps){
    int n = ps.size();
    int k = 0;
    sort(ps.begin(),ps.end());
    vector<point> ch(2*n);
    for(int i=0;i<n;ch[k++] = ps[i++]){
        while(k >= 2 and ccw(ch[k-2],ch[k-1],ps[i]) <= 0) --k;
    }
    for(int i=n-2,t=k+1;i>=0;ch[k++]=ps[i--]){
        while(k >= t and ccw(ch[k-2],ch[k-1],ps[i]) <= 0) --k;
    }
    ch.resize(k-1);
    return ch;
}

// remember,pts make convex.
// (http://judge.u-aizu.ac.jp/onlinejudge/cdescription.jsp?cid=ACAC002&pid=C)
double convex_diameter(const vector<point>& pts){
    const int n = pts.size();
    int is=0,js=0; // initial antipode.
    for(int i=1;i<n;i++){
        if(pts[i].imag() > pts[is].imag()) is = i;
        if(pts[i].imag() < pts[js].imag()) js = i;
    }
    double maxd = abs(pts[is]-pts[js]);
    int i,j,maxi,maxj;
    i = maxi = is;
    j = maxj = js;
    do{
        if(cross(pts[(i+1)%n]-pts[i],
                 pts[(j+1)%n]-pts[j]) >= 0){
            j = (j+1)%n;
        }else{
            i = (i+1)%n;
        }
        if(abs(pts[i]-pts[j]) > maxd){
            maxd = abs(pts[i]-pts[j]);
            maxi = i;maxj = j;
        }
    } while(not(i == is and j == js));
    // pts[maxi],pts[maxj] is pair of max diff.
    return maxd;
}

// 円と円の交点(2点ある前提)
vector<point> circles_point(point c1,double r1,point c2,double r2){
    double d = abs(c1-c2);
    double s = (r1+r2+d) / 2;
    double S = sqrt(s*(s-r1)*(s-r2)*(s-d));
    double h = 2 * S / d;
    point v = (c2-c1) / (abs(c2-c1));

    double m = sqrt(r1*r1 - h*h);

    vector<point> ret;
    ret.push_back(c1 + m*v+h*v*point(0,1));
    ret.push_back(c1 + m*v-h*v*point(0,1));
    return ret;
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

## 最近点対

~~~~~~{include="cpp/closest_pair_of_points.cpp" .cpp}
~~~~~~
