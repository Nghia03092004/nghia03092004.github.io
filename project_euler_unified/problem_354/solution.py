"""
Problem 354: Distances in a Bee's Honeycomb

Find count of n <= 10^12 with C(n) = 450 on the hexagonal lattice.

C(n) = 6 * (d1(n) - d2(n)) where d1, d2 count divisors == 1, 2 mod 3.
Need d1(n) - d2(n) = 75 with all primes == 2 mod 3 at even exponents.
Product of (a_i + 1) for primes == 1 mod 3 must equal 75.

Exponent patterns: (74), (24,2), (14,4), (4,4,2)
"""

def solve():
    LIMIT = 10**12

    # Sieve primes up to 10^6
    sieve_bound = 1000001
    is_prime = [True] * sieve_bound
    is_prime[0] = is_prime[1] = False
    for i in range(2, sieve_bound):
        if is_prime[i]:
            for j in range(i*i, sieve_bound, i):
                is_prime[j] = False

    primes1 = [p for p in range(2, sieve_bound) if is_prime[p] and p % 3 == 1]
    primes2 = [p for p in range(2, sieve_bound) if is_prime[p] and p % 3 == 2]

    answer = 0

    def count_multipliers(limit, idx2, current):
        """Count numbers <= limit of form 3^a * product(q_j^(2*c_j))"""
        cnt = 0
        # Count all powers of 3 times current
        val = current
        while val <= limit:
            cnt += 1
            val *= 3

        # Add squares of primes == 2 mod 3
        for i in range(idx2, len(primes2)):
            q = primes2[i]
            q2 = q * q
            if current * q2 > limit:
                break
            nxt = current * q2
            while nxt <= limit:
                cnt += count_multipliers(limit, i + 1, nxt)
                if nxt > limit // q2:
                    break
                nxt *= q2

        return cnt

    def enumerate_patterns(exps, idx, p1_idx, current):
        nonlocal answer
        if idx == len(exps):
            rem = LIMIT // current
            answer += count_multipliers(rem, 0, 1)
            return

        e = exps[idx]
        for i in range(p1_idx, len(primes1)):
            p = primes1[i]
            pe = p ** e
            if pe > LIMIT:
                break
            if current * pe > LIMIT:
                break

            nxt_start = i + 1 if (idx + 1 < len(exps) and exps[idx + 1] == e) else 0
            enumerate_patterns(exps, idx + 1, nxt_start, current * pe)

    # Exponent patterns for 75 = 75, 25*3, 15*5, 5*5*3
    patterns = [
        [74],
        [24, 2],
        [14, 4],
        [4, 4, 2],
    ]

    for pat in patterns:
        enumerate_patterns(pat, 0, 0, 1)

    print(answer)

if __name__ == "__main__":
    solve()
