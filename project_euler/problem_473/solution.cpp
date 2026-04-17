#include <bits/stdc++.h>
using namespace std;

// Use __float128 for sufficient precision (or long double as fallback)
#ifdef __SIZEOF_FLOAT128__
typedef __float128 Real;
#else
typedef long double Real;
#endif

static const long long LIMIT = 10000000000LL; // 10^10
static const int MAX_EXP = 50;

Real phi_pow[MAX_EXP + 1];  // phi^i
Real phi_neg[MAX_EXP + 1];  // phi^{-(i+1)}

long long total_sum = 0;
set<long long> found;

// phi = (1 + sqrt(5)) / 2
Real phi_val;

void init() {
    // Compute phi with high precision
    Real five = 5.0;
    Real sqrt5;
#ifdef __SIZEOF_FLOAT128__
    sqrt5 = sqrtq(five);
    phi_val = (1.0Q + sqrt5) / 2.0Q;
#else
    sqrt5 = sqrtl(five);
    phi_val = (1.0L + sqrt5) / 2.0L;
#endif

    phi_pow[0] = 1.0;
    for (int i = 1; i <= MAX_EXP; i++) {
        phi_pow[i] = phi_pow[i - 1] * phi_val;
    }
    phi_neg[0] = 1.0;
    Real inv_phi = 1.0 / phi_val;
    for (int i = 1; i <= MAX_EXP; i++) {
        phi_neg[i] = phi_neg[i - 1] * inv_phi;
    }
}

void dfs(int pos, Real val, bool prev_on) {
    // Prune: if value already exceeds limit
    if ((long long)(val + 0.5) > LIMIT + 1000) return;

    if (pos < 0) {
        long long rounded = (long long)(val + 0.5);
        if (rounded > 0 && rounded <= LIMIT) {
            Real diff = val - (Real)rounded;
            if (diff < 0) diff = -diff;
#ifdef __SIZEOF_FLOAT128__
            if (diff < 1e-15Q) {
#else
            if (diff < 1e-12L) {
#endif
                found.insert(rounded);
            }
        }
        return;
    }

    // Don't activate position pos
    dfs(pos - 1, val, false);

    // Activate position pos (if non-consecutive)
    if (!prev_on) {
        Real contrib;
        if (pos == 0) {
            contrib = 1.0; // phi^0 = 1
        } else {
            contrib = phi_pow[pos] + phi_neg[pos + 1];
        }
        dfs(pos - 1, val + contrib, true);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    init();

    // Search all possible half-widths
    for (int k = 0; k < MAX_EXP; k++) {
        if ((long long)(phi_pow[k] + 0.5) > LIMIT * 2) break;
        dfs(k, 0.0, false);
    }

    long long answer = 0;
    for (long long v : found) {
        answer += v;
    }

    cout << answer << endl;
    return 0;
}
