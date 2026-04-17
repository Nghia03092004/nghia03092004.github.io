"""
Project Euler Problem 297: Zeckendorf Representation

Find sum of z(n) for 0 < n < 10^17, where z(n) = number of terms
in the Zeckendorf representation of n.

Fibonacci: F[0]=1, F[1]=2, F[2]=3, F[3]=5, ...

S(N) = sum of z(n) for n in [1, N).

At Fibonacci boundaries:
  S(F[k+1]) = S(F[k]) + S(F[k-1]) + F[k-1]

General recursion (F[k] <= N < F[k+1]):
  S(N) = S(F[k]) + (N - F[k]) + S(N - F[k])
"""
import bisect

def solve():
    N = 10**17

    # Generate Fibonacci: F[0]=1, F[1]=2, F[2]=3, F[3]=5, ...
    F = [1, 2]
    while F[-1] < N:
        F.append(F[-1] + F[-2])
    M = len(F)

    # Precompute S(F[k]) for k = 0..M-1
    SF = [0] * M
    SF[0] = 0  # S(1) = 0
    if M >= 2:
        SF[1] = 1  # S(2) = z(1) = 1
    for i in range(2, M):
        SF[i] = SF[i-1] + SF[i-2] + F[i-2]

    def S(n):
        if n <= 1:
            return 0
        k = bisect.bisect_right(F, n) - 1
        if F[k] == n:
            return SF[k]
        return SF[k] + (n - F[k]) + S(n - F[k])

    print(S(N))

if __name__ == "__main__":
    solve()
