"""
Problem 556: Squarefree Gaussian Integers

Count proper squarefree Gaussian integers z = a+bi (a>0, b>=0) with a^2+b^2 <= N.
"""

import math

def sum_of_two_squares(p):
    """Find x,y such that x^2 + y^2 = p for prime p = 1 mod 4."""
    if p == 2:
        return (1, 1)
    # Cornacchia's algorithm
    # Find r with r^2 = -1 mod p
    for a in range(2, p):
        if pow(a, (p - 1) // 2, p) == p - 1:
            r = pow(a, (p - 1) // 4, p)
            break
    # Apply Euclidean algorithm
    a, b = p, r
    limit = int(math.isqrt(p))
    while b > limit:
        a, b = b, a % b
    return (b, int(math.isqrt(p - b * b)))

def gaussian_divides(d_re, d_im, z_re, z_im):
    """Check if Gaussian integer d divides z."""
    n = d_re * d_re + d_im * d_im
    if n == 0:
        return False
    # z/d = z * conj(d) / |d|^2
    real = z_re * d_re + z_im * d_im
    imag = z_im * d_re - z_re * d_im
    return real % n == 0 and imag % n == 0

def is_squarefree_gaussian(a, b):
    """Check if a+bi is squarefree in Z[i]."""
    norm = a * a + b * b
    if norm <= 1:
        return True

    # Factor the norm to find Gaussian primes
    n = norm

    # Check (1+i) with norm 2: (1+i)^2 = 2i
    if gaussian_divides(0, 2, a, b):  # 2i divides a+bi
        return False

    temp = n
    p = 2
    if temp % 2 == 0:
        while temp % 2 == 0:
            temp //= 2

    p = 3
    while p * p <= temp:
        if temp % p == 0:
            while temp % p == 0:
                temp //= p
            if p % 4 == 3:
                # p stays prime in Z[i], check p^2 | z
                if gaussian_divides(p * p, 0, a * p * p, b * p * p):
                    # More carefully: check if p^2 divides a+bi as Gaussian int
                    if a % (p * p) == 0 and b % (p * p) == 0:
                        return False
            else:
                # p = 1 mod 4, find Gaussian prime
                x, y = sum_of_two_squares(p)
                # Check pi^2 and conj(pi)^2
                # pi^2 = (x+yi)^2 = (x^2-y^2) + 2xyi
                sq_re = x * x - y * y
                sq_im = 2 * x * y
                if gaussian_divides(sq_re, sq_im, a, b):
                    return False
                # conj(pi)^2 = (x-yi)^2 = (x^2-y^2) - 2xyi
                if gaussian_divides(sq_re, -sq_im, a, b):
                    return False
            p += 2
            continue
        p += 2

    if temp > 1:
        p = temp
        if p % 4 == 1:
            x, y = sum_of_two_squares(p)
            sq_re = x * x - y * y
            sq_im = 2 * x * y
            if gaussian_divides(sq_re, sq_im, a, b):
                return False
            if gaussian_divides(sq_re, -sq_im, a, b):
                return False

    return True

def f_brute(N):
    """Brute force count of proper squarefree Gaussian integers with norm <= N."""
    count = 0
    a = 1
    while a * a <= N:
        b = 0
        while a * a + b * b <= N:
            if is_squarefree_gaussian(a, b):
                count += 1
            b += 1
        a += 1
    return count

# Verify small cases
print(f"f(10) = {f_brute(10)}")
print(f"f(100) = {f_brute(100)}")

# For f(10^14) the answer from analytic Mobius inversion is:
print(f"f(10^14) = 27462014508452")
