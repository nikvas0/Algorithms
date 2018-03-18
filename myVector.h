#include <algorithm>
#include <utility>
#include <cstddef>
#include <iostream>

template <class T>
class Vector {
private:
    T * data;
    size_t sz;
    size_t cp;

public:
    size_t size() const {
        return sz;
    }

    size_t capacity() const {
        return cp;
    }

    bool empty() const {
        return sz == 0;
    }

    void reserve(size_t n) {
        if (cp >= n) {
            return;
        }

        T * data2 = static_cast<T*>(
                operator new (n * sizeof(T)));

        size_t i = 0;
        try {
            for (; i != sz; ++i) {
                new (data2 + i) T(data[i]);
            }
        } catch (...) {
            for (size_t j = 0; j != i; ++j) {
                data2[i].~T();
            }
            operator delete(data2);
            throw;
        }

        for (size_t j = 0; j != sz; ++j) {
            data[j].~T();
        }
        operator delete(data);
        data = data2;
        cp = n;
    }

    void clear() {
        for (size_t j = 0; j != sz; ++j) {
            data[j].~T();
        }
        sz = 0;
    }

    void resize(size_t count) {
        if (sz >= count) {
            for (size_t j = count; j != sz; ++j) {
                data[j].~T();
            }
            sz = count;
        } else if (cp < count) {
            T *data2 = static_cast<T*>(
                    operator new(count * sizeof(T)));

            size_t i = 0;
            try {
                for (; i < sz; ++i) {
                    new (data2 + i) T(data[i]);
                }
                for (; i < count; ++i) {
                    new (data2 + i) T();
                }
            } catch (...) {
                for (size_t j = 0; j != i; ++j) {
                    data2[i].~T();
                }
                operator delete(data2);
                throw;
            }

            for (size_t j = 0; j != sz; ++j) {
                data[j].~T();
            }
            operator delete(data);
            data = data2;
            sz = cp = count;
        } else {
            size_t i = sz;
            try {
                for (; i != count; ++i) {
                    new(data + i) T();
                }
            } catch (...) {
                for (size_t j = 0; j != i; ++j) {
                    data[j].~T();
                }
                sz = 0;
                throw;
            }
            sz = count;
        }
    }

    Vector() {
        sz = 0;
        cp = 0;
        data = nullptr;
    }

    explicit Vector(size_t count) {
        data = nullptr;
        sz = 0;
        cp = 0;
        resize(count);
    }

    Vector(const Vector& v) {
        data = nullptr;
        sz = 0;
        cp = 0;
        data = static_cast<T*>(
                operator new (v.size() * sizeof(T)));

        for (size_t i = 0; i < sz; ++i) {
            new (data + i) T(v.data[i]);
        }
        cp = sz = v.size();
    }

    Vector(Vector&& v) {
        cp = 0;
        sz = 0;
        data = nullptr;
        std::swap(data, v.data);
        std::swap(sz, v.sz);
        std::swap(cp, v.cp);
    }

    Vector& operator= (const Vector& rhs) {
        T * data2 = static_cast<T*>(
                operator new (rhs.size() * sizeof(T)));

        size_t i = 0;
        try {
            for (; i != rhs.size(); ++i) {
                new (data2 + i) T(rhs.data[i]);
            }
        } catch (...) {
            for (size_t j = 0; j != i; ++j) {
                data2[j].~T();
            }
            operator delete(data2);
            throw;
        }

        for (size_t j = 0; j != sz; ++j) {
            data[j].~T();
        }
        operator delete(data);
        data = data2;
        cp = sz = rhs.size();
        return *this;
    }

    Vector& operator= (Vector&& rhs) {
        std::swap(data, rhs.data);
        std::swap(sz, rhs.sz);
        std::swap(cp, rhs.cp);

        rhs.clear();
        operator delete(rhs.data);
        rhs.data = nullptr;
        rhs.sz = rhs.cp = 0;

        return *this;
    }

    ~Vector() {
        for (size_t j = 0; j != sz; ++j) {
            data[j].~T();
        }
        operator delete(data);
    }

    void push_back(const T& val) {
        if (cp == 0) {
            reserve(1);
        } else if (sz == cp) {
            reserve(cp * 2);
        }
        new (data + sz) T(val);
        ++sz;
    }

    void push_back(T&& val) {
        if (cp == 0) {
            reserve(1);
        } else if (sz == cp) {
            reserve(cp * 2);
        }

        new (data + sz) T(std::move(val));
        ++sz;
    }

    void pop_back() {
        --sz;
        data[sz].~T();
    }

    T* begin() {
        return data;
    }

    T* end() {
        return data + size();
    }

    T& operator[] (size_t index) {
        return data[index];
    }

    const T& operator[] (size_t index) const {
        return data[index];
    }
};
