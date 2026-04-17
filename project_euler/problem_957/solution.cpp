/*
 * Problem 957: Diophantine Approximation
 *
 * Convergents of sqrt(2) = [1; 2, 2, ...] satisfy p_k^2 - 2*q_k^2 = (-1)^{k+1}.
 * Sum for k = 0..50 = -1 (closed form).
 *
 * Verification: compute convergents using arbitrary-precision integers
 * (or just use the closed-form answer).
 *
 * Complexity: O(1) closed form, O(N) with verification.
 */

#include <bits/stdc++.h>
using namespace std;

int main() {
    // Closed-form: sum_{k=0}^{50} (-1)^{k+1}
    // 26 even k's contribute -1, 25 odd k's contribute +1
    // Total = -26 + 25 = -1

    // Verification with __int128 (convergents grow as (1+sqrt(2))^k)
    __int128 p_prev = 0, p_curr = 1;
    __int128 q_prev = 1, q_curr = 1;
    long long total = 0;

    // k = 0
    __int128 val = p_curr * p_curr - 2 * q_curr * q_curr;
    total += (long long)val;

    for (int k = 1; k <= 50; k++) {
        __int128 p_next = 2 * p_curr + p_prev;
        __int128 q_next = 2 * q_curr + q_prev;
        p_prev = p_curr; p_curr = p_next;
        q_prev = q_curr; q_curr = q_next;

        val = p_curr * p_curr - 2 * q_curr * q_curr;
        total += (long long)val;
    }

    cout << total << endl;
    return 0;
}
