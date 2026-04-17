"""
Project Euler Problem 56: Powerful Digit Sum

Considering natural numbers of the form a^b, where a, b < 100,
find the maximum digital sum.

Answer: 972
"""

def digit_sum(n):
    """Return the sum of the decimal digits of n."""
    s = 0
    while n > 0:
        s += n % 10
        n //= 10
    return s

def solve():
    max_sum = 0
    for a in range(2, 100):
        power = 1
        for b in range(1, 100):
            power *= a
            s = digit_sum(power)
            if s > max_sum:
                max_sum = s
    return max_sum

if __name__ == "__main__":
    answer = solve()
assert answer == 972
print(answer)
