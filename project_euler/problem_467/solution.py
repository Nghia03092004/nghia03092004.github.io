"""
Project Euler Problem 467: Superinteger

Find f(10000) mod 1,000,000,007 where f(n) is the smallest superinteger
(shortest common supersequence as a number) of the digital root sequences
of the first n primes and first n composites.

The key algorithm: lexicographically smallest SCS via forward construction
using suffix LCS tables.
"""

import sys

MOD = 1_000_000_007

def digital_root(n):
    if n == 0:
        return 0
    return 1 + (n - 1) % 9

def sieve_primes(limit):
    is_prime = [True] * (limit + 1)
    is_prime[0] = is_prime[1] = False
    for i in range(2, int(limit**0.5) + 1):
        if is_prime[i]:
            for j in range(i*i, limit + 1, i):
                is_prime[j] = False
    return is_prime

def generate_sequences(n):
    limit = max(15 * n, 200000)
    is_prime = sieve_primes(limit)
    primes = []
    composites = []
    for i in range(2, limit + 1):
        if is_prime[i]:
            primes.append(i)
        else:
            composites.append(i)
        if len(primes) >= n and len(composites) >= n:
            break
    sp = [digital_root(primes[i]) for i in range(n)]
    sc = [digital_root(composites[i]) for i in range(n)]
    return sp, sc

def solve(n, verbose=False):
    """
    Solve for f(n) mod MOD.

    The SCS of two sequences A, B can be built forward using:
    - suffix LCS table: lcs[i][j] = LCS length of A[i:] and B[j:]
    - scs_rem(i,j) = (m-i) + (n-j) - lcs[i][j]

    To build the lexicographically smallest SCS, at each step from state (i,j)
    we must choose the next character. The next character MUST be either A[i]
    or B[j] (or both if they're equal), because:
    - The SCS must contain all of A and all of B as subsequences
    - If the next character were neither A[i] nor B[j], we'd need to still
      fit A[i:] and B[j:] as subsequences of the remaining SCS, plus we used
      one extra character, making it longer than optimal.

    So at state (i,j):
    - If i == m: must take B[j], move to (i, j+1)
    - If j == n: must take A[i], move to (i+1, j)
    - If A[i] == B[j]: take that digit, move to (i+1, j+1)
    - Else: we MUST take either A[i] or B[j] (whichever keeps SCS shortest).
      If both keep it shortest, pick the smaller digit.
      If same digit value, need deeper tie-breaking.
    """
    sp, sc = generate_sequences(n)
    A, B = sp, sc
    m, nn = len(A), len(B)

    if verbose:
        print(f"Generated sequences of length {n}")
        print(f"P_{n} starts with: {''.join(map(str, A[:20]))}")
        print(f"C_{n} starts with: {''.join(map(str, B[:20]))}")

    # Compute suffix LCS table
    lcs = [[0] * (nn + 1) for _ in range(m + 1)]
    for i in range(m - 1, -1, -1):
        for j in range(nn - 1, -1, -1):
            if A[i] == B[j]:
                lcs[i][j] = lcs[i+1][j+1] + 1
            else:
                lcs[i][j] = max(lcs[i+1][j], lcs[i][j+1])

    def scs_rem(i, j):
        return (m - i) + (nn - j) - lcs[i][j]

    total_len = scs_rem(0, 0)
    lcs_len = lcs[0][0]
    if verbose:
        print(f"LCS length: {lcs_len}")
        print(f"SCS length: {total_len}")

    # Build SCS forward
    val = 0
    i, j = 0, 0
    scs_digits = []

    while i < m or j < nn:
        if i == m:
            d = B[j]
            j += 1
        elif j == nn:
            d = A[i]
            i += 1
        elif A[i] == B[j]:
            d = A[i]
            i += 1
            j += 1
        else:
            # Try taking A[i] vs B[j]
            # Taking A[i]: new state (i+1, j), new scs_rem = scs_rem(i+1, j)
            # Taking B[j]: new state (i, j+1), new scs_rem = scs_rem(i, j+1)
            # Current scs_rem = scs_rem(i, j)
            # Valid option needs: 1 + scs_rem(new) == scs_rem(i, j)
            rem_a = scs_rem(i + 1, j)
            rem_b = scs_rem(i, j + 1)
            cur = scs_rem(i, j)

            opt_a = (1 + rem_a == cur)
            opt_b = (1 + rem_b == cur)

            if opt_a and opt_b:
                # Both valid, pick smaller digit
                if A[i] < B[j]:
                    d = A[i]
                    i += 1
                elif B[j] < A[i]:
                    d = B[j]
                    j += 1
                else:
                    # Same digit but A[i] == B[j] -- handled above, shouldn't reach here
                    d = A[i]
                    i += 1
            elif opt_a:
                d = A[i]
                i += 1
            elif opt_b:
                d = B[j]
                j += 1
            else:
                # This shouldn't happen
                print(f"ERROR: no valid option at ({i},{j})")
                break

        scs_digits.append(d)
        val = (val * 10 + d) % MOD

    if verbose:
        print(f"f({n}) mod {MOD} = {val}")
    return val, scs_digits, A, B, lcs_len

def create_visualization(sp, sc, lcs_len, n):
    """Return a compact preview of the generated sequences."""
    preview = min(25, n)
    return {
        "n": n,
        "lcs_length": lcs_len,
        "prime_prefix": "".join(map(str, sp[:preview])),
        "composite_prefix": "".join(map(str, sc[:preview])),
    }


def parse_args(argv):
    n = 10000
    verbose = False

    for arg in argv:
        if arg == "--verbose":
            verbose = True
            continue
        n = int(arg)

    return n, verbose


def main(argv=None):
    n, verbose = parse_args(sys.argv[1:] if argv is None else argv)
    value, _, _, _, _ = solve(n, verbose=verbose)
    print(value)


if __name__ == "__main__":
    main()
