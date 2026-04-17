from math import gcd

def solve():
    # streak(n) = k: smallest k such that n+k NOT divisible by k+1
    # streak(n) >= s: n+j divisible by j+1 for j=1..s-1
    #   i.e., n ≡ 1 (mod lcm(2,3,...,s))
    # streak(n) = s: streak >= s but not >= s+1
    # P(s, N) = floor((N-2)/lcm(2,...,s)) - floor((N-2)/lcm(2,...,s+1))
    # Special case s=1: P(1,N) = (N-2) - floor((N-2)/2)

    def lcm(a, b):
        return a // gcd(a, b) * b

    # L[k] = lcm(2, 3, ..., k)
    L = {1: 1}
    cur = 1
    for k in range(2, 34):
        cur = lcm(cur, k)
        L[k] = cur

    def P(s, N):
        Nm2 = N - 2
        if s == 1:
            c1 = Nm2
        else:
            Ls = L[s]
            c1 = Nm2 // Ls if Ls <= Nm2 else 0
        Ls1 = L[s + 1]
        c2 = Nm2 // Ls1 if Ls1 <= Nm2 else 0
        return c1 - c2

    ans = sum(P(i, 4**i) for i in range(1, 32))
    print(ans)

solve()
