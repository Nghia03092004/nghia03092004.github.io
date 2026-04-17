#include <bits/stdc++.h>
using namespace std;

bool is_triangular(long long m) {
    if (m < 0) return false;
    long long d = 8LL * m + 1;
    long long s = (long long)sqrt((double)d);
    for (long long c = s - 1; c <= s + 1; c++)
        if (c >= 0 && c * c == d) return true;
    return false;
}

int main() {
    int N = 80;
    long long total = 0;
    for (int n = 0; n <= N; n++) {
        int count = 0;
        for (int a = 0; (long long)a * (a + 1) / 2 <= n; a++) {
            long long ta = (long long)a * (a + 1) / 2;
            for (int b = 0; (long long)b * (b + 1) / 2 <= n - ta; b++) {
                long long tb = (long long)b * (b + 1) / 2;
                if (is_triangular(n - ta - tb)) count++;
            }
        }
        total += count;
    }
    cout << total << endl;
    return 0;
}
