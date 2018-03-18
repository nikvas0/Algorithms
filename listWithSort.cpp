#include <cstdlib>
#include <iostream>
#include <initializer_list>
#include <utility>
#include <random>

template<class T>
struct Node {
    Node * next;
    T data;

    Node() : next(nullptr) {}

    explicit Node(T&& d) : next(nullptr), data(std::forward(d)) {}

    explicit Node(const T& d) : next(nullptr), data(d) {}
};

template<class T>
class List {
public:
    Node<T> * first;

    List() : first(nullptr) {}

    explicit List(Node<T> * fst) : first(fst) {}

    explicit List(List<T>&& lst) {
        std::swap(first, lst.first);
    }

    ~List() {
        clear();
    }

    void clear() {
        while (first != nullptr) {
            Node<T> * tmp = first;
            first = first->next;
            delete tmp;
        }
    }

    size_t size() {
        size_t sz = 0;
        Node<T> * cur = first;
        while (cur != nullptr) {
            ++sz;
            cur = cur->next;
        }
        return sz;
    }

    void pushFront(const T& x) {
        Node<T> * tmp = new Node<T>(x);
        tmp->next = first;
        first = tmp;
    }

    Node<T> * releaseFirst() {
        Node<T> * tmp = first;
        first = nullptr;
        return tmp;
    }

    List<T>& operator= (List<T>&& rhs) {
        std::swap(first, rhs.first);
        return *this;
    }  
};

template <class T>
std::ostream& operator<< (std::ostream& out, const List<T>& l) {
    Node<T> * cur = l.first;
    while (cur != nullptr) {
        out << cur->data;
        if (cur->next != nullptr) {
            out << " ";
        }
        cur = cur->next;
    }
    return out;
}

template<class T>
void split(List<T>& lst, List<T>& a, List<T>& b, List<T>& c) {
    if (lst.first == nullptr) {
        return;
    }

    size_t sep_index = static_cast<size_t>(rand()) % lst.size();
    Node<T> * cur = lst.first;
    while (sep_index > 0) {
        --sep_index;
        cur = cur->next;
    }
    T& sep = cur->data;

    Node<T> * first = lst.first;
    Node<T> * left = nullptr;
    Node<T> * curLeft = nullptr;
    Node<T> * right = nullptr;
    Node<T> * curRight = nullptr;
    Node<T> * mid = nullptr;
    Node<T> * curMid = nullptr;

    while (first != nullptr) {
        if (first->data < sep) {
            if (curLeft == nullptr) {
                left = curLeft = first;
            } else {
                curLeft = curLeft->next = first;
            }
        } else if (first->data > sep) {
            if (curRight == nullptr) {
                right = curRight = first;
            } else {
                curRight = curRight->next = first;
            }
        } else {
            if (curMid == nullptr) {
                mid = curMid = first;
            } else {
                curMid = curMid->next = first;
            }
        }
        
        first = first->next;

        if (curLeft != nullptr) {
            curLeft->next = nullptr;
        }
        if (curRight != nullptr) {
            curRight->next = nullptr;
        }
        if (curMid != nullptr) {
            curMid->next = nullptr;
        }
    }
    curLeft = curRight = curMid = nullptr;
    lst.releaseFirst();
    a = List<T>(left);
    b = List<T>(mid);
    c = List<T>(right);
}

template<class T>
void merge(List<T>& a, List<T>& b, List<T>& c, List<T>& res) {
    Node<T> * first = a.first;
    Node<T> * cur = a.first;
    while (cur != nullptr && cur->next != nullptr) {
        cur = cur->next;
    }
    if (cur == nullptr) {
        first = b.first;
        cur = b.first;
    } else {
        cur = cur->next = b.first;
    }
    while (cur != nullptr && cur->next != nullptr) {
        cur = cur->next;
    }
    if (cur == nullptr) {
        first = c.first;
    } else {
        cur->next = c.first;
    }

    a.first = nullptr;
    b.first = nullptr;
    c.first = nullptr;

    res = List<T>(first);
}

template <class T>
void qsort(List<T>& lst) {
    if (lst.first == nullptr || lst.first->next == nullptr) {
        return;
    }
    List<T> lft;
    List<T> mid;
    List<T> rht;
    split<T>(lst, lft, mid, rht);
    qsort(lft);
    qsort(rht);
    merge<T>(lft, mid, rht, lst);
}

int main() {
    size_t n = 0;
    std::cin >> n;
    List<int> lst;
    for (size_t i = 0; i != n; ++i) {
        int x;
        std::cin >> x;
        lst.pushFront(x);
    }

    qsort(lst);
    std::cout << lst << "\n";
    return 0;
}
