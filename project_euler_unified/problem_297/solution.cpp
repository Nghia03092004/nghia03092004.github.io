#include <bits/stdc++.h>
using namespace std;

// Problem 297: Zeckendorf Representation
//
// Find sum of z(n) for 0 < n < 10^17, where z(n) = number of terms
// in the Zeckendorf representation of n.
//
// Fibonacci: F[1]=1, F[2]=2, F[3]=3, F[4]=5, ...
//
// S(N) = sum of z(n) for n in [1, N).
//
// At Fibonacci boundaries:
//   S(F[k+1]) = S(F[k]) + S(F[k-1]) + F[k-1]
//
// General recursion (for F[k] <= N < F[k+1]):
//   S(N) = S(F[k]) + (N - F[k]) + S(N - F[k])
//
// Each recursive call reduces N, and the sequence of remainders follows
// a Zeckendorf-like decomposition, terminating in O(log N) steps.

int main(){
    long long N = 100000000000000000LL; // 10^17

    // Generate Fibonacci numbers: F[1]=1, F[2]=2, F[3]=3, ...
    vector<long long> F;
    F.push_back(1);
    F.push_back(2);
    while (F.back() < N) {
        F.push_back(F[F.size()-1] + F[F.size()-2]);
    }
    int M = F.size(); // F[0]..F[M-1] = F_1..F_M

    // Precompute S(F[k]) for k = 1..M
    // S(F[1]) = S(1) = 0 (sum over empty range [1,1))
    // S(F[2]) = S(2) = z(1) = 1
    // S(F[k+1]) = S(F[k]) + S(F[k-1]) + F[k-1]
    // Using 0-indexed: S_fib[i] = S(F[i])
    vector<long long> SF(M + 1, 0);
    // SF[0] = S(F[0]) = S(1) = 0
    // SF[1] = S(F[1]) = S(2) = z(1) = 1
    SF[0] = 0;
    if (M >= 2) SF[1] = 1;
    for (int i = 2; i < M; i++) {
        // S(F[i]) = S(F[i-1]) + S(F[i-2]) + F[i-2]
        // In 0-indexed: F[i] corresponds to F_{i+1}, F[i-2] = F_{i-1}
        // Wait, let me re-index carefully.
        // F[0] = F_1 = 1, F[1] = F_2 = 2, F[2] = F_3 = 3, ...
        // S(F_{k+1}) = S(F_k) + S(F_{k-1}) + F_{k-1}
        // In 0-indexed: S(F[i]) = S(F[i-1]) + S(F[i-2]) + F[i-2]
        SF[i] = SF[i-1] + SF[i-2] + F[i-2];
    }

    // Now compute S(N) recursively
    // S(N): find largest F[k] <= N, then S(N) = SF[k] + (N - F[k]) + S(N - F[k])
    // where k is 0-indexed into F array.
    // Note: N - F[k] < F[k-1], so recursion terminates.

    function<long long(long long)> S = [&](long long n) -> long long {
        if (n <= 1) return 0; // S(1) = 0, S(0) = 0
        // Find largest F[k] <= n
        // F is sorted, use upper_bound
        int k = (int)(upper_bound(F.begin(), F.end(), n) - F.begin()) - 1;
        // F[k] <= n < F[k+1]
        if (F[k] == n) return SF[k];
        return SF[k] + (n - F[k]) + S(n - F[k]);
    };

    cout << S(N) << endl;
    return 0;
}
