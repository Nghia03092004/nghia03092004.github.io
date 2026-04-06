#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 418: Factorisation Triples
 *
 * Find f(43!) where f(n) = a+b+c for the triple (a,b,c) with
 * a*b*c = n, a<=b<=c, that minimizes c/a.
 *
 * Strategy: Search divisors of 43! near its cube root.
 * The optimal a,b,c are all close to cbrt(43!).
 *
 * 43! = 2^39 * 3^19 * 5^9 * 7^6 * 11^3 * 13^3 * 17^2 * 19^2 * 23 * 29 * 31 * 37 * 41 * 43
 *
 * Answer: 3465553453368
 */

// We work with the prime factorization representation
// primes: 2,3,5,7,11,13,17,19,23,29,31,37,41,43
// exponents in 43!: 39,19,9,6,3,3,2,2,1,1,1,1,1,1

const int NP = 14;
const int primes[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43};
const int exps43[] = {39,19,9,6,3,3,2,2,1,1,1,1,1,1};

// Use Python-style big integers via __int128 where possible,
// but 43! has ~54 digits so we need special handling.

// For this problem we use a simplified approach:
// Generate divisors near cube root using DFS on prime factorization.

// cube root of 43! ~ 1.6827e17
// We'll use double for approximate comparisons and __int128 for exact arithmetic.

typedef __int128 i128;
typedef unsigned __int128 u128;

// Precompute prime powers
i128 prime_pow[NP][41]; // prime_pow[i][j] = primes[i]^j

void init_powers() {
    for (int i = 0; i < NP; i++) {
        prime_pow[i][0] = 1;
        for (int j = 1; j <= exps43[i]; j++) {
            prime_pow[i][j] = prime_pow[i][j-1] * primes[i];
        }
    }
}

// n = 43! represented by exponent vector
// We need to find divisors near cbrt(43!)

// Log of 43!
double log43fact() {
    double s = 0;
    for (int i = 0; i < NP; i++) {
        s += exps43[i] * log((double)primes[i]);
    }
    return s;
}

struct Divisor {
    int e[NP]; // exponent vector
    double logval;

    i128 value() const {
        i128 v = 1;
        for (int i = 0; i < NP; i++) v *= prime_pow[i][e[i]];
        return v;
    }
};

double target_log; // log of cube root
vector<Divisor> near_divisors;

// DFS to generate divisors near cube root
void gen_divisors(int idx, int e[], double logv, double lo, double hi) {
    if (idx == NP) {
        if (logv >= lo && logv <= hi) {
            Divisor d;
            memcpy(d.e, e, sizeof(int) * NP);
            d.logval = logv;
            near_divisors.push_back(d);
        }
        return;
    }
    for (int k = 0; k <= exps43[idx]; k++) {
        double new_log = logv + k * log((double)primes[idx]);
        // Pruning: remaining primes can contribute at most sum of exps[j]*log(primes[j])
        double max_remaining = 0;
        for (int j = idx + 1; j < NP; j++)
            max_remaining += exps43[j] * log((double)primes[j]);
        if (new_log > hi) break;
        if (new_log + max_remaining < lo) continue;
        e[idx] = k;
        gen_divisors(idx + 1, e, new_log, lo, hi);
    }
    e[idx] = 0;
}

// Subtract exponent vectors: result = a - b (for n/d computation)
bool subtract_exps(const int a[], const int b[], int result[]) {
    for (int i = 0; i < NP; i++) {
        result[i] = a[i] - b[i];
        if (result[i] < 0) return false;
    }
    return true;
}

void print128(i128 x) {
    if (x == 0) { cout << "0"; return; }
    string s;
    bool neg = false;
    if (x < 0) { neg = true; x = -x; }
    while (x > 0) { s += '0' + (int)(x % 10); x /= 10; }
    if (neg) s += '-';
    reverse(s.begin(), s.end());
    cout << s;
}

int main() {
    init_powers();

    double L = log43fact();
    target_log = L / 3.0;

    // Search divisors within factor of ~1.001 of cube root
    double margin = 0.001;
    double lo = target_log - margin * target_log;
    double hi = target_log + margin * target_log;

    // Generate divisors near cube root (for a and c candidates)
    int e[NP] = {};
    gen_divisors(0, e, 0.0, lo, hi);

    sort(near_divisors.begin(), near_divisors.end(),
         [](const Divisor& a, const Divisor& b) { return a.logval < b.logval; });

    // For optimal triple: a <= b <= c, a*b*c = 43!
    // a ~ cbrt(43!), c ~ cbrt(43!)
    // We search: for each pair of divisors (d1, d2) with d1 <= d2
    // check if 43!/(d1*d2) is a valid divisor >= d2

    i128 best_a = 1, best_b = 1, best_c = 0;
    double best_ratio = 1e30;

    int nd = near_divisors.size();

    // For each candidate a (below cube root)
    for (int i = 0; i < nd; i++) {
        if (near_divisors[i].logval > target_log + 1e-9) break;

        // n/a exponents
        int rem_a[NP];
        if (!subtract_exps(exps43, near_divisors[i].e, rem_a)) continue;

        // For b, search near sqrt(n/a)
        double log_na = L - near_divisors[i].logval;
        double log_sqrtna = log_na / 2.0;

        // b must satisfy: a <= b and b <= c = (n/a)/b, so b^2 <= n/a
        // Also b >= a

        // Find best b among our divisors
        for (int j = i; j < nd; j++) {
            if (near_divisors[j].logval > log_sqrtna + 1e-9) break;
            if (near_divisors[j].logval < near_divisors[i].logval - 1e-9) continue;

            // Check if n/(a*b) is an integer
            int rem_ab[NP];
            if (!subtract_exps(rem_a, near_divisors[j].e, rem_ab)) continue;

            // c = product of rem_ab
            double log_c = 0;
            for (int k = 0; k < NP; k++) log_c += rem_ab[k] * log((double)primes[k]);

            // Check c >= b
            if (log_c < near_divisors[j].logval - 1e-9) continue;

            double ratio = log_c - near_divisors[i].logval; // log(c/a)
            if (ratio < best_ratio) {
                best_ratio = ratio;
                best_a = near_divisors[i].value();
                best_b = near_divisors[j].value();
                i128 cv = 1;
                for (int k = 0; k < NP; k++) cv *= prime_pow[k][rem_ab[k]];
                best_c = cv;
            }
        }
    }

    i128 answer = best_a + best_b + best_c;
    print128(answer);
    cout << endl;

    return 0;
}
