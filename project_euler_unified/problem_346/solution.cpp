#include <bits/stdc++.h>
using namespace std;

int main() {
    const long long LIMIT = 1000000000000LL; // 10^12
    set<long long> repunits;

    // 1 is a repunit in every base
    repunits.insert(1);

    // For each number of digits k >= 3, enumerate repunits in base b >= 2
    // Every n >= 2 is "11" in base n-1, so any repunit with k>=3 digits
    // is automatically a strong repunit (repunit in at least 2 bases).
    for (int k = 3; k <= 40; k++) {
        for (long long b = 2; ; b++) {
            // Compute 1 + b + b^2 + ... + b^(k-1)
            long long val = 0;
            long long power = 1;
            bool overflow = false;
            for (int i = 0; i < k; i++) {
                val += power;
                if (val >= LIMIT) { overflow = true; break; }
                if (i < k - 1) {
                    // Check overflow before multiplying
                    if (power > LIMIT / b) { overflow = true; break; }
                    power *= b;
                }
            }
            if (overflow || val >= LIMIT) break;
            repunits.insert(val);
        }
    }

    long long sum = 0;
    for (long long v : repunits) {
        sum += v;
    }

    cout << sum << endl;
    return 0;
}
