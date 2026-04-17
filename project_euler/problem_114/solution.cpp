#include <bits/stdc++.h>
using namespace std;

int main() {
    const int N = 50;
    // f[i+1] stores f(i), so f(−1) is at index 0, f(0) at index 1, etc.
    vector<long long> f(N + 2, 0);
    vector<long long> P(N + 2, 0);
    f[0] = 1;  // f(-1)
    f[1] = 1;  // f(0)
    P[0] = 1;  // P(-1)
    P[1] = 2;  // P(0)

    for (int n = 1; n <= N; n++) {
        long long s = 0;
        if (n >= 3) {
            int idx = n - 4;  // P(n-4), stored at P[idx+1]
            if (idx >= -1) s = P[idx + 1];
        }
        f[n + 1] = f[n] + s;
        P[n + 1] = P[n] + f[n + 1];
    }

    cout << f[N + 1] << endl;
    return 0;
}
