#include <bits/stdc++.h>
using namespace std;

int main() {
    // We need P(X_n < 1) = 0.25 where X_n = c * prod(U_i)
    // This gives 2*ln(c) = chi^2_{2n, 0.75}
    // Using Wilson-Hilferty approximation for chi-squared quantile

    long long n = 10000000LL;
    double nu = 2.0 * n;

    // z_{0.75} = inverse normal CDF at 0.75
    // Using the known value
    double z = 0.6744897501960817;

    // Wilson-Hilferty approximation
    double term = 1.0 - 2.0 / (9.0 * nu) + z * sqrt(2.0 / (9.0 * nu));
    double chi2 = nu * term * term * term;

    // 2*ln(c) = chi2, so ln(c) = chi2/2
    // log10(c) = ln(c) / ln(10) = chi2 / (2 * ln(10))
    double log10c = chi2 / (2.0 * log(10.0));

    // Round to 2 decimal places
    cout << fixed << setprecision(2) << log10c << endl;

    return 0;
}
