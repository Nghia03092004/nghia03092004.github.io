#include <bits/stdc++.h>
using namespace std;

int main() {
    // sigma(a,b,c,d,e,f) = (b,c,d,e,f, a XOR (b AND c))
    // Encode (a,b,c,d,e,f) as a 6-bit integer: a*32 + b*16 + c*8 + d*4 + e*2 + f

    auto sigma = [](int x) -> int {
        int a = (x >> 5) & 1;
        int b = (x >> 4) & 1;
        int c = (x >> 3) & 1;
        int d = (x >> 2) & 1;
        int e = (x >> 1) & 1;
        int f = x & 1;
        int new_f = a ^ (b & c);
        return (b << 5) | (c << 4) | (d << 3) | (e << 2) | (f << 1) | new_f;
    };

    // Find cycle structure
    vector<bool> visited(64, false);
    vector<int> cycle_lengths;

    for (int i = 0; i < 64; i++) {
        if (visited[i]) continue;
        int len = 0;
        int cur = i;
        while (!visited[cur]) {
            visited[cur] = true;
            cur = sigma(cur);
            len++;
        }
        cycle_lengths.push_back(len);
    }

    // For each cycle of length n, count independent sets = Lucas(n)
    // Lucas(1) = 1, Lucas(2) = 3, Lucas(n) = Lucas(n-1) + Lucas(n-2)
    // But Lucas(1) should be 1 for self-loops (tau(x) AND tau(x) = 0 => tau(x) = 0)
    // Standard Lucas: L(1)=1, L(2)=3, L(n)=L(n-1)+L(n-2)

    auto lucas = [](int n) -> long long {
        if (n == 1) return 1;
        if (n == 2) return 3;
        long long a = 1, b = 3;
        for (int i = 3; i <= n; i++) {
            long long c = a + b;
            a = b;
            b = c;
        }
        return b;
    };

    long long answer = 1;
    for (int len : cycle_lengths) {
        answer *= lucas(len);
    }

    cout << answer << endl;
    return 0;
}
