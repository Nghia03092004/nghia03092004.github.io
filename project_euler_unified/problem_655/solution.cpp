#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 655: Divisible Palindromes
 *
 * Count palindromes < 10^32 divisible by 10000019.
 *
 * A d-digit palindrome (d=1..32) is determined by h = ceil(d/2) free digits.
 * Max h = 16 for d=31 or d=32.
 *
 * For meet-in-the-middle: split h free digits into two halves of ~8 each.
 * 10^8 = 100M per half -- manageable.
 *
 * Weight of free digit position i (0-indexed):
 *   w[i] = 10^(d-1-i) + 10^i  (mod M)   if position i is paired
 *   w[i] = 10^(d-1-i)         (mod M)   if position i is middle (d odd, i = h-1)
 *
 * Leading digit (position 0) in [1,9], others in [0,9].
 */

typedef long long ll;
const ll M = 10000019;

ll power(ll base, ll exp, ll mod) {
    ll result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

ll count_palindromes(int d) {
    if (d == 1) return 0; // single digits < M

    int h = (d + 1) / 2;
    bool odd = (d % 2 == 1);

    vector<ll> w(h);
    for (int i = 0; i < h; i++) {
        ll pw_left = power(10, d - 1 - i, M);
        ll pw_right = power(10, i, M);
        if (odd && i == h - 1)
            w[i] = pw_left;
        else
            w[i] = (pw_left + pw_right) % M;
    }

    // Split: first half = positions 0..mid-1, second half = positions mid..h-1
    int mid = h / 2;
    if (mid < 1) mid = 1; // ensure first half has at least the leading digit
    int h1 = mid;
    int h2 = h - mid;

    // Build hash map for second half (all digits 0-9)
    // h2 <= 8 for our cases, so 10^8 = 100M entries max
    // Use vector indexed by remainder for speed
    vector<int> rhs_count(M, 0);

    {
        ll total = 1;
        for (int i = 0; i < h2; i++) total *= 10;

        // Recursive enumeration to avoid integer decomposition overhead
        function<void(int, ll)> enumerate_rhs = [&](int pos, ll rem) {
            if (pos == h2) {
                rhs_count[rem]++;
                return;
            }
            for (int digit = 0; digit <= 9; digit++) {
                ll nrem = (rem + digit * w[mid + pos]) % M;
                enumerate_rhs(pos + 1, nrem);
            }
        };
        enumerate_rhs(0, 0);
    }

    // Enumerate first half: position 0 in [1,9], positions 1..h1-1 in [0,9]
    ll count = 0;
    {
        function<void(int, ll)> enumerate_lhs = [&](int pos, ll rem) {
            if (pos == h1) {
                ll need = (M - rem) % M;
                count += rhs_count[need];
                return;
            }
            int start = (pos == 0) ? 1 : 0;
            for (int digit = start; digit <= 9; digit++) {
                ll nrem = (rem + digit * w[pos]) % M;
                enumerate_lhs(pos + 1, nrem);
            }
        };
        enumerate_lhs(0, 0);
    }

    return count;
}

int main() {
    ll total = 0;
    for (int d = 1; d <= 32; d++) {
        ll c = count_palindromes(d);
        total += c;
    }
    cout << total << endl;
    return 0;
}
