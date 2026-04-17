#include <bits/stdc++.h>
using namespace std;

int main() {
    const int N = 50;
    long long answer = 0;

    // For each tile length L in {2, 3, 4}, compute f_L(N) - 1
    for (int L = 2; L <= 4; L++) {
        vector<long long> f(N + 1, 0);
        for (int i = 0; i < L && i <= N; i++) f[i] = 1;
        for (int i = L; i <= N; i++) {
            f[i] = f[i - 1] + f[i - L];
        }
        answer += f[N] - 1;
    }

    cout << answer << endl;
    return 0;
}
