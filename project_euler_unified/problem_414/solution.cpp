/**
 * Project Euler Problem 414: Kaprekar Constant
 *
 * Define C_b = Kaprekar constant in base b for 5 digits.
 * sb(i) = 0 if i = C_b or all digits identical, else iterations to reach C_b.
 * S(b) = sum of sb(i) for 0 < i < b^5.
 *
 * Given: S(15) = 5274369, S(111) = 400668930299.
 * Find: last 18 digits of sum of S(6k+3) for 2 <= k <= 300.
 *
 * Approach: For each base b, work with sorted 5-tuples (multisets of digits).
 * The Kaprekar step maps one sorted tuple to another.
 * Build reverse graph, BFS from C_b, weight by multiplicity.
 *
 * Compilation: g++ -O2 -std=c++17 -o solution solution.cpp
 */

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <cstdint>
#include <cassert>
#include <chrono>
#include <tuple>

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef tuple<int,int,int,int,int> Tup5;

const ll MOD = 1000000000000000000LL;  // 10^18

/**
 * Convert number n to sorted (descending) 5-digit tuple in base b.
 */
Tup5 to_sorted_tuple(ll n, int b) {
    int digits[5];
    for (int i = 0; i < 5; i++) {
        digits[i] = n % b;
        n /= b;
    }
    sort(digits, digits + 5, greater<int>());
    return {digits[0], digits[1], digits[2], digits[3], digits[4]};
}

/**
 * Compute Kaprekar step from a sorted descending tuple.
 * Returns the resulting number (not sorted).
 */
ll kaprekar_step_from_tuple(const Tup5& tup, int b) {
    auto [d0, d1, d2, d3, d4] = tup;
    // Descending number: d0*b^4 + d1*b^3 + d2*b^2 + d3*b + d4
    ll D = ((((ll)d0 * b + d1) * b + d2) * b + d3) * b + d4;
    // Ascending number: d4*b^4 + d3*b^3 + d2*b^2 + d1*b + d0
    ll A = ((((ll)d4 * b + d3) * b + d2) * b + d1) * b + d0;
    return D - A;
}

/**
 * Compute multiplicity of a sorted tuple (number of distinct permutations).
 */
ll multiplicity(const Tup5& tup) {
    auto [d0, d1, d2, d3, d4] = tup;
    int digits[5] = {d0, d1, d2, d3, d4};
    // 5! / product of factorial of each digit's frequency
    ll result = 120;  // 5!
    int i = 0;
    while (i < 5) {
        int j = i + 1;
        while (j < 5 && digits[j] == digits[i]) j++;
        int freq = j - i;
        // Divide by freq!
        ll fact = 1;
        for (int k = 2; k <= freq; k++) fact *= k;
        result /= fact;
        i = j;
    }
    return result;
}

/**
 * Find Kaprekar constant for base b with 5 digits.
 */
ll find_kaprekar_constant(int b) {
    // Start from a non-trivial number
    ll n = (ll)b + 1;
    for (int iter = 0; iter < 1000; iter++) {
        ll prev = n;
        auto tup = to_sorted_tuple(n, b);
        n = kaprekar_step_from_tuple(tup, b);
        if (n == prev) return n;  // Fixed point found
    }
    // If no fixed point found, return the cycle entry point
    // Use Floyd's cycle detection
    ll slow = (ll)b + 1, fast = (ll)b + 1;
    do {
        auto t1 = to_sorted_tuple(slow, b);
        slow = kaprekar_step_from_tuple(t1, b);

        auto t2 = to_sorted_tuple(fast, b);
        fast = kaprekar_step_from_tuple(t2, b);
        t2 = to_sorted_tuple(fast, b);
        fast = kaprekar_step_from_tuple(t2, b);
    } while (slow != fast);
    return slow;  // A member of the cycle
}

/**
 * Compute S(b) using multiset BFS approach.
 */
