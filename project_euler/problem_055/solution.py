"""
Problem 55: Lychrel Numbers

How many Lychrel numbers are there below ten-thousand?
(Numbers that don't become palindromes within 50 iterations of reverse-and-add)
"""


def is_palindrome(n):
    s = str(n)
    return s == s[::-1]


def is_lychrel(n, max_iter=50):
    for _ in range(max_iter):
        n = n + int(str(n)[::-1])
        if is_palindrome(n):
            return False
    return True


def solve():
    count = sum(1 for n in range(1, 10000) if is_lychrel(n))
    print(count)

solve()
