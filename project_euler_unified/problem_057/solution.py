"""
Project Euler Problem 57: Square Root Convergents

In the first 1000 expansions of the continued fraction for sqrt(2),
how many fractions contain a numerator with more digits than the denominator?

Recurrence: p_n = p_{n-1} + 2*q_{n-1}, q_n = p_{n-1} + q_{n-1}
Initial values: p_1 = 3, q_1 = 2

Answer: 153
"""

def solve():
    p, q = 3, 2
    count = 0
    for _ in range(1000):
        if len(str(p)) > len(str(q)):
            count += 1
        p, q = p + 2 * q, p + q
    return count

if __name__ == "__main__":
    answer = solve()
assert answer == 153
print(answer)
