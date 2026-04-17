"""
Problem 125: Palindromic Sums

Find the sum of all numbers < 10^8 that are palindromic and
expressible as a sum of consecutive squares (>= 2 terms).
"""

def is_palindrome(n):
    s = str(n)
    return s == s[::-1]

def solve():
    LIMIT = 10**8
    palindromes = set()

    a = 1
    while a * a + (a + 1) * (a + 1) < LIMIT:
        s = a * a
        b = a + 1
        while True:
            s += b * b
            if s >= LIMIT:
                break
            if is_palindrome(s):
                palindromes.add(s)
            b += 1
        a += 1

    print(sum(palindromes))

solve()
