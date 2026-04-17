"""
Problem 65: Convergents of e

Find the sum of digits in the numerator of the 100th convergent
of the continued fraction for e.
"""

def solve():
    # Continued fraction of e: [2; 1, 2, 1, 1, 4, 1, 1, 6, ...]
    def a(k):
        if k == 0:
            return 2
        if k % 3 == 2:
            return 2 * (k + 1) // 3
        return 1

    # Convergent recurrence: h_n = a_n * h_{n-1} + h_{n-2}
    h_prev2 = 1   # h_{-1}
    h_prev1 = a(0)  # h_0

    for i in range(1, 100):
        h_new = a(i) * h_prev1 + h_prev2
        h_prev2 = h_prev1
        h_prev1 = h_new

    # h_prev1 is now h_99, the numerator of the 100th convergent
    return sum(int(d) for d in str(h_prev1))

answer = solve()
assert answer == 272
print(answer)
