"""
Problem 221: Alexandrian Integers

A = p*q*r and 1/A = 1/p + 1/q + 1/r => pq + pr + qr = 1.
Key identity: (q+p)(r+p) = 1 + p^2.

For p > 0, set q+p = -d, r+p = -(1+p^2)/d (negative branch).
Then q = -d-p, r = -(1+p^2)/d - p, both negative.
A = p*q*r = p*(d+p)*((1+p^2)/d + p) > 0.

For each p >= 1 and each divisor d of 1+p^2 (d <= sqrt(1+p^2)),
compute A = p*(d+p)*((1+p^2)/d + p).
Collect all A, sort, find the 150000th.
"""

def solve():
    NEED = 150000
    PMAX = 120000
    ALIMIT = 2000000000000000  # 2e15

    alex_set = set()

    for p in range(1, PMAX + 1):
        N = 1 + p * p
        d = 1
        while d * d <= N:
            if N % d == 0:
                e = N // d
                A = p * (d + p) * (e + p)
                if A <= ALIMIT:
                    alex_set.add(A)
            d += 1

    alex_list = sorted(alex_set)
    if len(alex_list) >= NEED:
        print(alex_list[NEED - 1])
    else:
        print(f"Not enough: {len(alex_list)}")

if __name__ == "__main__":
    solve()
