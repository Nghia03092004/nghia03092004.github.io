"""Project Euler Problem 4: Largest Palindrome Product"""

def is_palindrome(n: int) -> bool:
    s = str(n)
    return s == s[::-1]

def solve() -> int:
    best = 0
    # One factor must be divisible by 11 (Theorem 1 + Euclid's lemma)
    for a in range(990, 99, -11):
        for b in range(999, a - 1, -1):
            prod = a * b
            if prod <= best:
                break
            if is_palindrome(prod):
                best = prod
                break
    return best

answer = solve()
assert answer == 906609
print(answer)
