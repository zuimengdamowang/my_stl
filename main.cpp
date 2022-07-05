#include <iostream>
#include "include/algorithm.h"
#include "include/list.h"
#include "include/deque.h"

using std::cout;
using std::endl;

template <class T>
struct print {
    void operator()(const T &x) {
        cout << x << endl;
    }
};

struct test_class {
    int x = 0;
    double y = 9.1;
    test_class() {
        cout << "default constructor" << endl;
    }
    test_class(int _x, double _y): x(_x), y(_y) {
        cout << "constructor" << endl;
    }
};

int main() {

    tt::deque<int> d1;
    for (int i = 0; i < 1600; ++i) {
        cout << i << endl;
        d1.push_back(i);
    }
    cout << "###" << endl;



    return 0;
}
