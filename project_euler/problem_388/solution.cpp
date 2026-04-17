#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 388: Distinct Lines through Lattice Points
 *
 * D(N) = sum_{d=1}^{N} mu(d) * ((floor(N/d)+1)^3 - 1)
 *
 * Sub-linear computation via Mertens function M(x) = sum_{d<=x} mu(d).
 * Time: O(N^{2/3}), Space: O(N^{2/3}).
 */

using i64 = long long;
using i128 = __int128;

const i64 N = 10000000000LL;  // 10^10
int THRESH;

vector<int> mu_arr;
vector<i64> M_small;
unordered_map<i64, i64> M_cache;

void sieve_mu() {
    mu_arr.assign(THRESH + 1, 0);
    mu_arr[1] = 1;
    vector<bool> is_prime(THRESH + 1, true);
    vector<int> primes;
    for (int i = 2; i <= THRESH; i++) {
        if (is_prime[i]) {
            primes.push_back(i);
            mu_arr[i] = -1;
        }
        for (int p : primes) {
            if ((i64)i * p > THRESH) break;
            is_prime[i * p] = false;
            if (i % p == 0) {
                mu_arr[i * p] = 0;
                break;
            }
            mu_arr[i * p] = -mu_arr[i];
        }
    }
    M_small.assign(THRESH + 1, 0);
    for (int i = 1; i <= THRESH; i++)
        M_small[i] = M_small[i - 1] + mu_arr[i];
}

i64 mertens(i64 n) {
    if (n <= THRESH) return M_small[n];
    auto it = M_cache.find(n);
    if (it != M_cache.end()) return it->second;

    i64 result = 1;
    i64 k = 2;
    while (k <= n) {
        i64 q = n / k;
        i64 k_next = n / q + 1;
        result -= (k_next - k) * mertens(q);
        k = k_next;
    }
    M_cache[n] = result;
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Threshold ~ N^{2/3}
    THRESH = (int)cbrt((double)N * N) + 1;
    // More precise: N^{2/3}
    while ((i128)THRESH * THRESH * THRESH < (i128)N * N) THRESH++;

    sieve_mu();

    // D(N) = sum over blocks grouped by q = floor(N/d)
    i128 total = 0;
    i64 d = 1;
    while (d <= N) {
        i64 q = N / d;
        i64 d_max = N / q;
        i64 mu_sum = mertens(d_max) - mertens(d - 1);
        i128 g = (i128)(q + 1) * (q + 1) * (q + 1) - 1;
        total += (i128)mu_sum * g;
        d = d_max + 1;
    }

    // Format: first 9 digits + last 9 digits
    // Convert i128 to string
    string s;
    i128 val = total;
    if (val == 0) {
        s = "0";
    } else {
        bool neg = val < 0;
        if (neg) val = -val;
        while (val > 0) {
            s += '0' + (int)(val % 10);
            val /= 10;
        }
        if (neg) s += '-';
        reverse(s.begin(), s.end());
    }

    string answer = s.substr(0, 9) + s.substr(s.size() - 9);
    cout << answer << endl;
    return 0;
}
