"""
Problem 154: Count triples (i,j,k) with i+j+k=200000 where
the trinomial coefficient IS divisible by 10^12.

v_p(C(n; i,j,k)) = v_p(n!) - v_p(i!) - v_p(j!) - v_p(k!)
We need v_2 >= 12 AND v_5 >= 12.

Note: This Python solution is slow for N=200000 (O(N^2) iterations).
For the actual answer, use the C++ version. This serves as a reference.
"""

def solve():
    N = 200000
    TARGET = 12

    # Precompute v_p(m!) for p=2 and p=5
    v2 = [0] * (N + 1)
    v5 = [0] * (N + 1)
    for m in range(1, N + 1):
        x = m
        c2 = 0
        while x % 2 == 0:
            c2 += 1
            x //= 2
        x = m
        c5 = 0
        while x % 5 == 0:
            c5 += 1
            x //= 5
        v2[m] = v2[m - 1] + c2
        v5[m] = v5[m - 1] + c5

    V2N = v2[N]
    V5N = v5[N]

    count = 0

    for i in range(N // 3 + 1):
        r2i = V2N - v2[i]
        r5i = V5N - v5[i]

        for j in range(i, (N - i) // 2 + 1):
            k = N - i - j
            val2 = r2i - v2[j] - v2[k]
            val5 = r5i - v5[j] - v5[k]

            if val2 >= TARGET and val5 >= TARGET:
                if i == j == k:
                    count += 1
                elif i == j or j == k:
                    count += 3
                else:
                    count += 6

    print(count)

solve()
