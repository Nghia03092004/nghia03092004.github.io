#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

ll catalan(int n) {
    ll c = 1;
    for (int i = 0; i < n; i++) {
        c = c * 2 * (2*i + 1) / (i + 2);
    }
    return c;
}

int main() {
    for (int n = 0; n <= 20; n++)
        cout << "C(" << n << ") = " << catalan(n) << endl;
    return 0;
}
