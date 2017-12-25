#include <iostream>
#include <random>

struct Node {
    Node * next;
    int data;

    Node() : next(nullptr), data(0) {}

    explicit Node(int d) : next(nullptr), data(d) {}
};

void qsort(Node *& first) {
    if (first == nullptr || first->next == nullptr) {
        return;
    }

    int sz = 0;
    Node * cur = first;
    while (cur != nullptr) {
        ++sz;
        cur = cur->next;
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
    Node * mid = nullptr;
    Node * curMid = nullptr;

    while (first != nullptr) {
        if (first->data < m) {
            if (curLeft == nullptr) {
                left = curLeft = first;
            } else {
                curLeft = curLeft->next = first;
            }
        } else if (first->data > m) {
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

    qsort(left);
    qsort(right);

    // TODO:concatenate three lists to one
    first = left;
    cur = left;
    while (cur != nullptr && cur->next != nullptr) {
        cur = cur->next;
    }
    if (cur == nullptr) {
        first = mid;
        cur = mid;
    } else {
        cur = cur->next = mid;
    }
    while (cur != nullptr && cur->next != nullptr) {
        cur = cur->next;
    }
    cur->next = right;
}

int main() {
    size_t n = 0;
    std::cin >> n;

    Node * first = nullptr;
    Node * cur = nullptr;

    for (size_t i = 0; i != n; ++i) {
        Node * tmp = new Node();
        std::cin >> tmp->data;

        if (first == nullptr) {
            cur = first = tmp;
        } else {
            cur = cur->next = tmp;
        }
    }

    qsort(first);

    cur = first;

    while (cur != nullptr) {
        std::cout << cur->data << " ";

        Node * tmp = cur;
        cur = cur->next;
        delete tmp;
    }
    std::cout << "\n";

    return 0;
}
