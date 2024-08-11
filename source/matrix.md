# 行列

~~~~~~
// T 型を要素とする行列型
// Matrix<int> m = {{1, 2}, {3, 4}}
template<typename T>
struct Matrix {
    Matrix(size_t height, size_t width) : m_height(height), m_width(width), m_value(height, vector<T>(width)) { }
    Matrix(const vector<vector<T>>& v) :  m_height(v.size()), m_width(v[0].size()), m_value(v) { } // NOLINT
    Matrix(initializer_list<initializer_list<T>> init) : m_height(init.size()), m_width(init.begin()->size()), m_value(m_height, vector<T>(m_width))
    {
        size_t r = 0;
        for (const auto& row : init) {
            size_t c = 0;
            for (const auto& col : row) {
                m_value[r][c] = col;
                c++;
            }
            assert(c == m_width);
            r++;
        }
    }
    // i 行目を取り出す
    vector<T>& operator[](size_t i) { return m_value[i]; }
    Matrix& operator+=(const Matrix& rhs)
    {
        assert(m_width == rhs.m_width);
        assert(m_height == rhs.m_height);
        for (size_t i = 0; i < m_height; i++)
            for (size_t j = 0; j < m_width; j++)
                m_value[i][j] += rhs.m_value[i][j];
        return *this;
    }
    Matrix& operator-=(const Matrix& rhs)
    {
        assert(m_width == rhs.m_width);
        assert(m_height == rhs.m_height);
        for (size_t i = 0; i < m_height; i++)
            for (size_t j = 0; j < m_width; j++)
                m_value[i][j] -= rhs.m_value[i][j];
        return *this;
    }
    friend Matrix operator+(Matrix lhs, const Matrix& rhs) { lhs += rhs; return lhs;}
    friend Matrix operator-(Matrix lhs, const Matrix& rhs) { lhs -= rhs; return lhs;}
    friend Matrix operator*(T lhs, Matrix rhs)
    {
        for (size_t i = 0; i < rhs.m_height; i++)
            for (size_t j = 0; j < rhs.m_width; j++)
                rhs.m_value[i][j] *= lhs;
        return rhs;
    }
    // AxB の行列と BxC の行列を受け取って、 AxC の行列を返す
    friend Matrix operator*(Matrix lhs, const Matrix& rhs)
    {
        assert(lhs.m_width == rhs.m_height);
        // TODO: 最適化する。 r -> k -> c の方が早いはず？？
        Matrix ret(lhs.m_height, rhs.m_width);
        for (size_t r = 0; r < lhs.m_height; r++)
            for (size_t c = 0; c < rhs.m_width; c++)
                for (size_t k = 0; k < lhs.m_width; k++)
                    ret.m_value[r][c] += lhs.m_value[r][k] * rhs.m_value[k][c];
        return ret;
    }
    // 一次元ベクトルとの掛け算
    friend vector<T> operator*(Matrix lhs, const vector<T>& rhs)
    {
        assert(lhs.m_width == rhs.size());
        vector<T> ret(lhs.m_height);
        for (size_t r = 0; r < lhs.m_height; r++) {
            for (size_t c = 0; c < lhs.m_width; c++) {
                ret[r] += lhs[r][c] * rhs[c];
            }
        }
        return ret;
    }
    // 行列を n 乗
    Matrix pow(uint64_t n)
    {
        assert(m_height == m_width);
        if (n == 0) {
            Matrix E(m_height, m_width);
            for (size_t i = 0; i < m_height; i++) E[i][i] = 1;
            return E;
        }
        Matrix ret = (*this * *this).pow(n/2);
        if (n % 2 == 1) ret = ret * *this;
        return ret;
    }
    friend ostream& operator<<(ostream& os, const Matrix& rhs)
    {
        for (size_t i = 0; i < rhs.m_height; i++) {
            for (size_t j = 0; j < rhs.m_width; j++) {
                if (j != 0) os << " ";
                os << rhs.m_value[i][j];
            }
            if (i != rhs.m_height - 1) os << endl;
        }
        return os;
    }
private:
    size_t m_height;
    size_t m_width;
    vector<vector<T>> m_value;
};
~~~~~~

## Gauss-Jordan

GF上で連立方程式を解く

~~~~~~
const int none = 0; // if no answer
const int one = 1;  // if there is exactly one answer
const int many = 2; // many answer.
// answer will be inserted in b.
int gauss(Matrix<int> A,vector<int>& b){
    int n = A.height();
    int m = A.width();
    int pi = 0,pj = 0;
    while(pi < n and pj < m){
        for(int i=pi+1;i<n;i++){ // pivot
            if(abs(A[i][pj]) > abs(A[pi][pj])){
                swap(A[i],A[pi]);
                swap(b[i],b[pi]);
            }
        }
        if(abs(A[pi][pj]) > 0){
            int d = invert(A[pi][pj]);
            for(int j=0;j<m;j++){
                A[pi][j] = modulo(A[pi][j] * d);
            }
            b[pi] = modulo(b[pi]*d);
            for(int i=pi+1;i<n;i++){
                int k = A[i][pj];
                for(int j=0;j<m;j++){
                    A[i][j] = modulo(A[i][j] - k * A[pi][j]);
                }
                b[i] = modulo(b[i] - k*b[pi]);
            }
            pi++;
        }
        pj++;
    }
    for(int i=pi;i<n;i++){
        if(abs(b[i]) > 0){
            return none;
        }
    }
    if(pi < m or pj < m){
        return many;
    }
    for(int j=m-1;j>=0;j--){
        for(int i=0;i<j;i++){
            b[i] = modulo(b[i] - b[j] * A[i][j]);
        }
    }
    return one;
}
~~~~~~
