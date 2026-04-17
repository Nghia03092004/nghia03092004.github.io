"""
Problem 225: Tribonacci Non-divisors

Find the 124th odd number that does not divide any Tribonacci number.
T(1) = T(2) = T(3) = 1, T(n) = T(n-1) + T(n-2) + T(n-3).

For each odd m, compute T(n) mod m. If 0 never appears in the cycle,
then m is a non-divisor.
"""

def does_not_divide_any_tribonacci(m):
    """Return True if m does not divide any Tribonacci number."""
    if m == 1:
        return False  # 1 divides T(1) = 1

    a, b, c = 1 % m, 1 % m, 1 % m

    for _ in range(m * m * m + 10):
        nxt = (a + b + c) % m
        a, b, c = b, c, nxt
        if c == 0:
            return False  # m divides some T(n)
        if a == 1 and b == 1 and c == 1:
            return True  # cycle completed, 0 never appeared

    return True  # shouldn't reach here for reasonable m

def solve():
    target = 124
    count = 0
    m = 1

    while True:
        if does_not_divide_any_tribonacci(m):
            count += 1
            if count == target:
                print(m)
                return
        m += 2

if __name__ == "__main__":
    solve()
