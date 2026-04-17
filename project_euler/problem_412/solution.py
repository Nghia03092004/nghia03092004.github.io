"""
Project Euler Problem 412: Gnomon Numbering

Compute LC(m, n) = number of Standard Young Tableaux of the L-shaped
(gnomon) partition lambda = (m^{m-n}, (m-n)^n), modulo a prime p.

Uses the hook length formula: f^lambda = N! / prod h(i,j)
where hook lengths decompose into three rectangular regions.

Answer: LC(10000, 5000) mod 76543217 = 38788800
"""

def solve(m, n, p):
    """
    Compute LC(m, n) mod p using the hook length formula.

    The gnomon L(m, n) is an m x m grid with top-right n x n removed.
    As a Young diagram: partition = (m^{m-n}, (m-n)^n).

    The hook lengths fall into three rectangular regions:
      Region 1: rows [0, a), cols [0, a)    -> hook = 2m - i - j - 1
      Region 2: rows [0, a), cols [a, m)    -> hook = m + a - i - j - 1
      Region 3: rows [a, m), cols [0, a)    -> hook = a + m - i - j - 1
    where a = m - n.

    Each row's hook product is a ratio of consecutive factorials,
    enabling O(m) accumulation after O(m^2) factorial precomputation.
    """
    a = m - n
    N = m * m - n * n  # total cells

    # Precompute factorial and inverse factorial mod p
    max_val = max(N, 2 * m) + 10
    fact = [1] * (max_val + 1)
    for i in range(1, max_val + 1):
        fact[i] = fact[i - 1] * i % p

    inv_fact = [1] * (max_val + 1)
    inv_fact[max_val] = pow(fact[max_val], p - 2, p)
    for i in range(max_val - 1, -1, -1):
        inv_fact[i] = inv_fact[i + 1] * (i + 1) % p

    numerator = fact[N]
    denom = 1

    # Region 1: i in [0, a), j in [0, a)
    # Row i hook product = (2m-i-1)! / (2m-i-a-1)!
    for i in range(a):
        top = 2 * m - i - 1
        bot = 2 * m - i - a - 1
        denom = denom * fact[top] % p * inv_fact[bot] % p

    # Region 2: i in [0, a), j in [a, m)
    # Row i hook product = (m-i-1)! / (a-i-1)!
    for i in range(a):
        top = m - i - 1
        bot = a - i - 1
        denom = denom * fact[top] % p * inv_fact[bot] % p

    # Region 3: i in [a, m), j in [0, a)
    # Row i hook product = (a+m-i-1)! / (m-i-1)!
    for i in range(a, m):
        top = a + m - i - 1
        bot = m - i - 1
        denom = denom * fact[top] % p * inv_fact[bot] % p

    return numerator * pow(denom, p - 2, p) % p

def visualize_gnomon(m, n, filename='visualization.png'):
    """
    Create a visualization of the gnomon L(m, n) shape showing:
    1. The L-shaped grid with the removed corner highlighted
    2. The three hook-length regions color-coded
    3. A small example with actual hook lengths filled in
    """
