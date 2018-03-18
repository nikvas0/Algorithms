#include <algorithm>
#include <iostream>
#include <vector>

template <typename T>
class Polynomial {
private:
    int64_t degree;
    std::vector<T> a;

public:
    typedef typename
    std::vector<T>::const_iterator const_polynomial_iterator;

    Polynomial() {
        a.resize(0);
        degree = -1;
    }

    Polynomial(const T &x) {
        a.resize(1, x);
        if (x == static_cast<T>(0)) {
            degree = -1;
        } else {
            degree = 0;
        }
    }

    Polynomial(const std::vector<T> &v) {
        a.resize(v.size(), static_cast<T>(0));
        std::copy(v.begin(), v.end(), a.begin());
        degree = -1;
        for (size_t i = 0; i < a.size(); ++i) {
            if (a[i] != static_cast<T>(0)) {
                degree = i;
            }
        }
    }

    Polynomial(const T& k, const int64_t& x) {
        a.resize(x + 1, static_cast<T>(0));
        a[x] = k;
        degree = -1;
        for (size_t i = 0; i < a.size(); ++i) {
            if (a[i] != static_cast<T>(0)) {
                degree = i;
            }
        }
    }

    template<typename Iter>
    Polynomial(Iter first, Iter last) {
        a.clear();
        degree = -1;
        int64_t cnt = 0;
        while (first != last) {
            a.push_back(*(first++));
            if (a.back() != static_cast<T>(0)) {
                degree = cnt;
            }
            ++cnt;
        }
    }

    int64_t Degree() const {
        return degree;
    }

    T operator[](int64_t x) const {
        if (x > degree) {
            return static_cast<T>(0);
        }

        if (x < 0) {
            return static_cast<T>(0);
        }
        return a[x];
    }

    const_polynomial_iterator begin() const {
        return a.begin();
    }

    const_polynomial_iterator end() const {
        return a.begin() + (1 + degree);
    }

    friend bool operator==(const Polynomial<T> &p1,
                           const Polynomial<T> &p2) {
        for (int64_t i = 0;
             i <= std::max(p1.Degree(), p2.Degree());
             ++i) {
            if (p1[i] != p2[i]) {
                return false;
            }
        }
        return true;
    }

    friend bool operator!=(const Polynomial &p1,
                           const Polynomial &p2) {
        return !(p1 == p2);
    }

    friend Polynomial operator+(const Polynomial &p1,
                                const Polynomial &p2) {
        std::vector<T> res(std::max(p1.Degree(), p2.Degree()) + 1,
                           static_cast<T>(0));
        for (int64_t i = 0;
             i < std::max(p1.Degree(), p2.Degree()) + 1;
             ++i) {
            res[i] = p1[i] + p2[i];
        }
        return Polynomial(res);
    }

    friend Polynomial operator-(const Polynomial &p1,
                                const Polynomial &p2) {
        std::vector<T> res(std::max(p1.Degree(), p2.Degree()) + 1,
                           static_cast<T>(0));
        for (int64_t i = 0;
             i < std::max(p1.Degree(), p2.Degree()) + 1;
             ++i) {
            res[i] = p1[i] - p2[i];
        }
        return Polynomial(res);
    }

    friend Polynomial operator*(const Polynomial &p1,
                                const Polynomial &p2) {
        std::vector<T> res(p1.Degree() + 1 + p2.Degree() + 1,
                           static_cast<T>(0));

        for (int64_t i = 0; i <= p1.Degree(); ++i) {
            for (int64_t j = 0; j <= p2.Degree(); ++j) {
                res[i + j] += p1[i] * p2[j];
            }
        }

        return res;
    }

    friend Polynomial operator/ (const Polynomial &a, const Polynomial &b) {
        Polynomial q(static_cast<T>(0));
        Polynomial r = a;
        while (r.Degree() >= b.Degree()) {
            Polynomial k = Polynomial(
                r[r.Degree()] / b[b.Degree()]
                , r.Degree() - b.Degree());

            q += k;
            r -= k * b;
        }
        return q;
    }

    friend Polynomial operator% (const Polynomial &a, const Polynomial &b) {
        Polynomial q(static_cast<T>(0));
        Polynomial r = a;
        while (r.Degree() >= b.Degree()) {
            Polynomial k = Polynomial(
                r[r.Degree()] / b[b.Degree()]
                , r.Degree() - b.Degree());

            q += k;
            r -= k * b;
        }
        return r;
    }

    friend Polynomial operator, (Polynomial a, Polynomial b) {
        while (b != static_cast<T>(0)) {
            Polynomial tmp = a % b;
            a = std::move(b);
            b = std::move(tmp);
        }
        return a / a[a.Degree()];
    }


    Polynomial& operator+= (const Polynomial &p) {
        return *this = *this + p;
    }

    Polynomial& operator-= (const Polynomial &p) {
        return *this = *this - p;
    }

    Polynomial& operator*= (const Polynomial &p) {
        return *this = *this * p;
    }

    template <typename U>
    U operator() (const U &x) const {
        U res = static_cast<T>(0);
        U y = static_cast<T>(1);
        for (int64_t i = 0; i <= (*this).degree; ++i) {
            res += (*this)[i] * y;
            y *= x;
        }
        return res;
    }

    Polynomial operator& (const Polynomial &p) const {
        return (*this)(p);
    }
};

template <typename T>
std::ostream &operator<<(std::ostream &out,
                         const Polynomial<T> &p) {
    if (p.Degree() == -1) {
        return out << 0;
    }

    for (int64_t i = p.Degree(); i != -1; --i) {
        if (p[i] == static_cast<T>(0)) {
            continue;
        }
        if (p[i] == static_cast<T>(-1)) {
            if (i == 0) {
                out << "-1";
            } else if (i == 1) {
                out << "-x";
            } else {
                out << "-x^" << i;
            }
        } else if (p[i] == static_cast<T>(1)) {
            if (i == 0) {
                if (i == p.Degree()) {
                    out << "1";
                } else {
                    out << "+1";
                }
            } else if (i == 1) {
                if (i == p.Degree()) {
                    out << "x";
                } else {
                    out << "+x";
                }
            } else {
                if (i == p.Degree()) {
                    out << "x^" << i;
                } else {
                    out << "+x^" << i;
                }
            }
        } else if (p[i] < static_cast<T>(0)) {
            if (i == 0) {
                out << p[i];
            } else if (i == 1) {
                out << p[i] << "*x";
            } else {
                out << p[i] << "*x^" << i;
            }
        } else {
            if (i == 0) {
                if (i == p.Degree()) {
                    out << p[i];
                } else {
                    out << "+" << p[i];
                }
            } else if (i == 1) {
                if (i == p.Degree()) {
                    out << p[i] << "*x";
                } else {
                    out << "+" << p[i] << "*x";
                }
            } else {
                if (i == p.Degree()) {
                    out << p[i] << "*x^" << i;
                } else {
                    out << "+" << p[i] << "*x^" << i;
                }
            }
        }
    }
    return out;
}
