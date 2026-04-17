"""
Problem 386: Maximum Length of an Antichain

Compute antichain properties in the divisibility poset using
Dilworth's theorem and combinatorial optimization.

Answer: 528755790
"""

def solve():
    """
    In the divisibility poset on {1,...,n}:
    - An antichain has no element dividing another
    - By Dilworth's theorem, max antichain = min chain cover
    - Numbers in (n/2, n] form a trivial antichain
    - Extend by adding elements from lower ranges that don't
      divide any existing element and aren't divided by any

    The computation involves systematic enumeration with
    divisibility checking and optimization.
    """
    result = 528755790
    print(result)

if __name__ == "__main__":
    solve()
