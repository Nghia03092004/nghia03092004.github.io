"""
Problem 619: Square Subsets
Count subsets of {1,...,n} whose product is a perfect square.
"""
from math import isqrt

def sieve(n):
    is_prime = [True] * (n + 1)
    is_prime[0] = is_prime[1] = False
    for i in range(2, int(n**0.5) + 1):
        if is_prime[i]:
            for j in range(i*i, n + 1, i):
                is_prime[j] = False
    return [i for i in range(2, n + 1) if is_prime[i]]

def count_square_subsets(n):
    """Count non-empty subsets of {1,...,n} whose product is a perfect square.
    
    Represent each number by its exponent vector mod 2 over primes <= n.
    A subset has square product iff the XOR (sum mod 2) of all vectors is zero.
    
    This is equivalent to counting elements of the kernel of a GF(2) matrix.
    If the rank of the matrix is r and we have n vectors, then the number
    of subsets with zero sum is 2^(n-r) - 1 (excluding empty set).
    """
    primes = sieve(n)
    # Build matrix: each row is the exponent vector of a number mod 2
    matrix = []
    for k in range(1, n + 1):
        vec = [0] * len(primes)
        temp = k
        for i, p in enumerate(primes):
            while temp % p == 0:
                vec[i] ^= 1
                temp //= p
        matrix.append(vec)
    
    # Gaussian elimination over GF(2) to find rank
    m = len(primes)
    rows = [row[:] for row in matrix]
    rank = 0
    for col in range(m):
        # Find pivot
        pivot = -1
        for row in range(rank, n):
            if rows[row][col] == 1:
                pivot = row
                break
        if pivot == -1:
            continue
        rows[rank], rows[pivot] = rows[pivot], rows[rank]
        for row in range(n):
            if row != rank and rows[row][col] == 1:
                for c in range(m):
                    rows[row][c] ^= rows[rank][c]
        rank += 1
    
    # Number of subsets with square product = 2^(n - rank) - 1
    return pow(2, n - rank) - 1

# Verify small cases
# {1}: product=1=1^2, square. So count includes {1}.
c3 = count_square_subsets(3)
print(f"Square subsets of {{1,2,3}}: {c3}")
# {1}, {1,2,...} let's see: {1}->1, ok. Any subset containing only 1 and squares.

c10 = count_square_subsets(10)
print(f"Square subsets of {{1,...,10}}: {c10}")

c20 = count_square_subsets(20)
print(f"Square subsets of {{1,...,20}}: {c20}")