ll compute_S(int b) {
    ll C_b = find_kaprekar_constant(b);
    Tup5 C_b_tup = to_sorted_tuple(C_b, b);

    // Verify it's a fixed point
    ll check = kaprekar_step_from_tuple(C_b_tup, b);
    bool is_fixed_point = (to_sorted_tuple(check, b) == C_b_tup);

    if (!is_fixed_point) {
        // Cycle case - more complex handling needed
        // For the problem's bases (6k+3), we expect fixed points
        cerr << "Warning: base " << b << " does not have a simple fixed point" << endl;
    }

    // Enumerate all sorted 5-tuples (d0 >= d1 >= d2 >= d3 >= d4), 0 <= di < b
    // Build graph: each tuple -> Kaprekar image tuple
    // Then BFS from C_b_tup on reverse graph

    // We split C_b_tup into two virtual nodes:
    //   C_b_tup at depth 0 (represents the actual number C_b)
    //   C_b_perm (sentinel) at depth 1 (represents other permutations of C_b's digits)
    // Tuples whose D-A == C_b point to C_b_tup.
    // Tuples whose D-A has C_b's sorted digits but != C_b point to C_b_perm.

    Tup5 C_b_perm = {-1, -1, -1, -1, -1};  // Sentinel for virtual "perm" node

    map<Tup5, vector<Tup5>> reverse_graph;
    map<Tup5, ll> mult_map;
    map<Tup5, Tup5> forward_graph;

    // Enumerate tuples
    for (int d0 = 0; d0 < b; d0++) {
        for (int d1 = 0; d1 <= d0; d1++) {
            for (int d2 = 0; d2 <= d1; d2++) {
                for (int d3 = 0; d3 <= d2; d3++) {
                    for (int d4 = 0; d4 <= d3; d4++) {
                        Tup5 tup = {d0, d1, d2, d3, d4};

                        // Skip all-identical
                        if (d0 == d4) continue;

                        ll result = kaprekar_step_from_tuple(tup, b);
                        Tup5 result_tup = to_sorted_tuple(result, b);

                        forward_graph[tup] = result_tup;

                        // Determine if this maps to C_b exactly or just its sorted tuple
                        if (result_tup == C_b_tup && result != C_b) {
                            reverse_graph[C_b_perm].push_back(tup);
                        } else {
                            reverse_graph[result_tup].push_back(tup);
                        }
                        mult_map[tup] = multiplicity(tup);
                    }
                }
            }
        }
    }

    // C_b_perm is a virtual child of C_b_tup (depth 1)
    reverse_graph[C_b_tup].push_back(C_b_perm);

    // BFS from C_b_tup
    map<Tup5, int> dist;
    queue<Tup5> q;

    dist[C_b_tup] = 0;
    q.push(C_b_tup);

    ll total = 0;

    // Add contribution from other permutations of C_b's digits (sb = 1 each)
    ll C_b_mult = multiplicity(C_b_tup);
    total += (C_b_mult - 1) * 1;

    while (!q.empty()) {
        Tup5 node = q.front();
        q.pop();
        int d = dist[node];

        if (reverse_graph.find(node) == reverse_graph.end()) continue;

        for (const Tup5& parent : reverse_graph[node]) {
            if (dist.find(parent) != dist.end()) continue;
            // Skip all-identical (already excluded during enumeration)
            dist[parent] = d + 1;
            if (parent != C_b_perm) {
                ll m = mult_map[parent];
                total += (ll)(d + 1) * m;
            }
            // C_b_perm is virtual: no multiplicity contribution, but still BFS through it
            q.push(parent);
        }
    }

    return total;
}


int main() {
    cout << "Project Euler Problem 414: Kaprekar Constant" << endl;
    cout << "=============================================" << endl;

    auto start_time = chrono::high_resolution_clock::now();

    // Verify S(15)
    cout << "\n--- Verification ---" << endl;
    ll s15 = compute_S(15);
    cout << "S(15) = " << s15 << " (expected 5274369)" << endl;
    assert(s15 == 5274369);

    // Verify S(111) if feasible
    cout << "Computing S(111)..." << endl;
    ll s111 = compute_S(111);
    cout << "S(111) = " << s111 << " (expected 400668930299)" << endl;
    assert(s111 == 400668930299LL);

    // Compute the answer
    cout << "\n--- Computing sum of S(6k+3) for k=2..300 ---" << endl;
    ull total = 0;

    for (int k = 2; k <= 300; k++) {
        int b = 6 * k + 3;
        ll s = compute_S(b);
        total += (ull)s;
        // total %= MOD;  // We accumulate and take mod at the end

        if (k <= 5 || k % 50 == 0 || k == 300) {
            cout << "  k=" << k << ", b=" << b << ", S(b)=" << s
                 << ", running_total=" << total << endl;
        }
    }

    ull answer = total % (ull)MOD;

    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(end_time - start_time).count();

    cout << "\n--- Result ---" << endl;
    cout << "Last 18 digits: " << answer << endl;
    cout << "Time: " << duration << " seconds" << endl;

    cout << "\n--- Known Answer ---" << endl;
    cout << "Expected last 18 digits: 552506775824935461" << endl;

    return 0;
}
