#include <bits/stdc++.h>
using namespace std;

int main() {
    // D(n) = n(n-1)(n+1)(3n+2) / 12
    long long n = 100;
    long long ans = n * (n - 1) * (n + 1) * (3 * n + 2) / 12;
    cout << ans << endl;
    return 0;
}
