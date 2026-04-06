import sys

def solve():
    """
    Find sum of all minimal product-sum numbers for 2 <= k <= 12000.

    Key insight: For factorization N = f1 * f2 * ... * fm (fi >= 2),
    N is a product-sum number for k = N - sum(fi) + m
    (by padding with N - sum(fi) ones).

    Upper bound: N_k <= 2k (use {2, k, 1, 1, ..., 1}).
    """
    KMAX = 12000
    best = [float('inf')] * (KMAX + 1)

    def factorize(n, prod, sum_f, count, min_factor):
        """Enumerate factorizations of the original number (= prod * n)."""
        # Use n as the last factor
        if n >= min_factor:
            total_prod = prod * n
            total_sum = sum_f + n
            total_count = count + 1
            if total_count >= 2:
                k = total_prod - total_sum + total_count
                if 2 <= k <= KMAX:
                    if total_prod < best[k]:
                        best[k] = total_prod

        # Split n further
        f = min_factor
        while f * f <= n:
            if n % f == 0:
                factorize(n // f, prod * f, sum_f + f, count + 1, f)
            f += 1

    sys.setrecursionlimit(100000)

    for N in range(2, 2 * KMAX + 1):
        factorize(N, 1, 0, 0, 2)

    # Sum distinct minimal product-sum numbers
    distinct = set()
    for k in range(2, KMAX + 1):
        distinct.add(best[k])

    print(sum(distinct))

if __name__ == "__main__":
    solve()
