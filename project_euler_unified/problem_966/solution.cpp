/*
 * Problem 966: Abundant Number Chains
 * Count n <= 10^5 starting abundant chain of length >= 5.
 */
#include <bits/stdc++.h>
using namespace std;
int main() {
    const int N = 100000;
    const int M = 10000000;
    vector<long long> s(M + 1, 0);
    for (int d = 1; d <= M; d++)
        for (int m = 2*d; m <= M; m += d)
            s[m] += d;
    int count = 0;
    for (int n = 1; n <= N; n++) {
        long long cur = n;
        bool good = true;
        for (int step = 0; step < 5; step++) {
            if (cur > M || cur <= 0) { good = false; break; }
            long long nxt = s[(int)cur];
            if (nxt <= cur) { good = false; break; }
            cur = nxt;
        }
        if (good) count++;
    }
    cout << count << endl;
    return 0;
}
