"""
Project Euler Problem 692: Siegbert and Jo

Fibonacci Nim variant. H(N) = smallest Fibonacci number in Zeckendorf
representation of N. Find G(23416728348467685) = sum of H(k) for k=1..n.

Uses digit-DP on Zeckendorf representation with precomputed:
  A[i] = count of non-empty non-consecutive subsets from {F[0],...,F[i]}
  S[i] = sum of minimums over such subsets
"""

def solve():
    # Zeckendorf Fibonacci: 1, 2, 3, 5, 8, 13, ...
    fib = [1, 2]
    while fib[-1] + fib[-2] <= 5 * 10**16:
        fib.append(fib[-1] + fib[-2])

    sz = len(fib)

    # Precompute A[i] and S[i]
    A = [0] * sz
    S = [0] * sz
    A[0] = 1
    S[0] = 1
    A[1] = 2
    S[1] = 3
    for i in range(2, sz):
        A[i] = A[i-1] + 1 + A[i-2]
        S[i] = S[i-1] + fib[i] + S[i-2]

    def zeckendorf(n):
        rep = []
        for i in range(sz - 1, -1, -1):
            if fib[i] <= n:
                rep.append(i)
                n -= fib[i]
        return rep

    def G(n):
        z = zeckendorf(n)
        m = len(z)
        ans = fib[z[m-1]]
        for j in range(m):
            if j == 0:
                if z[0] > 0:
                    ans += S[z[0] - 1]
            else:
                ans += fib[z[j-1]]
                if z[j] > 0:
                    ans += S[z[j] - 1]
        return ans

    n = 23416728348467685
    print(G(n))

if __name__ == "__main__":
    solve()
