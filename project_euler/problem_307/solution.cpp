#include <bits/stdc++.h>
using namespace std;

int main() {
    // Chip Defects: p(k,n) = probability that at least one chip has 3+ defects
    // k=20000 defects, n=1000000 chips
    // P(no chip has 3+) = sum_{j=0}^{k/2} C(n,j)*C(n-j,k-2j)*k!/(2^j * n^k)
    // p(k,n) = 1 - P(no chip has 3+)
    //
    // Use ratio method in log space:
    // T_{j+1}/T_j = (k-2j)(k-2j-1) / (2(j+1)(n-k+j+1))

    long long n = 1000000;
    long long k = 20000;

    // Compute log(T_0) = sum_{i=0}^{k-1} log((n-i)/n)
    long double log_T0 = 0.0L;
    for (long long i = 0; i < k; i++) {
        log_T0 += logl((long double)(n - i) / (long double)n);
    }

    // log_T0 is very negative. We sum T_j using log-sum-exp.
    // log(T_j) = log(T_0) + sum of log(ratios)
    // We collect all log(T_j) and use log-sum-exp

    // Actually, let's find the maximum term first, then sum relative to it
    // Compute all log(T_j) values
    vector<long double> log_terms;
    long double log_Tj = log_T0;
    log_terms.push_back(log_Tj);

    for (long long j = 0; j < k / 2; j++) {
        long double log_ratio = logl((long double)(k - 2*j)) + logl((long double)(k - 2*j - 1))
                              - logl(2.0L) - logl((long double)(j + 1)) - logl((long double)(n - k + j + 1));
        log_Tj += log_ratio;
        log_terms.push_back(log_Tj);
        if (log_Tj < log_T0 - 100.0L) break; // negligible
    }

    // Log-sum-exp
    long double max_log = *max_element(log_terms.begin(), log_terms.end());
    long double sum_exp = 0.0L;
    for (auto lt : log_terms) {
        sum_exp += expl(lt - max_log);
    }
    long double log_P_complement = max_log + logl(sum_exp);

    // P(no chip has 3+) = exp(log_P_complement)
    // p(k,n) = 1 - exp(log_P_complement)
    // But if P_complement is close to 1, we need careful subtraction
    // log_P_complement should be close to log(0.27) ~ -1.3

    long double P_complement = expl(log_P_complement);
    long double answer = 1.0L - P_complement;

    printf("%.10Lf\n", answer);
    return 0;
}
