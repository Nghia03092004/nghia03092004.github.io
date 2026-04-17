"""
Problem 273: Sum of Squares

For squarefree N that is a product of primes p = 1 mod 4 with p < 150,
let S(N) = sum of all a values where a^2 + b^2 = N, 0 <= a <= b.
Find sum of S(N) over all such N.

Primes = 1 mod 4 less than 150: 5,13,17,29,37,41,53,61,73,89,97,101,109,113,137,149
(16 primes)

Uses Gaussian integers and Brahmagupta-Fibonacci identity.
"""

def solve():
    # Find primes p = 1 mod 4, p < 150
    primes = []
    for p in range(2, 150):
        if all(p % d != 0 for d in range(2, int(p**0.5) + 1)) and p > 1 and p % 4 == 1:
            primes.append(p)

    # Gaussian integer representation for each prime
    prime_gi = []
    for p in primes:
        for a in range(1, p):
            b2 = p - a * a
            if b2 <= 0:
                break
            b = int(b2**0.5 + 0.5)
            if b * b == b2 and a <= b:
                prime_gi.append((a, b))
                break

    total = 0

    def dfs(idx, reps):
        nonlocal total
        for i in range(idx, len(primes)):
            pg = prime_gi[i]
            if not reps:
                new_reps = [pg]
            else:
                new_reps = []
                for (a, b) in reps:
                    c, d = pg
                    # Brahmagupta-Fibonacci: two new representations
                    a1 = abs(a * c - b * d)
                    b1 = abs(a * d + b * c)
                    a2 = abs(a * c + b * d)
                    b2 = abs(a * d - b * c)
                    if a1 > b1: a1, b1 = b1, a1
                    if a2 > b2: a2, b2 = b2, a2
                    new_reps.append((a1, b1))
                    new_reps.append((a2, b2))

            # Add S(N) for this subset
            for (a, b) in new_reps:
                total += a

            dfs(i + 1, new_reps)

    dfs(0, [])
    print(total)

if __name__ == "__main__":
    solve()
