#include <bits/stdc++.h>
using namespace std;

int A(int n) {
    // Find smallest k such that R(k) % n == 0
    // R(k) = (10*R(k-1) + 1)
    int r = 1;
    int k = 1;
    while (r != 0) {
        r = (10LL * r + 1) % n;
        k++;
    }
    return k;
}

int main() {
    int limit = 1000000;

    for (int n = limit + 1; ; n++) {
        if (n % 2 == 0 || n % 5 == 0) continue;
        if (A(n) > limit) {
            cout << n << endl;
            return 0;
        }
    }

    return 0;
}
