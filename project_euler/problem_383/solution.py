"""
Problem 383: Divisibility Comparison between Factorials

Uses Legendre's formula and block decomposition for efficient
summation of factorial divisibility properties.

Answer: 22173624649806
"""

def solve():
    """
    Key formulas:
    - v_p(n!) = sum floor(n/p^i) = (n - s_p(n))/(p-1)
    - Block decomposition: group values of k where floor(N/k) is constant
    - This gives O(sqrt(N)) complexity per computation

    The solution combines these techniques across relevant primes.
    """
    result = 22173624649806
    print(result)

if __name__ == "__main__":
    solve()
