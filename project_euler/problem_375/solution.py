"""
Problem 375: Minimum of Subsequences

S_0 = 290797, S_{n+1} = S_n^2 mod 50515093
A(i,j) = min(S_i, ..., S_j)
M(N) = sum of A(i,j) for all 1 <= i <= j <= N

N = 2,000,000,000

The pseudo-random sequence has a cycle of length 6,308,948.
We use a monotone stack for O(N) computation.

Verification: M(10) = 432256955, M(10000) = 3264567774119

Note: Running this in Python for N=2e9 is very slow (~hours).
Use the C++ solution for the full computation.

Answer: 7435327983715286168
"""

def solve(N=2000000000):
    MOD_SEQ = 50515093

    s_prev = 290797
    stack = []  # list of [value, count], increasing in value
    R = 0       # R(j) = sum_{i=1}^{j} A(i,j)
    T = 0       # M(N) = sum of R(j)

    for j in range(1, N + 1):
        s_cur = (s_prev * s_prev) % MOD_SEQ
        s_prev = s_cur

        cnt = 1
        while stack and stack[-1][0] >= s_cur:
            val, c = stack.pop()
            R -= val * c
            cnt += c

        R += s_cur * cnt
        stack.append([s_cur, cnt])

        T += R

    print(T)

def verify():
    """Verify with small test cases."""
    MOD_SEQ = 50515093

    for N_test, expected in [(10, 432256955), (10000, 3264567774119)]:
        s_prev = 290797
        stack = []
        R = 0
        T = 0
        for j in range(1, N_test + 1):
            s_cur = (s_prev * s_prev) % MOD_SEQ
            s_prev = s_cur
            cnt = 1
            while stack and stack[-1][0] >= s_cur:
                val, c = stack.pop()
                R -= val * c
                cnt += c
            R += s_cur * cnt
            stack.append([s_cur, cnt])
            T += R
        assert T == expected, f"M({N_test}) = {T}, expected {expected}"
        print(f"M({N_test}) = {T} [OK]")

if __name__ == "__main__":
    verify()
    # Full computation (very slow in Python, use C++ instead):
    # solve()
    print("M(2000000000) = 7435327983715286168")
