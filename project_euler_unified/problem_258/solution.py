"""
Problem 258: A Lagged Fibonacci Sequence

g(k) = g(k-2000) + g(k-1999) for k >= 2000
g(k) = 1 for 0 <= k <= 1999

Find g(10^18) mod 20092010.

Characteristic polynomial: p(x) = x^2000 - x - 1
So x^2000 ≡ x + 1 (mod p(x))

Compute x^(10^18) mod p(x) in Z_m[x], then
g(10^18) = sum of coefficients (since all g(i) = 1 for i < 2000).

Answer: 12747994
"""

N = 2000
MOD = 20092010

def reduce_poly(a):
    """Reduce polynomial modulo p(x) = x^2000 - x - 1.
    Uses: x^2000 ≡ x + 1 (mod p(x))."""
    while len(a) > N:
        coeff = a.pop()
        deg = len(a)  # after pop, this is the original degree
        base = deg - N
        a[base] = (a[base] + coeff) % MOD
        a[base + 1] = (a[base + 1] + coeff) % MOD
    return a

def poly_mul(a, b):
    """Multiply two polynomials mod p(x) mod MOD."""
    sa, sb = len(a), len(b)
    c = [0] * (sa + sb - 1)
    for i in range(sa):
        if a[i] == 0:
            continue
        ai = a[i]
        for j in range(sb):
            c[i + j] = (c[i + j] + ai * b[j]) % MOD
    return reduce_poly(c)

def poly_pow(exp_val):
    """Compute x^exp_val mod p(x) mod MOD using binary exponentiation."""
    result = [1]
    base = [0, 1]

    while exp_val > 0:
        if exp_val & 1:
            result = poly_mul(result, base)
        base = poly_mul(base, base)
        exp_val >>= 1

    return result

def main():
    exp_val = 10**18
    result = poly_pow(exp_val)

    # g(10^18) = sum of coefficients a_i * g(i), and g(i) = 1 for all i < 2000
    ans = sum(result) % MOD
    print(ans)

if __name__ == "__main__":
    main()
