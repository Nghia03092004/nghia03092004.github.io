"""
Problem 329: Prime Frog

A frog on squares 1..500 croaks P on primes (2/3), N on non-primes (2/3).
Find P(sequence PPPPNNPPPNPPNPN) as a reduced fraction.

Answer: 199740353/29386561536000
"""

from fractions import Fraction

def sieve(n):
    """Sieve of Eratosthenes returning set of primes up to n."""
    is_prime = [False, False] + [True] * (n - 1)
    for i in range(2, int(n**0.5) + 1):
        if is_prime[i]:
            for j in range(i*i, n + 1, i):
                is_prime[j] = False
    return is_prime

def solve():
    N = 500
    sequence = "PPPPNNPPPNPPNPN"
    L = len(sequence)

    is_prime = sieve(N)

    def croak_prob(square, letter):
        """Probability of croaking 'letter' on 'square'."""
        if is_prime[square]:
            return Fraction(2, 3) if letter == 'P' else Fraction(1, 3)
        else:
            return Fraction(1, 3) if letter == 'P' else Fraction(2, 3)

    # dp[i] = probability of being on square i having produced sequence so far
    # Initialize for step 0 (first croak)
    dp = [Fraction(0)] * (N + 1)
    for s in range(1, N + 1):
        dp[s] = Fraction(1, N) * croak_prob(s, sequence[0])

    # Process remaining croaks
    for t in range(1, L):
        new_dp = [Fraction(0)] * (N + 1)
        for j in range(1, N + 1):
            # Sum over neighbors
            prob_arrive = Fraction(0)
            if j == 1:
                # Can only come from 2
                prob_arrive = dp[2]  # forced jump, probability 1
            elif j == N:
                # Can only come from N-1
                prob_arrive = dp[N - 1]  # forced jump, probability 1
            else:
                # Come from j-1 or j+1 with prob 1/2 each
                # But we need to account for boundary neighbors
                half = Fraction(1, 2)
                if j - 1 == 1:
                    # Square 1 only jumps to 2, so prob of 1->j is 1 if j==2
                    prob_arrive += dp[j - 1] * (Fraction(1, 1) if j == 2 else Fraction(0))
                elif j - 1 == N:
                    prob_arrive += dp[j - 1] * (Fraction(1, 1) if j == N - 1 else Fraction(0))
                else:
                    prob_arrive += dp[j - 1] * half

                if j + 1 == 1:
                    prob_arrive += dp[j + 1] * (Fraction(1, 1) if j == 2 else Fraction(0))
                elif j + 1 == N:
                    prob_arrive += dp[j + 1] * (Fraction(1, 1) if j == N - 1 else Fraction(0))
                elif j + 1 <= N:
                    prob_arrive += dp[j + 1] * half

            new_dp[j] = prob_arrive * croak_prob(j, sequence[t])
        dp = new_dp

    total = sum(dp[i] for i in range(1, N + 1))
    print(total)
    # Expected: 199740353/29386561536000

if __name__ == "__main__":
    solve()
