#!/usr/bin/env python3
"""Project Euler Problem 697: Randomly Decaying Sequence"""

from scipy.stats import norm, chi2
import math

def solve():
    n = 10_000_000
    nu = 2 * n  # degrees of freedom for chi-squared

    # We need P(X_n < 1) = 0.25
    # X_n = c * prod(U_i), -ln(U_i) ~ Exp(1)
    # -2*sum(ln(U_i)) ~ chi^2(2n)
    # P(X_n < 1) = P(sum(-ln(U_i)) > ln(c)) = P(chi^2(2n) > 2*ln(c)) = 0.25
    # So 2*ln(c) = chi^2 quantile at 0.75

    chi2_val = chi2.ppf(0.75, nu)
    log10_c = chi2_val / (2 * math.log(10))

    print(f"{log10_c:.2f}")

    z_075 = norm.ppf(0.75)  # ~0.6744897501960817
    term = 1 - 2/(9*nu) + z_075 * math.sqrt(2/(9*nu))
    chi2_approx = nu * term**3
    log10_c_approx = chi2_approx / (2 * math.log(10))
    print(f"Wilson-Hilferty: {log10_c_approx:.2f}")

solve()
