#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef __int128 lll;

// Get digit counts of a number
vector<int> digit_counts(ll n) {
    vector<int> cnt(10, 0);
    if (n == 0) { cnt[0] = 1; return cnt; }
    while (n > 0) {
        cnt[n % 10]++;
        n /= 10;
    }
    return cnt;
}

int num_digits(ll n) {
    if (n == 0) return 1;
    int d = 0;
    while (n > 0) { d++; n /= 10; }
    return d;
}

int main() {
    const int D = 16;
    set<ll> near_power_sums;

    // Precompute powers: i^k for i=0..9, k=1..20
    // Use __int128 to avoid overflow during computation
    // But final values must fit in range [1, 10^16]

    for (int k = 1; k <= 20; k++) {
        // Precompute i^k
        lll pw[10];
        pw[0] = 0;
        bool too_big = false;
        for (int i = 1; i <= 9; i++) {
            pw[i] = 1;
            for (int j = 0; j < k; j++) {
                pw[i] *= i;
                if (pw[i] > (lll)1e18) { too_big = true; break; }
            }
            if (too_big) break;
        }
        if (too_big) break;

        // Check if D * 9^k is reasonable
        if (pw[9] > (lll)1e17 / D) {
            // Even a single 9 might make sum too large
            // But we can still check - the sum just needs to be <= 10^16
        }

        // Enumerate digit multisets using recursion
        // c[0..9] with sum(c) in [1, D]
        // Compute power_sum = sum(c[i] * pw[i])

        vector<int> c(10, 0);

        function<void(int, int, lll)> enumerate = [&](int digit, int remaining, lll power_sum) {
            if (digit == 10) {
                if (remaining == D) return; // no digits used
                // Check power_sum +/- 1
                for (int delta : {-1, 1}) {
                    lll candidate = power_sum + delta;  // This is n = power_sum +/- 1
                    // But we need: sum of k-th powers of digits of n = n -/+ 1 = power_sum
                    // Actually: n is near power sum if digit_power_sum(n) = n +/- 1
                    // So digit_power_sum(n) = power_sum, and n = power_sum -/+ 1? No.
                    // Let me re-read: n is near power sum if sum of k-th powers of digits of n = n+1 or n-1
                    // So: power_sum = n+1 or power_sum = n-1
                    // Thus: n = power_sum - 1 or n = power_sum + 1
                    // But power_sum is computed from c[], which should be the digit counts of n

                    ll n_val;
                    if (delta == -1) {
                        // n = power_sum - 1 (case: digit_power_sum = n + 1)
                        if (power_sum < 1) continue;
                        if (power_sum - 1 > (lll)1e16) continue;
                        n_val = (ll)(power_sum - 1);
                    } else {
                        // n = power_sum + 1 (case: digit_power_sum = n - 1)
                        if (power_sum + 1 > (lll)1e16) continue;
                        if (power_sum + 1 < 1) continue;
                        n_val = (ll)(power_sum + 1);
                    }

                    if (n_val < 1) continue;
                    int nd = num_digits(n_val);
                    int total_digits = D - remaining;
                    if (nd != total_digits) continue;

                    // Check digit counts match
                    vector<int> dc = digit_counts(n_val);
                    bool match = true;
                    for (int i = 0; i < 10; i++) {
                        if (dc[i] != c[i]) { match = false; break; }
                    }
                    if (match) {
                        near_power_sums.insert(n_val);
                    }
                }
                return;
            }

            int max_c = remaining;
            for (int cnt = 0; cnt <= max_c; cnt++) {
                c[digit] = cnt;
                lll new_sum = power_sum + (lll)cnt * pw[digit];
                if (digit > 0 && new_sum > (lll)1e16 + 1) {
                    c[digit] = 0;
                    break;
                }
                enumerate(digit + 1, remaining - cnt, new_sum);
                c[digit] = 0;
            }
        };

        enumerate(0, D, 0);
    }

    ll answer = 0;
    for (ll x : near_power_sums) {
        answer += x;
    }

    cout << answer << endl;

    return 0;
}
