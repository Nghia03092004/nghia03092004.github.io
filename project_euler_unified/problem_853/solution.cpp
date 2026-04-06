#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int pisano_period(int m) {
    if (m == 1) return 1;
    int a = 0, b = 1;
    for (int i = 1; i <= 6*m; i++) {
        int c = (a + b) % m;
        a = b; b = c;
        if (a == 0 && b == 1) return i;
    }
    return -1;
}

int main() {
    assert(pisano_period(2) == 3);
    assert(pisano_period(5) == 20);
    assert(pisano_period(10) == 60);

    int N = 1000;
    ll total = 0;
    for (int m = 2; m <= N; m++) total += pisano_period(m);
    cout << total << endl;
    return 0;
}
