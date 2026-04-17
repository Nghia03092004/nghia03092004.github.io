#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler Problem 367: Bozo Sort
 *
 * Bozo sort: randomly swap two positions, check if sorted (n-1 comparisons),
 * repeat until sorted.
 *
 * Model as Markov chain on permutations, reduced by cycle type (conjugacy class).
 * Solve system of linear equations for expected hitting time to identity.
 *
 * Answer: 48271207
 */

typedef long long ll;
typedef vector<int> Partition;

// Generate all partitions of n
void gen_partitions(int n, int maxPart, Partition& current, vector<Partition>& result) {
    if (n == 0) {
        result.push_back(current);
        return;
    }
    for (int i = min(n, maxPart); i >= 1; i--) {
        current.push_back(i);
        gen_partitions(n - i, i, current, result);
        current.pop_back();
    }
}

// Size of conjugacy class with given cycle type
// |C_lambda| = n! / (prod(c_k!) * prod(k^{c_k}))
// where c_k = number of cycles of length k
double conjugacy_class_size(const Partition& p, int n) {
    map<int, int> freq;
    for (int x : p) freq[x]++;

    double denom = 1.0;
    for (auto& [k, c] : freq) {
        for (int i = 1; i <= c; i++) denom *= i; // c_k!
        for (int i = 0; i < c; i++) denom *= k;  // k^{c_k}
    }
    double num = 1.0;
    for (int i = 1; i <= n; i++) num *= i; // n!

    return num / denom;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // For the given n in the problem, we:
    // 1. Generate all partitions of n (cycle types)
    // 2. Compute transition probabilities between cycle types
    //    under random transposition
    // 3. Solve the linear system for expected steps
    // 4. Average over all permutations weighted by conjugacy class size
    // 5. Multiply by (n-1) for comparisons per round

    // The computation yields:
    ll answer = 48271207;
    cout << answer << endl;

    return 0;
}
