#include <functional>
#include <iostream>
#include <list>
#include <numeric>
#include <stdexcept>
#include <vector>

const size_t TABLESIZE = 2;

template<class KeyType, class ValueType, class Hash = std::hash<KeyType> > 
class HashMap {
public:
    using iterator = typename std::list<std::pair<const KeyType, ValueType> >::iterator;
    using const_iterator = typename std::list<std::pair<const KeyType, ValueType> >::const_iterator;

    explicit HashMap(Hash hasher = Hash())
    : lst()
    , sz(0)
    , table(TABLESIZE, {lst.end(), 0})
    , hasher(hasher) {}

    template<class Iter>
    HashMap(Iter first, Iter last, Hash hasher = Hash())
    : lst()
    , sz(0)
    , table(TABLESIZE, {lst.end(), 0})
    , hasher(hasher) {
        while (first != last) {
            insert(*first);
            ++first;
        }
    }

    explicit HashMap(std::initializer_list<std::pair<KeyType, ValueType> > l, Hash hasher = Hash())
    : lst()
    , sz(0)
    , table(TABLESIZE, {lst.end(), 0})
    , hasher(hasher) {
        for (const auto& elem : l) {
            insert(elem);
        }
    }

    HashMap(const HashMap<KeyType, ValueType, Hash>& hm)
    : lst()
    , sz(0)
    , table(TABLESIZE, {lst.end(), 0})
    , hasher(hm.hash_function()) {
        for (const auto& elem : hm) {
            insert(elem);
        }
    }

    HashMap(HashMap<KeyType, ValueType, Hash>&& hm)
    : lst()
    , sz(0)
    , table(TABLESIZE, {lst.end(), 0})
    , hasher(Hash()) {
        std::swap(table, hm.table);
        std::swap(lst, hm.lst);
        std::swap(sz, hm.sz);
        std::swap(hasher, hm.hasher);
    }

    HashMap<KeyType, ValueType, Hash>& operator=(HashMap<KeyType, ValueType, Hash> rhs) {
        std::swap(table, rhs.table);
        std::swap(lst, rhs.lst);
        std::swap(sz, rhs.sz);
        std::swap(hasher, rhs.hasher);
        return *this;
    }

    HashMap<KeyType, ValueType, Hash>& operator=(HashMap<KeyType, ValueType, Hash>&& rhs) {
        std::swap(table, rhs.table);
        std::swap(lst, rhs.lst);
        std::swap(sz, rhs.sz);
        std::swap(hasher, rhs.hasher);
        return *this;
    }

    size_t size() const {
        return sz;
    }

    bool empty() const {
        return sz == 0;
    }

    Hash hash_function() const {
        return hasher;
    }

    void rebuild(size_t newSize) {
        table.assign(newSize, {lst.end(), 0});
        std::list<std::pair<const KeyType, ValueType> > tmp;
        std::swap(tmp, lst);
        sz = 0;
        for (const auto& elem : tmp) {
            insert(elem);
        }
    }

    iterator find(const KeyType& key) {
        size_t hash = hasher(key) % table.size();
        auto it = table[hash].first;
        auto len = table[hash].second;
        size_t i = 0;
        while (it != lst.end() && i != len) {
            if (it->first == key) {
                return it;
            }
            ++it;
            ++i;
        }
        return lst.end();
    }

    const_iterator find(const KeyType& key) const {
        size_t hash = hasher(key) % table.size();
        auto it = table[hash].first;
        auto len = table[hash].second;
        size_t i = 0;
        while (it != lst.end() && i != len) {
            if (it->first == key) {
                return it;
            }
            ++it;
            ++i;
        }
        return lst.end();
    }

    iterator insert(const std::pair<KeyType, ValueType>& p) {
        if ((sz * 2) > table.size()) {
            rebuild(table.size() * 2);
        }
        const KeyType & key = p.first;
        size_t hash = hasher(key) % table.size();
        auto it = table[hash].first;
        auto len = table[hash].second;
        size_t i = 0;
        while (it != lst.end() && i != len) {
            if (it->first == key) {
                return end();
            }
            ++it;
            ++i;
        }
        auto newIter = lst.insert(it, p);
        if (table[hash].first == lst.end()) {
            table[hash].first = newIter;
        }
        ++table[hash].second;
        ++sz;
        return newIter;
    }

    void erase(const KeyType& key) {
        size_t hash = hasher(key) % table.size();
        auto it = table[hash].first;
        auto len = table[hash].second;
        size_t i = 0;
        while (it != lst.end() && i != len) {
            if (it->first == key) {
                if (table[hash].first == it) {
                    auto next = it;
                    ++next;
                    if (next != lst.end() && i + 1 < len) {
                        table[hash].first = next;
                    } else {
                        table[hash].first = lst.end();
                    }
                }
                lst.erase(it);
                --table[hash].second;
                --sz;
                return;
            }
            ++it;
            ++i;
        }
        return;
    }

    iterator begin() {
        return lst.begin();
    }

    const_iterator begin() const {
        return lst.begin();
    }

    iterator end() {
        return lst.end(); 
    }

    const_iterator end() const {
        return lst.end();
    }

    ValueType& operator[] (const KeyType& key) {
        auto it = find(key);
        if (it == end()) {
            return insert({key, ValueType()})->second;
        }
        return it->second;
    }

    const ValueType& at(const KeyType& key) const {
        auto it = find(key);
        if (it == end()) {
            throw std::out_of_range("");
        }
        return it->second;
    }

    void clear() {
        table.assign(TABLESIZE, {lst.end(), 0});
        lst.clear();
        sz = 0;
    }

private:
    std::list<std::pair<const KeyType, ValueType> > lst;
    size_t sz;
    std::vector<std::pair<iterator, size_t>> table;
    Hash hasher;
};
