#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

template <typename T>
class Polynomial {
private:
    int64_t degree;
    std::map<int64_t, T> a;

    Polynomial(const T& k, const int64_t& x) {
        if (k == static_cast<T>(0)) {
            degree = -1;
            return;
        }
        a[x] = k;
        degree = x;
    }

public:
    typedef typename
    std::map<int64_t, T>::const_iterator const_polynomial_iterator;
    typedef typename
    std::map<int64_t, T>::const_reverse_iterator const_polynomial_reiterator;

    Polynomial() {
        degree = -1;
    }

    Polynomial(const T &x) {
        if (x == static_cast<T>(0)) {
            degree = -1;
        } else {
            a[0] = x;
            degree = 0;
        }
    }

    Polynomial(const std::vector<T> &v) {
        degree = -1;
        for (int64_t i = 0; i < v.size(); ++i) {
            if (v[i] != static_cast<T>(0)) {
                degree = i;
                a[i] = v[i];
            }
        }
    }

    Polynomial(const std::map<int64_t, T> &m) {
        degree = -1;
        for (const auto &p : m) {
            if (p.second != static_cast<T>(0)) {
                degree = std::max(degree, p.first);
                a[p.first] = p.second;
            }
        }
    }

    template<typename Iter>
    Polynomial(Iter first, Iter last) {
        degree = -1;
        int64_t cnt = 0;
        while (first != last) {
            if (*first != static_cast<T>(0)) {
                degree = cnt;
                a[cnt] = static_cast<T>(*first);
            }
            ++first;
            ++cnt;
        }
    }

    int64_t Degree() const {
        return degree;
    }

    T operator[](int64_t x) const {
        if (a.find(x) == a.end()) {
            return static_cast<T>(0);
        }
        return a.at(x);
    }

    const_polynomial_iterator begin() const {
        return a.begin();
    }

    const_polynomial_iterator end() const {
        return a.end();
    }

    const_polynomial_reiterator rbegin() const {
        return a.rbegin();
    }

    const_polynomial_reiterator rend() const {
        return a.rend();
    }

    friend bool operator==(const Polynomial<T> &p1,
                           const Polynomial<T> &p2) {
        for (const auto &p : p1) {
            if (p2[p.first] != p.second) {
                return false;
            }
        }

        for (const auto &p : p2) {
            if (p1[p.first] != p.second) {
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
        std::map<int64_t, T> res;

        for (const auto& p : p1) {
            res[p.first] += p.second;
        }

        for (const auto& p : p2) {
            res[p.first] += p.second;
        }

        return Polynomial(res);
    }

    friend Polynomial operator-(const Polynomial &p1,
                                const Polynomial &p2) {
        std::map<int64_t, T> res;

        for (const auto& p : p1) {
            res[p.first] += p.second;
        }

        for (const auto& p : p2) {
            res[p.first] -= p.second;
        }

        return Polynomial(res);
    }

    friend Polynomial operator*(const Polynomial &p1,
                                const Polynomial &p2) {
        std::map<int64_t, T> res;

        for (const auto &x : p1) {
            for (const auto &y : p2) {
                res[x.first + y.first] += x.second * y.second;
            }
        }

        return Polynomial(res);
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
        U res = static_cast<U>(0);
        U y = static_cast<U>(1);

        for (int64_t i = 0; i <= this->Degree(); ++i) {
            res += y * (*this)[i];
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

    for (auto it = p.rbegin(); it != p.rend(); ++it) {
        int64_t cur_degree = it->first;
        if (p[cur_degree] == static_cast<T>(0)) {
            continue;
        }
        if (p[cur_degree] == static_cast<T>(-1)) {
            if (cur_degree == 0) {
                out << "-1";
            } else if (cur_degree == 1) {
                out << "-x";
            } else {
                out << "-x^" << cur_degree;
            }
        } else if (p[cur_degree] == static_cast<T>(1)) {
            if (cur_degree == 0) {
                if (cur_degree == p.Degree()) {
                    out << "1";
                } else {
                    out << "+1";
                }
            } else if (cur_degree == 1) {
                if (cur_degree == p.Degree()) {
                    out << "x";
                } else {
                    out << "+x";
                }
            } else {
                if (cur_degree == p.Degree()) {
                    out << "x^" << cur_degree;
                } else {
                    out << "+x^" << cur_degree;
                }
            }
        } else if (p[cur_degree] < static_cast<T>(0)) {
            if (cur_degree == 0) {
                out << p[cur_degree];
            } else if (cur_degree == 1) {
                out << p[cur_degree] << "*x";
            } else {
                out << p[cur_degree]
                    << "*x^" << cur_degree;
            }
        } else {
            if (cur_degree == 0) {
                if (cur_degree == p.Degree()) {
                    out << p[cur_degree];
                } else {
                    out << "+" << p[cur_degree];
                }
            } else if (cur_degree == 1) {
                if (cur_degree == p.Degree()) {
                    out << p[cur_degree] << "*x";
                } else {
                    out << "+"
                        << p[cur_degree] << "*x";
                }
            } else {
                if (cur_degree == p.Degree()) {
                    out << p[cur_degree]
                        << "*x^" << cur_degree;
                } else {
                    out << "+" << p[cur_degree]
                        << "*x^" << cur_degree;
                }
            }
        }
    }
    return out;
}
