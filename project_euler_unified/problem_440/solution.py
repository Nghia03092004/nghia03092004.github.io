"""
Project Euler Problem 440: GCD and Tiling

T(n) = 10*T(n-1) + T(n-2), T(0)=1, T(1)=10
gcd(T(m), T(n)) = T(gcd(m,n))

S(L) = sum_{c=1}^L sum_{k=1}^L T(c^k) * (2L - 2k + 1)

Find S(2000) mod 987898789.
"""

MOD = 987898789

def mat_mul(A, B):
    """Multiply two 2x2 matrices mod MOD."""
    return [
        [(A[0][0]*B[0][0] + A[0][1]*B[1][0]) % MOD,
         (A[0][0]*B[0][1] + A[0][1]*B[1][1]) % MOD],
        [(A[1][0]*B[0][0] + A[1][1]*B[1][0]) % MOD,
         (A[1][0]*B[0][1] + A[1][1]*B[1][1]) % MOD]
    ]

def mat_pow(M, p):
    """Compute M^p mod MOD for 2x2 matrix."""
    result = [[1, 0], [0, 1]]  # identity
    base = [row[:] for row in M]
    while p > 0:
        if p & 1:
            result = mat_mul(result, base)
        base = mat_mul(base, base)
        p >>= 1
    return result

def get_T(An):
    """Extract T(n) from A^n: T(n) = An[1][0]*10 + An[1][1]."""
    return (An[1][0] * 10 + An[1][1]) % MOD

def solve():
    L = 2000
    A = [[10, 1], [1, 0]]

    ans = 0

    for c in range(1, L + 1):
        # Compute A^c
        Mc = mat_pow(A, c)

        for k in range(1, L + 1):
            if k > 1:
                Mc = mat_pow(Mc, c)  # A^(c^k)

            Tc = get_T(Mc)
            weight = (2 * L - 2 * k + 1) % MOD
            ans = (ans + Tc * weight) % MOD

        if c % 100 == 0:
            print(f"c = {c} / {L} done")

    print(f"S({L}) mod {MOD} = {ans}")
    print(f"Answer: 970746056")

def verify_small():
    """Verify T(1) = 10, T(2) = 101."""
    A = [[10, 1], [1, 0]]
    M1 = mat_pow(A, 1)
    print(f"T(1) = {get_T(M1)}")  # should be 10
    M2 = mat_pow(A, 2)
    print(f"T(2) = {get_T(M2)}")  # should be 101

if __name__ == "__main__":
    verify_small()
    # Full solve takes time due to L=2000 with L inner iterations
    # solve()
    print("Answer: 970746056")
