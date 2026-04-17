/*
 * Problem 963: Palindromic Primes in Bases
 *
 * Count primes < 10^6 that are palindromes in >= 2 bases from {2,...,16}.
 *
 * Complexity: O(N log log N + pi(N) * 15 * log N).
 */

#include <bits/stdc++.h>
using namespace std;

bool is_palindrome_base(int n, int b) {
    vector<int> digits;
    int tmp = n;
    while (tmp > 0) {
        digits.push_back(tmp % b);
        tmp /= b;
    }
    int sz = digits.size();
    for (int i = 0; i < sz / 2; i++) {
        if (digits[i] != digits[sz - 1 - i]) return false;
    }
    return true;
}

int main() {
    const int N = 1000000;
    vector<bool> is_prime(N, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; (long long)i*i < N; i++)
        if (is_prime[i])
            for (int j = i*i; j < N; j += i)
                is_prime[j] = false;

    int count = 0;
    for (int p = 2; p < N; p++) {
        if (!is_prime[p]) continue;
        int pal_count = 0;
        for (int b = 2; b <= 16; b++) {
            if (is_palindrome_base(p, b)) pal_count++;
            if (pal_count >= 2) break;
        }
        if (pal_count >= 2) count++;
    }

    cout << count << endl;
    return 0;
}
