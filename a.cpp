#include <iostream>
#include <random>

struct Node {
    Node * next;
    int data;

    Node() : next(nullptr), data(0) {}

    explicit Node(int d) : next(nullptr), data(d) {}
};

void qsort(Node * &first) {
    if (first == nullptr || first->next == nullptr) {
        return;
    }

    int sz = 0;
    int prev = first->data;
    bool sorted = true;
    Node * cur = first;
    while (cur != nullptr) {
        ++sz;

        if (prev > cur->data) {
            sorted = false;
        }
        prev = cur->data;

        cur = cur->next;
    }

    if (sorted) {
        return;
    }

    int m = rand() % sz;

    cur = first;
    while (m > 0) {
        --m;
        cur = cur->next;
    }
    m = cur->data;

    Node * left = nullptr;
    Node * curLeft = nullptr;
    Node * right = nullptr;
    Node * curRight = nullptr;

    int i = 0;
    while (first != nullptr) {
        if (first->data < m) {
            if (curLeft == nullptr) {
                left = curLeft = first;
            } else {
                curLeft = curLeft->next = first;
            }
        } else {
            if (curRight == nullptr) {
                right = curRight = first;
            } else {
                curRight = curRight->next = first;
            }
        }
        ++i;
        first = first->next;

        if (curLeft != nullptr) {
            curLeft->next = nullptr;
        }
        if (curRight != nullptr) {
            curRight->next = nullptr;
        }
    }
    curLeft = curRight = nullptr;

    qsort(left);
    qsort(right);

    if (left != nullptr) {
        first = left;
        while (left->next != nullptr) {
            left = left->next;
        }
        left->next = right;
    } else {
        first = right;
    }
}

int main() {
    size_t n = 0;
    std::cin >> n;

    Node * first = nullptr;
    Node * cur = nullptr;

    for (size_t i = 0; i != n; ++i) {
        Node * tmp = new Node();
        std::cin >> (tmp->data);

        if (first == nullptr) {
            cur = first = tmp;
        } else {
            cur = cur->next = tmp;
        }
    }

    qsort(first);

    cur = first;

    while (cur != nullptr) {
        std::cout << (cur->data) << " ";

        Node * tmp = cur;
        cur = cur->next;
        delete tmp;
    }
    std::cout << "\n";

    return 0;
}
