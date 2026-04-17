"""
Problem 146: Investigating a Prime Pattern

Find the sum of all n, 0 < n < 150,000,000, such that
n^2+1, n^2+3, n^2+7, n^2+9, n^2+13, n^2+27 are consecutive primes.
"""

def is_prime_miller_rabin(n):
    """Deterministic Miller-Rabin for n < 3.317e14 (sufficient witnesses)."""
    if n < 2:
        return False
    if n < 4:
        return True
    if n % 2 == 0 or n % 3 == 0:
        return False
    # For n < 3.317 * 10^14, witnesses {2,3,5,7,11,13,17} suffice.
    # Our max is ~(1.5e8)^2 + 27 ~ 2.25e16, so use more witnesses.
    d = n - 1
    r = 0
    while d % 2 == 0:
        d //= 2
        r += 1
    for a in [2, 3, 5, 7, 11, 13, 17, 19, 23, 29]:
        if a >= n:
            continue
        x = pow(a, d, n)
        if x == 1 or x == n - 1:
            continue
        for _ in range(r - 1):
            x = pow(x, x, n) if False else (x * x) % n
            if x == n - 1:
                break
        else:
            return False
    return True

def solve():
    LIMIT = 150_000_000

    # Precompute valid residues mod 210
    valid_residues = []
    for r in range(210):
        if r % 2 != 0:
            continue
        if r % 3 == 0:
            continue
        if r % 5 != 0:
            continue
        # Check mod 7
        r2 = r * r
        ok = True
        for k in [1, 3, 7, 9, 13, 27]:
            if (r2 + k) % 7 == 0:
                ok = False
                break
        if ok:
            valid_residues.append(r)

    must_be_prime = [1, 3, 7, 9, 13, 27]
    must_not_be_prime = [5, 11, 15, 17, 19, 21, 23, 25]

    total = 0
    for base in range(0, LIMIT, 210):
        for r in valid_residues:
            n = base + r
            if n <= 0 or n >= LIMIT:
                continue

            n2 = n * n

            ok = True
            for k in must_be_prime:
                if not is_prime_miller_rabin(n2 + k):
                    ok = False
                    break
            if not ok:
                continue

            for k in must_not_be_prime:
                if is_prime_miller_rabin(n2 + k):
                    ok = False
                    break
            if not ok:
                continue

            total += n

    print(total)

if __name__ == "__main__":
    solve()
