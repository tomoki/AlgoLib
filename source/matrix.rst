######################
行列
######################

.. code-block:: cpp

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
        const vector<T>& operator[](size_t i) const { return m_value[i]; }

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
        // 行基本変形によって上三角化 (標準形) への変形を行う
        // https://drken1215.hatenablog.com/entry/2019/03/20/202800
        int triangler_inplace(bool is_extended = false)
        {
            assert(m_width >= 1 && m_height >= 1);
            int rank = 0;
            // 連立方程式を解く際には係数拡大行列を作る、その際にはその部分は掃き出し法の対象にしない
            for (size_t col = 0; col < (is_extended ? (m_width-1) : m_width) ; col++) {
                std::optional<int> pivot = std::nullopt;
                for (size_t row = rank; row < m_height; row++) {
                    if constexpr (std::is_floating_point_v<T>) {
                        if ((!pivot.has_value() && abs(m_value[row][col]) > 1e-10) || (pivot.has_value() && abs(m_value[row][col]) > abs(m_value[pivot.value()][col])))
                            pivot = row;;
                    } else {
                        if (!pivot.has_value() && m_value[row][col] != 0) {
                            pivot = row;
                            break;
                        }
                    }
                }
                // ピボットが見つからなかったら次の列へ
                if (!pivot.has_value()) continue;
                swap(m_value[pivot.value()], m_value[rank]);

                // ピボットの値を 1 にする
                for (int j = m_width - 1; j >= static_cast<int>(col); j--) {
                    m_value[rank][j] /= m_value[rank][col];
                }

                // 掃き出しによって他の列から
                for (size_t row = 0; row < m_height; row++) {
                    if (static_cast<int>(row) == rank) continue;
                    auto m = m_value[row][col];
                    if constexpr (std::is_floating_point_v<T>) {
                        if (abs(m) < 1e-10) continue;
                    } else {
                        if (m == 0) continue;
                    }
                    for (size_t j = rank; j < m_width; j++) {
                        m_value[row][j] -= m_value[rank][j] * m;
                    }
                }
                rank++;
            }

            return rank;
        }
        // 行基本変形によって上三角化を行う
        Matrix trianglar(bool is_extended = false)
        {
            Matrix ret = *this;
            ret.triangler_inplace(is_extended);
            return ret;
        }
        [[nodiscard]] size_t width() const { return m_width; }
        [[nodiscard]] size_t height() const { return m_height; }
    private:
        size_t m_height;
        size_t m_width;
        vector<vector<T>> m_value;
    };

*********************
Gauss-Jordan
*********************

掃き出し法によって連立一次方程式を解く

.. code-block:: cpp

    // Gauss-Jordan 法によって一次連立方程式を解く
    template <typename T>
    struct GaussJordanResult {
        enum class SolutionType {
            none, // 解なし
            one, // 一意に定まる
            some  // いくつか存在する
        };
        SolutionType type;
        Matrix<T> extended_matrix {0, 0};
        int rank;
    };

    // mat x = v を解く
    // result.type == none なら解なし (一次方程式の中に矛盾が存在)
    // result.type == one なら解が一つだけ
    // result.type == some の時、実際の解の数は T に依存する
    //   自由変数 (パラメータ) の数が mat.width() - rank 個存在するため、
    //   - もし T が mod_int なら解の数は 2 ** (mat.width() - rank)
    //   - もし T が実数 (double など) なら無限
    template<typename T>
    GaussJordanResult<T> solve_linear_equations(const Matrix<T>& mat, const vector<T>& v)
    {
        assert(mat.height() == v.size());
        // 拡大係数行列を作成する (右側に v をくっつける)
        GaussJordanResult<T> result;

        result.extended_matrix = Matrix<T>(mat.height(), mat.width() + 1);

        size_t extended_col = mat.width();
        for (size_t r = 0; r < mat.height(); r++) {
            for (size_t c = 0; c < mat.width(); c++) {
                result.extended_matrix[r][c] = mat[r][c];
            }
            result.extended_matrix[r][extended_col] = v[r];
        }

        result.rank = result.extended_matrix.triangler_inplace(true);

        for (size_t row = result.rank; row < mat.height(); row++) {
            // 矛盾があるので解なし
            bool no_answer = false;
            if constexpr (std::is_floating_point_v<T>)
                no_answer = abs(result.extended_matrix[row][extended_col]) > 1e-10;
            else
                no_answer = result.extended_matrix[row][extended_col] != 0;

            if (no_answer) {
                result.type = GaussJordanResult<T>::SolutionType::none;
                return result;
            }
        }
        if (result.rank == static_cast<int>(result.extended_matrix.width() - 1)) {
            result.type = GaussJordanResult<T>::SolutionType::one;
        } else {
            // some の時、実際の解の数は T に依存する
            // 自由変数 (パラメータ) の数が mat.width() - rank 個存在するため、
            // - もし T が mod_int なら解の数は 2 ** (mat.width() - rank)
            // - もし T が実数 (double など) なら無限
            result.type = GaussJordanResult<T>::SolutionType::some;
        }
        return result;
    }
