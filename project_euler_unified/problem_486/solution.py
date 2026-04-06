"""
Project Euler 486: Palindrome-containing Strings

F5(n) = number of binary strings of length <= n containing a palindromic
substring of length >= 5.

D(L) = count of n in [5, L] where F5(n) is divisible by 87654321.
Find D(10^18).

Approach:
- G(n) = number of palindrome-free binary strings of length n
- Transfer matrix of 16 states (last 4 bits)
- F5(n) = sum_{k=5}^{n} (2^k - G(k))
- Find period of F5(n) mod 87654321 and count zeros
"""

MOD = 87654321

def build_transfer_matrix():
    """Build 16x16 transfer matrix. State = last 4 bits."""
    T = [[0]*16 for _ in range(16)]
    for s in range(16):
        a = (s >> 3) & 1
        b = (s >> 2) & 1
        d = s & 1
        for e in range(2):
            if e == a and d == b:
                continue
            ns = ((s << 1) | e) & 0xF
            T[ns][s] += 1
    return T

def mat_vec_mod(A, v, mod):
    n = len(v)
    result = [0]*n
    for i in range(n):
        s = 0
        row = A[i]
        for j in range(n):
            s += row[j] * v[j]
        result[i] = s % mod
    return result

def verify_small():
    """Verify F5 for small values using brute force."""
    def has_palindrome5(s):
        n = len(s)
        for i in range(n - 4):
            sub = s[i:i+5]
            if sub == sub[::-1]:
                return True
        return False

    # Count F5(n) by brute force for small n
    for n in [5, 6, 11]:
        count = 0
        for length in range(5, n+1):
            for val in range(2**length):
                s = bin(val)[2:].zfill(length)
                if has_palindrome5(s):
                    count += 1
        print(f"F5({n}) = {count}")

def solve():
    verify_small()

    # Now use transfer matrix to compute G(n) mod M
    T = build_transfer_matrix()
    cur = [1]*16  # all 4-bit strings exist at length 4

    # Verify G(5)
    cur5 = mat_vec_mod(T, cur, 10**15)
    g5 = sum(cur5)
    print(f"G(5) = {g5}, F5(5) should be {2**5 - g5} = 8")

    # Compute F5(n) mod M for larger n
    # Reset
    cur = [1]*16
    pow2 = 32  # 2^5
    cumF5 = 0
    count = 0

    for n in range(5, 200001):
        cur = mat_vec_mod(T, cur, MOD)
        g_n = sum(cur) % MOD
        cumF5 = (cumF5 + pow2 - g_n) % MOD
        if cumF5 == 0:
            count += 1
            if count <= 5:
                print(f"  F5({n}) ≡ 0 (mod {MOD})")
        pow2 = pow2 * 2 % MOD

    print(f"D(200000) = {count}")

solve()
