#include <bits/stdc++.h>
using namespace std;

// Problem 176: Right-angled Triangles That Share a Cathetus
// Find smallest a with exactly 47547 right triangles having leg a.
//
// For odd a = prod p_i^{e_i}: f(a) = (prod(2e_i+1) - 1) / 2
// For even a = 2^{e0} * prod p_i^{e_i}: f(a) = ((2e0-1)*prod(2e_i+1) - 1) / 2
//
// We need f(a) = 47547, so the product = 95095 = 5*7*11*13*19.

typedef __int128 lll;

int target = 95095;
vector<int> divisors;
vector<vector<int>> partitions;

void get_divisors(int n) {
    for (int i = 2; (long long)i * i <= n; i++) {
        if (n % i == 0) {
            divisors.push_back(i);
            if (i != n / i) divisors.push_back(n / i);
        }
    }
    divisors.push_back(n);
    sort(divisors.begin(), divisors.end());
}

void gen_partitions(int n, int min_f, vector<int>& cur) {
    if (n == 1) {
        partitions.push_back(cur);
        return;
    }
    for (int d : divisors) {
        if (d < min_f) continue;
        if (d > n) break;
        if (n % d != 0) continue;
        cur.push_back(d);
        gen_partitions(n / d, d, cur);
        cur.pop_back();
    }
}

int small_primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};

// Compute value using logarithms for comparison, actual value for result
double log_value(int e0, vector<int>& exps) {
    double v = e0 * log(2.0);
    sort(exps.rbegin(), exps.rend());
    for (int i = 0; i < (int)exps.size(); i++)
        v += exps[i] * log((double)small_primes[i + 1]);
    return v;
}

lll actual_value(int e0, vector<int>& exps) {
    sort(exps.rbegin(), exps.rend());
    lll v = 1;
    for (int j = 0; j < e0; j++) v *= 2;
    for (int i = 0; i < (int)exps.size(); i++)
        for (int j = 0; j < exps[i]; j++)
            v *= small_primes[i + 1];
    return v;
}

void print128(lll x) {
    if (x == 0) { printf("0\n"); return; }
    string s;
    while (x > 0) { s += '0' + (int)(x % 10); x /= 10; }
    reverse(s.begin(), s.end());
    printf("%s\n", s.c_str());
}

int main() {
    get_divisors(target);
    vector<int> cur;
    gen_partitions(target, 2, cur);

    double best_log = 1e18;
    lll best_val = -1;

    for (auto& p : partitions) {
        // Case 1: odd a
        {
            vector<int> exps;
            for (int f : p) exps.push_back((f - 1) / 2);
            double lv = 0;
            sort(exps.rbegin(), exps.rend());
            for (int i = 0; i < (int)exps.size(); i++)
                lv += exps[i] * log((double)small_primes[i + 1]);
            if (lv < best_log) {
                best_log = lv;
                best_val = actual_value(0, exps);
            }
        }

        // Case 2: even a
        set<int> seen;
        for (int idx = 0; idx < (int)p.size(); idx++) {
            if (!seen.insert(p[idx]).second) continue;
            int e0 = (p[idx] + 1) / 2;
            vector<int> exps;
            for (int j = 0; j < (int)p.size(); j++) {
                if (j == idx) continue;
                int e = (p[j] - 1) / 2;
                if (e > 0) exps.push_back(e);
            }
            sort(exps.rbegin(), exps.rend());
            double lv = e0 * log(2.0);
            for (int i = 0; i < (int)exps.size(); i++)
                lv += exps[i] * log((double)small_primes[i + 1]);
            if (lv < best_log) {
                best_log = lv;
                best_val = actual_value(e0, exps);
            }
        }
    }

    print128(best_val);
    return 0;
}
