#include <bits/stdc++.h>
using namespace std;

int main() {
    // Find all primes below 190
    vector<int> primes;
    for (int i = 2; i < 190; i++) {
        bool is_prime = true;
        for (int j = 2; j * j <= i; j++) {
            if (i % j == 0) { is_prime = false; break; }
        }
        if (is_prime) primes.push_back(i);
    }

    int n = primes.size(); // 42
    int half = n / 2;      // 21
    int other = n - half;   // 21

    const long long MOD = 10000000000000000LL; // 10^16

    // Compute log(sqrt(P)) using long double for precision
    long double log_sqrtP = 0;
    for (int p : primes) log_sqrtP += logl((long double)p);
    log_sqrtP /= 2.0L;

    // Modular multiplication using __int128
    auto mulmod = [&](long long a, long long b) -> long long {
        return (long long)((__int128)a * b % MOD);
    };

    // Generate all subset products for first half
    int sz1 = 1 << half;
    vector<long double> logA(sz1);
    vector<long long> modA(sz1);

    for (int mask = 0; mask < sz1; mask++) {
        long double lg = 0;
        long long val = 1;
        for (int i = 0; i < half; i++) {
            if (mask & (1 << i)) {
                lg += logl((long double)primes[i]);
                val = mulmod(val, primes[i]);
            }
        }
        logA[mask] = lg;
        modA[mask] = val;
    }

    // Sort by log value
    vector<int> idxA(sz1);
    iota(idxA.begin(), idxA.end(), 0);
    sort(idxA.begin(), idxA.end(), [&](int a, int b) { return logA[a] < logA[b]; });

    vector<long double> sortedLogA(sz1);
    vector<long long> sortedModA(sz1);
    for (int i = 0; i < sz1; i++) {
        sortedLogA[i] = logA[idxA[i]];
        sortedModA[i] = modA[idxA[i]];
    }

    // For each subset of second half, find best match
    int sz2 = 1 << other;
    long double best_log = -1;
    long long best_mod = 0;

    for (int mask = 0; mask < sz2; mask++) {
        long double lg = 0;
        long long val = 1;
        for (int i = 0; i < other; i++) {
            if (mask & (1 << i)) {
                lg += logl((long double)primes[half + i]);
                val = mulmod(val, primes[half + i]);
            }
        }

        long double target = log_sqrtP - lg;
        if (target < -1e-15) continue;

        // Binary search: find largest index where sortedLogA[idx] <= target
        int lo = 0, hi = sz1 - 1, pos = -1;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            if (sortedLogA[mid] <= target + 1e-15) {
                pos = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }

        if (pos < 0) continue;

        // Check a few candidates near pos for floating point safety
        for (int j = max(0, pos - 3); j <= min(sz1 - 1, pos + 3); j++) {
            if (sortedLogA[j] > target + 1e-15) continue;
            long double total_log = sortedLogA[j] + lg;
            if (total_log > best_log + 1e-15) {
                best_log = total_log;
                best_mod = mulmod(sortedModA[j], val);
            } else if (fabsl(total_log - best_log) < 1e-12) {
                // Tie: compare mod values... but we actually want the LARGER divisor.
                // When logs are equal within precision, we need true comparison.
                // Use the fact that if log(a*b) > log(c*d), then a*b > c*d.
                // If they're truly equal in log space within precision, we can't distinguish.
                // Try to pick the larger mod value (heuristic, since actual value matters mod 10^16).
                // Actually for the correct answer, the maximum is unique (no exact ties).
                // So one of the candidates will have strictly larger log.
                long long candidate = mulmod(sortedModA[j], val);
                if (candidate > best_mod) best_mod = candidate;
            }
        }
    }

    cout << best_mod << endl;
    return 0;
}
