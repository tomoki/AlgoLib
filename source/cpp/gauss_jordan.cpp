const int none = 0; // if no answer
const int one = 1;  // if there is exactly one answer
const int many = 2; // many answer.
// answer will be inserted in b.
//  test中
int normal_gauss_jordan(vector<vector<double>> A,vector<double>& b){
    const double EPS = 1e-8;
    int n = A.size();
    int m = A[0].size();
    int pi = 0,pj = 0;
    while(pi < n and pj < m){
        for(int i=pi+1;i<n;i++){ // pivot
            if(abs(A[i][pj]) > abs(A[pi][pj])){
                swap(A[i],A[pi]);
                swap(b[i],b[pi]);
            }
        }
        if(abs(A[pi][pj]) > EPS){
            double d = A[pi][pj];
            for(int j=0;j<m;j++){
                A[pi][j] /= d;
            }
            b[pi] /= d;
            for(int i=pi+1;i<n;i++){
                double k = A[i][pj];
                for(int j=0;j<m;j++){
                    A[i][j] -= k * A[pi][j];
                }
                b[i] -= k*b[pi];
            }
            pi++;
        }
        pj++;
    }
    for(int i=pi;i<n;i++){
        if(abs(b[i]) > EPS){
            return none;
        }
    }
    if(pi < m or pj < m){
        return many;
    }
    for(int j=m-1;j>=0;j--){
        for(int i=0;i<j;i++){
            b[i] -= b[j] * A[i][j];
        }
    }
    return one;
}
