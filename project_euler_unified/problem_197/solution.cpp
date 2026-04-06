#include <bits/stdc++.h>
using namespace std;
int main() {
    double u = -1.0;
    for (int i = 0; i < 1000; i++)
        u = floor(pow(2.0, 30.403243784 - u * u)) * 1e-9;
    double u_n = u;
    double u_n1 = floor(pow(2.0, 30.403243784 - u * u)) * 1e-9;
    long long ans = (long long)floor((u_n + u_n1) * 1e9);
    assert(ans == 1710637717LL);
    cout << fixed << setprecision(9) << ans / 1e9 << endl;
    return 0;
}
