"""
Problem 617: Mirror Power Sequence
Find numbers where digit reversal relates to powers.
"""

def reverse_number(n):
    return int(str(n)[::-1])

def is_palindrome(n):
    s = str(n)
    return s == s[::-1]

def find_mirror_powers(limit):
    """Find n such that reverse(n^k) has special properties."""
    results = []
    for n in range(2, limit):
        for k in range(2, 10):
            pk = n ** k
            rev = reverse_number(pk)
            if rev == n and pk != n:
                results.append((n, k, pk))
    return results

# Simpler: find palindromic powers
def find_palindromic_powers(limit, max_k=10):
    results = []
    for n in range(2, limit):
        for k in range(2, max_k + 1):
            pk = n ** k
            if is_palindrome(pk):
                results.append((n, k, pk))
    return results

palins = find_palindromic_powers(1000, 5)
print(f"Palindromic powers (n < 1000, k <= 5):")
for n, k, pk in palins[:20]:
    print(f"  {n}^{k} = {pk}")

# Count palindromic squares
pal_sq = sum(1 for n in range(1, 10001) if is_palindrome(n * n))
print(f"Palindromic squares n^2 for n <= 10000: {pal_sq}")
