#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 lll;

// Problem 273: Sum of Squares
//
// For a squarefree N that is a product of primes p = 1 mod 4 with p < 150,
// let S(N) = sum of all values of a where a^2 + b^2 = N, 0 <= a <= b.
// Find the sum of S(N) over all such N.
//
// Primes p = 1 mod 4 less than 150: 5, 13, 17, 29, 37, 41, 53, 61, 73, 89, 97, 101, 109, 113, 137, 149
// There are 16 such primes, giving 2^16 - 1 = 65535 squarefree products.

typedef pair<ll, ll> gi;

gi multiply(gi a, gi b) {
    return {a.first * b.first - a.second * b.second,
            a.first * b.second + a.second * b.first};
}

gi conj(gi a) {
    return {a.first, -a.second};
}

void print128(lll v) {
    if (v == 0) { cout << "0" << endl; return; }
    bool neg = false;
    if (v < 0) { neg = true; v = -v; }
    string s;
    while (v > 0) { s += ('0' + (int)(v % 10)); v /= 10; }
    if (neg) s += '-';
    reverse(s.begin(), s.end());
    cout << s << endl;
}

int main() {
    // Find primes p = 1 mod 4 less than 150
    vector<int> primes;
    for (int p = 2; p < 150; p++) {
        bool is_p = true;
        for (int d = 2; d * d <= p; d++)
            if (p % d == 0) { is_p = false; break; }
        if (is_p && p % 4 == 1) primes.push_back(p);
    }
    int np = primes.size(); // 16

    // For each prime, find Gaussian integer factor pi = a + bi with a^2 + b^2 = p, a <= b
    vector<gi> prime_gi(np);
    for (int i = 0; i < np; i++) {
        int p = primes[i];
        for (int a = 1; a * a < p; a++) {
            int b2 = p - a * a;
            int b = (int)round(sqrt(b2));
            if (b * b == b2 && a <= b) {
                prime_gi[i] = {a, b};
                break;
            }
        }
    }

    // For each non-empty subset of the 16 primes (2^16 - 1 subsets),
    // compute all representations a^2 + b^2 = N and sum the a values (with a <= b).
    //
    // For a product of k primes, there are 2^(k-1) essentially distinct representations
    // (the factor of 2 reduction is because conjugating ALL factors gives (a,-b) -> same {a,b}).
    //
    // We use the Gaussian integer product: for each subset, multiply the Gaussian factors
    // choosing conjugate or not for each. alpha = product -> a = Re(alpha), b = Im(alpha).
    // We want 0 <= a <= b.
    //
    // Optimization: enumerate subsets and for each, iterate over 2^(k-1) sign masks.
    // With k up to 16, 2^15 = 32768 per subset, and 65535 subsets, total ~ 2 * 10^9.
    // That's too slow for brute force.
    //
    // Better: build incrementally. Use DP where state is the current set of representations.
    // For each new prime, combine existing representations with the new prime's representation.
    //
    // For a single prime p = a^2 + b^2: representations are {(a,b)} (just one with a<=b).
    // When multiplying two representations:
    // (a^2+b^2)(c^2+d^2) = (ac-bd)^2 + (ad+bc)^2 = (ac+bd)^2 + (|ad-bc|)^2
    //
    // So from one existing representation (a,b) and one new prime representation (c,d),
    // we get two new representations: (|ac-bd|, ad+bc) and (|ad-bc|, ac+bd)
    // (taking absolute values and ensuring a <= b).
    //
    // We process primes one by one. We maintain for the "current product" a list of
    // all (a,b) representations. Adding a new prime doubles the number of representations.
    //
    // But we need to process ALL subsets. The key: we want sum of S(N) over all N.
    // S(N) = sum of a values over all representations (a,b) of N with a <= b.
    //
    // We can compute the total by DFS over subsets of primes:
    // At each step, we either include or exclude the next prime.
    // If included, combine each existing representation with the prime's representation.
    // Track the sum of a values across all representations.
    //
    // To avoid storing all representations (which grow exponentially), we process
    // the inclusion of each prime and aggregate the sum of a values.
    //
    // Actually, for 16 primes, max representations per subset = 2^15 = 32768.
    // Total representations across all subsets = sum over k=1..16 of C(16,k) * 2^(k-1)
    // = (1/2) * sum of C(16,k) * 2^k = (1/2) * (3^16 - 1) ~ 21.5 million.
    // That's manageable!

    lll total = 0;

    // DFS: idx = current prime index, reps = list of current representations
    function<void(int, vector<gi>&)> dfs = [&](int idx, vector<gi>& reps) {
        // Try including each remaining prime
        for (int i = idx; i < np; i++) {
            gi pg = prime_gi[i];
            vector<gi> new_reps;

            if (reps.empty()) {
                // First prime: just one representation
                new_reps.push_back(pg);
            } else {
                // Combine each existing rep with the new prime
                for (auto& r : reps) {
                    // Two new representations from Brahmagupta-Fibonacci
                    ll a1 = abs(r.first * pg.first - r.second * pg.second);
                    ll b1 = abs(r.first * pg.second + r.second * pg.first);
                    ll a2 = abs(r.first * pg.first + r.second * pg.second);
                    ll b2 = abs(r.first * pg.second - r.second * pg.first);
                    // Ensure a <= b
                    if (a1 > b1) swap(a1, b1);
                    if (a2 > b2) swap(a2, b2);
                    new_reps.push_back({a1, b1});
                    new_reps.push_back({a2, b2});
                }
            }

            // Compute S(N) for this subset: sum of a values
            for (auto& r : new_reps) {
                total += r.first;
            }

            // Continue with more primes
            dfs(i + 1, new_reps);
        }
    };

    vector<gi> empty;
    dfs(0, empty);

    print128(total);
    return 0;
}
