#include <bits/stdc++.h>
using namespace std;

int main() {
    // g[n] = number of valid strings of length n over {O,A} with no "AAA"
    // Recurrence: g[n] = g[n-1] + g[n-2] + g[n-3]
    const int N = 30;
    vector<long long> g(N + 1);
    g[0] = 1; g[1] = 2; g[2] = 4;
    for (int i = 3; i <= N; i++)
        g[i] = g[i-1] + g[i-2] + g[i-3];

    // Strings with 0 L's
    long long ans = g[N];

    // Strings with exactly 1 L: place L at position k, 0-indexed
    // prefix of length k and suffix of length N-1-k must both be valid no-L strings
    for (int k = 0; k < N; k++)
        ans += g[k] * g[N - 1 - k];

    cout << ans << endl;
    return 0;
}
