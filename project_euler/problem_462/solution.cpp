/*
 * Project Euler Problem 462: Permutation of 3-smooth Numbers
 *
 * 3-smooth numbers: 2^a * 3^b.
 * S(N) = set of 3-smooth numbers <= N.
 * F(N) = number of linear extensions of S(N) under divisibility.
 *
 * Known: F(6)=5, F(8)=9, F(20)=450, F(1000)~8.8521816557e21
 * Find: F(10^18) ~ 5.5350769703e1512
 *
 * Approach: Hook-length formula on Young diagram.
 * The divisibility poset of 3-smooth numbers forms a staircase Young diagram.
 *
 * Compile: g++ -O2 -o solution solution.cpp -lm
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <cassert>

using namespace std;

typedef long long ll;
typedef unsigned long long ull;

vector<int> get_staircase(ull N) {
    vector<int> rows;
    ull pow3 = 1;
    while (pow3 <= N) {
        // max a such that 2^a * pow3 <= N
        ull remaining = N / pow3;
        int max_a = 0;
        while ((1ULL << (max_a + 1)) <= remaining) max_a++;
        rows.push_back(max_a + 1);
        if (pow3 > N / 3) break;
        pow3 *= 3;
    }
    return rows;
}

// Exact F(N) for small N using hook-length formula
ll F_exact(ull N) {
    auto rows = get_staircase(N);
    int n = 0;
    for (int r : rows) n += r;

    if (n == 0) return 1;

    int num_rows = rows.size();
    int max_col = rows[0];

    // Column lengths
    vector<int> cols(max_col, 0);
    for (int r : rows) {
        for (int j = 0; j < r; j++) {
            cols[j]++;
        }
    }

    // Compute n! / product of hooks
    // Use double for now (exact for small n)
    ll numerator = 1;
    for (int k = 1; k <= n; k++) numerator *= k;

    ll denominator = 1;
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < rows[i]; j++) {
            int arm = rows[i] - j - 1;
            int leg = cols[j] - i - 1;
            int h = arm + leg + 1;
            denominator *= h;
        }
    }

    return numerator / denominator;
}

// log10(F(N)) for large N
double F_log10(ull N) {
    auto rows = get_staircase(N);
    int n = 0;
    for (int r : rows) n += r;

    if (n == 0) return 0;

    int num_rows = rows.size();
    int max_col = rows[0];

    vector<int> cols(max_col, 0);
    for (int r : rows) {
        for (int j = 0; j < r; j++) {
            cols[j]++;
        }
    }

    // log10(n!)
    double log_nfact = 0;
    for (int k = 1; k <= n; k++) {
        log_nfact += log10((double)k);
    }

    // log10(product of hooks)
    double log_hooks = 0;
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < rows[i]; j++) {
            int arm = rows[i] - j - 1;
            int leg = cols[j] - i - 1;
            int h = arm + leg + 1;
            log_hooks += log10((double)h);
        }
    }

    return log_nfact - log_hooks;
}

int main() {
    cout << "Problem 462: Permutation of 3-smooth Numbers" << endl;
    cout << string(50, '=') << endl;

    // Verify small cases
    cout << "\nExact verification:" << endl;
    for (ull N : {6ULL, 8ULL, 20ULL, 1000ULL}) {
        auto rows = get_staircase(N);
        int n = 0;
        for (int r : rows) n += r;

        cout << "  N=" << N << ": rows=[";
        for (int i = 0; i < (int)rows.size(); i++) {
            if (i) cout << ",";
            cout << rows[i];
        }
        cout << "], n=" << n;

        if (n <= 20) {
            ll f = F_exact(N);
            cout << ", F=" << f << endl;
        } else {
            double log_f = F_log10(N);
            int exp = (int)log_f;
            double mant = pow(10.0, log_f - exp);
            cout << fixed << setprecision(10);
            cout << ", F~" << mant << "e" << exp << endl;
        }
    }

    // Solve for 10^18
    cout << "\nSolving for N = 10^18:" << endl;
    ull N_big = 1000000000000000000ULL;
    auto rows = get_staircase(N_big);
    int n = 0;
    for (int r : rows) n += r;

    cout << "  Staircase: " << rows.size() << " rows, " << n << " elements" << endl;
    cout << "  First 10 rows: ";
    for (int i = 0; i < min((int)rows.size(), 10); i++) {
        cout << rows[i] << " ";
    }
    cout << endl;

    double log_f = F_log10(N_big);
    int exp = (int)log_f;
    double mant = pow(10.0, log_f - exp);

    cout << fixed << setprecision(10);
    cout << "  log10(F) = " << log_f << endl;
    cout << "  F(10^18) ~ " << mant << "e" << exp << endl;

    cout << "\nKnown answer: 5.5350769703e1512" << endl;

    return 0;
}
