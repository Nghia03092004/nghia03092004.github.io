#include <bits/stdc++.h>
using namespace std;

// Problem 313: Sliding Game
//
// S(m,n) = minimum moves to slide red counter from top-left to bottom-right
// on an m x n grid with empty space starting at bottom-right.
//
// Count how many grids (m,n) have S(m,n) = p^2 for some prime p < 10^6.
//
// Formula: For each prime p, the number of grids with S(m,n) = p^2 is:
//   - p = 2: (4-1)/12 = not integer, need to check... actually the formula is cumulative
//   - The contribution of each prime p to the total count:
//     p = 3: add 2
//     p != 3: add (p^2 - 1) / 12
//
// Total = sum over all primes p < 10^6 of contribution(p)

int main() {
    const int LIMIT = 1000000; // 10^6

    // Sieve of Eratosthenes
    vector<bool> is_prime(LIMIT, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; (long long)i * i < LIMIT; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j < LIMIT; j += i) {
                is_prime[j] = false;
            }
        }
    }

    long long answer = 0;

    for (int p = 2; p < LIMIT; p++) {
        if (!is_prime[p]) continue;

        if (p == 2) {
            // For p=2: p^2=4, need to count grids with S(m,n)=4
            // (2^2-1)/12 is not integer. Let's check: the formula says
            // for p=2, the number of grids is (4-1)/12 which isn't integer.
            // Actually from the reference, the sum formula is:
            // total = sum of (p^2-1)/12 for p>=5 prime, plus 2 for p=3, plus something for p=2
            // Let me check: 5482 grids for primes < 100
            // Primes < 100: 2,3,5,7,11,13,...,97
            // For p=2: (4-1)/12 = 0.25 -> not integer
            // The reference says "p=3: add 2, otherwise: add (p^2-1)/12"
            // For p=2: (4-1)/12 = 0.25, that's wrong.
            // Maybe p=2 also has a special count. Let me compute:
            // For p>=5 (odd primes >=5): p^2-1 = (p-1)(p+1), both even, so divisible by 4,
            // and one of p-1,p+1 divisible by 3, so (p^2-1)/12 is integer.
            // For p=2: S(m,n)=4. From BFS: S(2,2)=5, so no grid has S=4.
            // So p=2 contributes 0.
            answer += 0;
        } else if (p == 3) {
            answer += 2;
        } else {
            answer += ((long long)p * p - 1) / 12;
        }
    }

    cout << answer << endl;

    return 0;
}
