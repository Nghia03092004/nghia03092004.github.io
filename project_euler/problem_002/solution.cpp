#include <bits/stdc++.h>
using namespace std;

int main() {
    long long a = 2, b = 8, s = 0;
    while (a <= 4000000) {
        s += a;
        long long c = 4 * b + a;
        a = b;
        b = c;
    }
    cout << s << endl;
    return 0;
}
