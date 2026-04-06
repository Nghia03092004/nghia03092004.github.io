#include <bits/stdc++.h>
using namespace std;

int main() {
    // Parameters
    const int N = 100;      // Change to 100000000 for full problem
    const int K = 10;       // Change to 10000 for full problem
    const int L = N + K - 1;

    // Divisor count sieve
    vector<int> d(L + 1, 0);
    for (int j = 1; j <= L; j++) {
        for (int m = j; m <= L; m += j) {
            d[m]++;
        }
    }

    // Sliding window maximum using monotone deque
    deque<int> dq;
    long long total = 0;

    for (int i = 1; i <= L; i++) {
        while (!dq.empty() && d[dq.back()] <= d[i])
            dq.pop_back();
        dq.push_back(i);

        int n = i - K + 1;
        if (n >= 1) {
            while (dq.front() < n)
                dq.pop_front();
            total += d[dq.front()];
        }
    }

    cout << "Sum M(n," << K << ") for n=1.." << N << " = " << total << endl;

    // For N=100, K=10: expected 432
    if (N == 100 && K == 10) {
        assert(total == 432);
    }

    return 0;
}
