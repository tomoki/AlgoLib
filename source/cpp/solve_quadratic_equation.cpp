// solve ax^2 + bx + c = 0.
vector<double> solve_quadratic_equation(double a,double b,double c){
    vector<double> ret;
    if(abs(a-EPS) < 0){
        ret.push_back(-c/b);
    }else{
        double d = b*b-4*a*c;
        if(d >= 0){
            if(b >= 0){
                ret.push_back(-2*c/(b+sqrt(d)));
                ret.push_back((-b-sqrt(d))/(2*a));
            }else{
                ret.push_back(-2*c/(b-sqrt(d)));
                ret.push_back((-b+sqrt(d))/(2*a));
            }
            if(abs(ret[0]-ret[1]) < EPS){
                ret.pop_back();
            }
        }
    }
    return ret;
}
