"""Project Euler Problem 36: Double-base Palindromes"""

def is_binary_palindrome(n):
    rev, t = 0, n
    while t:
        rev = (rev << 1) | (t & 1)
        t >>= 1
    return rev == n

def generate_base10_palindromes(limit):
    pals = []
    for a in range(1, 10):
        if a < limit: pals.append(a)
    for a in range(1, 10):
        n = a * 11
        if n < limit: pals.append(n)
    for a in range(1, 10):
        for b in range(10):
            n = a * 101 + b * 10
            if n < limit: pals.append(n)
    for a in range(1, 10):
        for b in range(10):
            n = a * 1001 + b * 110
            if n < limit: pals.append(n)
    for a in range(1, 10):
        for b in range(10):
            for c in range(10):
                n = a * 10001 + b * 1010 + c * 100
                if n < limit: pals.append(n)
    for a in range(1, 10):
        for b in range(10):
            for c in range(10):
                n = a * 100001 + b * 10010 + c * 1100
                if n < limit: pals.append(n)
    return pals

total = sum(p for p in generate_base10_palindromes(1_000_000) if is_binary_palindrome(p))
print(total)
