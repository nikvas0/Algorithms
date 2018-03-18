#include <cstddef>
#include <utility>
#include <tuple>
#include <memory>

template<class T, class Deleter = std::default_delete<T>>
class UniquePtr {
private:
    std::tuple<T*, Deleter> t;

public:
    UniquePtr() noexcept : t(nullptr, Deleter()) {}

    UniquePtr(T * _ptr) noexcept : t(_ptr, Deleter()) {}

    UniquePtr(T * _ptr, const Deleter& _deleter) noexcept
            : t(_ptr, _deleter) {}

    UniquePtr(const UniquePtr&) = delete;

    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& rval) noexcept {
        reset(rval.release());
        std::get<1>(t) = std::move(std::get<1>(rval.t));
    }

    UniquePtr& operator=(std::nullptr_t) noexcept {
        reset(nullptr);
        return *this;
    }

    UniquePtr& operator=(UniquePtr&& rval) noexcept {
        reset(rval.release());
        std::get<1>(t) = std::move(std::get<1>(rval.t));
        return *this;
    }

    ~UniquePtr() noexcept {
        if (std::get<0>(t) != nullptr) {
            std::get<1>(t)(std::get<0>(t));
        }
    }

    T& operator* () const {
        return *std::get<0>(t);
    }

    T& operator* () {
        return *std::get<0>(t);
    }

    T * operator-> () noexcept {
        return std::get<0>(t);
    }

    T * operator-> () const noexcept {
        return std::get<0>(t);
    }

    T * release() noexcept {
        T * tmp = std::get<0>(t);
        std::get<0>(t) = nullptr;
        return  tmp;
    }

    void reset(T * _ptr) noexcept {
        if (std::get<0>(t) != nullptr) {
            std::get<1>(t)(std::get<0>(t));
        }
        std::get<0>(t) = _ptr;
    }

    void swap(UniquePtr& other) noexcept {
        std::swap(t, other.t);
    }

    T * get() const noexcept {
        return std::get<0>(t);
    }

    const Deleter& get_deleter() const noexcept {
        return std::get<1>(t);
    }

    Deleter& get_deleter() noexcept {
        return std::get<1>(t);
    }

    explicit operator bool() const noexcept {
        return std::get<0>(t) != nullptr;
    }
};
