"""
Problem 307: Chip Defects

k=20000 defects randomly distributed among n=1000000 chips.
p(k,n) = probability that at least one chip has 3+ defects.

P(complement) = sum_{j=0}^{k/2} C(n,j)*C(n-j,k-2j)*k! / (2^j * n^k)
p(k,n) = 1 - P(complement)

Using ratio: T_{j+1}/T_j = (k-2j)(k-2j-1) / (2(j+1)(n-k+j+1))

We compute in log-space and use mpmath for high precision.

Answer: 0.7311720251
"""

from mpmath import mp, mpf, log, exp, fsum, log1p

def solve():
    mp.dps = 30  # 30 decimal places of precision

    n = 1000000
    k = 20000

    # Compute log(T_j) for each j using log-space arithmetic
    # log(T_0) = sum_{i=0}^{k-1} log((n-i)/n) = sum log(1 - i/n)
    log_T0 = fsum(log1p(mpf(-i) / mpf(n)) for i in range(k))

    # Collect log(T_j) for all j
    log_terms = [log_T0]
    log_Tj = log_T0
    for j in range(k // 2):
        num = mpf(k - 2 * j) * mpf(k - 2 * j - 1)
        den = mpf(2) * mpf(j + 1) * mpf(n - k + j + 1)
        log_ratio = log(num / den)
        log_Tj += log_ratio
        log_terms.append(log_Tj)
        # Stop when terms become negligible
        if log_Tj < log_T0 - 100:
            break

    # Log-sum-exp to compute P(complement)
    max_log = max(log_terms)
    sum_exp = fsum(exp(lt - max_log) for lt in log_terms)
    log_P = max_log + log(sum_exp)
    P_complement = exp(log_P)

    answer = 1 - P_complement
    print(mp.nstr(answer, 10, strip_zeros=False))
