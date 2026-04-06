#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 lll;

/*
 * Problem 272: Modular Cubes, Part 2
 *
 * Find the sum of all n <= 10^11 with C(n) = 242 (cube roots of unity minus 1).
 * c(n) = 3^k, need k = 5.
 *
 * Case A: 5 type-A primes (p=1 mod 3), 9 does not divide n.
 * Case B: 4 type-A primes, 9 divides n.
 *
 * Two-phase approach:
 * Phase 1: Enumerate active sets (subsets of type-A primes).
 * Phase 2: For each active set, compute sum of valid n using I-E on forbidden primes.
 */

const ll N = 100000000000LL;

vector<int> typeA;

void sieve(int lim) {
    vector<bool> is_p(lim + 1, true);
    is_p[0] = is_p[1] = false;
    for (int i = 2; i <= lim; i++) {
        if (is_p[i]) {
            if (i >= 7 && i % 3 == 1) typeA.push_back(i);
            for (ll j = (ll)i * i; j <= lim; j += i)
                is_p[j] = false;
        }
    }
}

lll total_sum = 0;

inline lll sum_multiples(ll L, ll d) {
    if (d > L || L <= 0) return 0;
    lll f = L / d;
    return (lll)d * f * (f + 1) / 2;
}

// I-E DFS on forbidden primes
// forbidden: sorted list of type-A primes not in active set, up to L
// Computes sum of t in [1, L] with no forbidden factor (and optional 9-constraint)
void ie_dfs(const vector<int>& forbidden, int idx, ll L, ll prod, int sign,
            ll active_prod, bool caseA, lll& result) {
    // Contribution at this node
    lll contrib;
    if (caseA) {
        contrib = sum_multiples(L, prod);
        if ((lll)prod * 9 <= L) {
            contrib -= sum_multiples(L, prod * 9);
        }
    } else {
        contrib = sum_multiples(L, prod);
    }
    result += (lll)sign * (lll)active_prod * contrib;

    // Recurse
    for (int i = idx; i < (int)forbidden.size(); i++) {
        ll p = forbidden[i];
        if (prod > L / p) break;
        ie_dfs(forbidden, i + 1, L, prod * p, -sign, active_prod, caseA, result);
    }
}

// Enumerate active sets
void active_dfs(int idx, int cnt, int target, ll prod, bool caseA) {
    if (cnt == target) {
        ll L = N / prod;
        if (L < 1) return;

        // Build forbidden list
        vector<int> forbidden;
        // We need all type-A primes up to L that are NOT in the active set.
        // Problem: we don't have the active set stored, just the product.
        // We need to track which primes are active.
        // Let's pass the active set.
        // ... Actually, for the I-E, we just need forbidden primes.
        // Let me restructure to pass active set.
        return;
    }

    for (int i = idx; i < (int)typeA.size(); i++) {
        ll p = typeA[i];
        if (prod > N / p) break;
        active_dfs(i + 1, cnt + 1, target, prod * p, caseA);
    }
}

// Better: pass active set explicitly
void active_dfs2(int idx, int cnt, int target, ll prod, bool caseA,
                  vector<int>& active_set) {
    if (cnt == target) {
        ll L = N / prod;
        if (L < 1) return;

        // Build forbidden: type-A primes not in active_set, up to L
        int ai = 0;
        vector<int> forbidden;
        for (int i = 0; i < (int)typeA.size(); i++) {
            int p = typeA[i];
            if (p > L) break;
            if (ai < (int)active_set.size() && active_set[ai] == p) {
                ai++;
                continue;
            }
            forbidden.push_back(p);
        }

        lll result = 0;
        ie_dfs(forbidden, 0, L, 1, 1, prod, caseA, result);
        total_sum += result;
        return;
    }

    for (int i = idx; i < (int)typeA.size(); i++) {
        ll p = typeA[i];
        if (prod > N / p) break;
        active_set.push_back(typeA[i]);
        active_dfs2(i + 1, cnt + 1, target, prod * p, caseA, active_set);
        active_set.pop_back();
    }
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
    sieve(50000000);

    vector<int> as;

    // Case A: 5 type-A primes, 9 does not divide n
    active_dfs2(0, 0, 5, 1, true, as);

    // Case B: 4 type-A primes, 9 divides n
    active_dfs2(0, 0, 4, 9, false, as);

    print128(total_sum);
    return 0;
}
