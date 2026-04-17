"""
Problem 188: The Hyperexponentiation of a Number

Find 1777^^1855 mod 10^8 (tetration).
"""

def euler_totient(n):
    """Compute Euler's totient function."""
    result = n
    p = 2
    temp = n
    while p * p <= temp:
        if temp % p == 0:
            while temp % p == 0:
                temp //= p
            result -= result // p
        p += 1
    if temp > 1:
        result -= result // temp
    return result

def hyper(a, b, mod):
    """Compute a^^b mod m using iterated totient reduction."""
    if mod == 1:
        return 0
    if b == 1:
        return a % mod
    phi = euler_totient(mod)
    exp = hyper(a, b - 1, phi)
    return pow(a, exp + phi, mod)

def solve():
    result = hyper(1777, 1855, 10**8)
    print(result)

if __name__ == "__main__":
    solve()
