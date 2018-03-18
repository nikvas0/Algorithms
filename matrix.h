#include <algorithm>
#include <iostream>
#include <vector>

template  <typename T>
class Matrix {
private:
    std::size_t n, m;
    std::vector<T> v;

public:
    Matrix(size_t _n, size_t _m, T elem = 0) : n(_n), m(_m) {
        v.resize(n * m, elem);
    }

    explicit Matrix(const std::vector<std::vector<T> >& t) {
        n = t.size();
        m = t[0].size();
        v.reserve(n * m);
        for (const auto& line : t) {
            for (const auto& elem : line) {
                v.emplace_back(elem);
            }
        }
    }

    template <typename U>
    Matrix(size_t _n, size_t _m, const std::vector<U>& t)
            : n(_n), m(_m) {
        v.resize(n * m);
        std::copy(t.begin(), t.end(), v.begin());
    }

    Matrix(const Matrix& x) {
        n = x.size().first;
        m = x.size().second;
        v.resize(n * m);
        std::copy(x.v.begin(), x.v.end(), v.begin());
    }

    explicit Matrix(const Matrix&& that) {
        n = that.size().first;
        m = that.size().second;
        v = std::move(that.v);
    }

    const T& at(const size_t i, const size_t j) const {
        return v.at(i * m + j);
    }

    void set(size_t i, size_t j, const T& val) {
        v[i * m + j] = val;
    }

    void add(size_t i, size_t j, const T& val) {
        v[i * m + j] += val;
    }


    std::pair<size_t, size_t> size() const {
        return {n, m};
    }

    Matrix<T>& operator+= (const Matrix<T>& a) {
        for (size_t i = 0; i != n; ++i) {
            for (size_t j = 0; j != m; ++j) {
                this->v[m * i + j] += a.at(i, j);
            }
        }
        return *this;
    }

    Matrix<T>& operator*= (const Matrix<T>& x) {
        size_t k = x.size().second;
        Matrix<T> res(n, k, 0);

        for (size_t t = 0; t != m; ++t) {
            for (size_t i = 0; i != n; ++i) {
                for (size_t j = 0; j != k; ++j) {
                    res.add(i, j, this->at(i, t) * x.at(t, j));
                }
            }
        }
        *this = std::move(res);
        return *this;
    }

    template <typename N>
    Matrix<T>& operator*= (const N& x) {
        for (size_t i = 0; i != n; ++i) {
            for (size_t j = 0; j != m; ++j) {
                this->v[m * i + j] *= x;
            }
        }
        return *this;
    }

    Matrix<T>& operator= (const Matrix<T>& x) {
        this->n = x.size().first;
        this->m = x.size().second;
        v.resize(n * m, 0);
        std::copy(x.v.begin(), x.v.end(), v.begin());
        return *this;
    }

    Matrix<T> transposed() const {
        Matrix<T> tmp(this->m, this->n, 0);
        for (size_t i = 0; i != n; ++i) {
            for (size_t j = 0; j != m; ++j) {
                tmp.v[j * n + i] = this->at(i, j);
            }
        }
        return tmp;
    }

    Matrix<T>& transpose() {
        Matrix<T> tmp(this->m, this->n, 0);
        for (size_t i = 0; i != n; ++i) {
            for (size_t j = 0; j != m; ++j) {
                tmp.v[j * n + i] = this->at(i, j);
            }
        }
        *this = std::move(tmp);
        return *this;
    }

    typedef typename
    std::vector<T>::iterator matrix_iterator;

    matrix_iterator begin() {
        return v.begin();
    }

    typedef typename
    std::vector<T>::iterator iterator;

    matrix_iterator end() {
        return v.end();
    }

    typedef typename
    std::vector<T>::const_iterator matrix_const_iterator;

    matrix_const_iterator begin() const {
        return v.begin();
    }

    typedef typename
    std::vector<T>::const_iterator const_iterator;

    matrix_const_iterator end() const {
        return v.end();
    }

    void swap_rows(size_t i, size_t j) {
        std::swap_ranges(v.begin() + i * m,
                         v.begin() + (i + 1) * m,
                         v.begin() + j * m);
    }

    template <typename F>
    void apply_row(size_t i, F f) {
        std::transform(v.begin() + i * m,
                       v.begin() + (i + 1) * m,
                       v.begin() + i * m,
                       f);
    }

    template <typename U>
    void row_diff(size_t i, size_t j, const U& k) {
        std::transform(v.begin() + i * m,
                       v.begin() + (i + 1) * m,
                       v.begin() + j * m,
                       v.begin() + i * m,
                       [k](const T& a, const T& b) {
                           return a - k * b;
                       });
    }

    template <typename U>
    std::vector<U> solve(std::vector<U> b) const {
        Matrix<U> a(n, m, this->v);

        for (size_t col = 0; col != m; ++col) {
            U mx = 0;
            size_t mx_ind = 0;
            for (size_t row = col; row != n; ++row) {
                if (a.at(row, col) > mx) {
                    mx = a.at(row, col);
                    mx_ind = row;
                } else if (-a.at(row, col) > mx) {
                    mx = -a.at(row, col);
                    mx_ind = row;
                }
            }

            if (mx == 0) {
                continue;
            }

            a.swap_rows(col, mx_ind);
            std::swap(b[col], b[mx_ind]);

            mx = a.at(col, col);
            b[col] /= mx;
            a.apply_row(col, [mx](const U& t){
                return t / mx;
            });

            for (size_t row = 0; row != n; ++row) {
                if (row == col) {
                    continue;
                }
                b[row] -= b[col] * a.at(row, col);
                a.row_diff(row, col, a.at(row, col));
            }
        }

        return b;
    }
};

template <typename T>
Matrix<T> operator+ (const Matrix<T>& a) {
    return a * 1;
}

template <typename T>
Matrix<T> operator- (const Matrix<T>& a) {
    return a * (-1);
}

template <typename T>
Matrix<T> operator+ (const Matrix<T>& a, const Matrix<T>& b) {
    Matrix<T> result(a.size().first, a.size().second, 0);
    result += a;
    result += b;
    return result;
}

template <typename T>
Matrix<T> operator* (const Matrix<T>& a, const Matrix<T>& b) {
    Matrix<T> result(a);
    result *= b;
    return result;
}

template <typename T, typename N>
Matrix<T> operator* (const Matrix<T>& a, const N& b) {
    Matrix<T> result(a);
    result *= b;
    return result;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const Matrix<T>& m) {
    size_t rows = m.size().first;
    size_t cols = m.size().second;
    for (size_t i = 0; i != rows; ++i) {
        if (i != 0) {
            out << "\n";
        }
        for (size_t j = 0; j != cols; ++j) {
            if (j != 0) {
                out << "\t";
            }
            out << m.at(i, j);
        }
    }
    return out;
}

