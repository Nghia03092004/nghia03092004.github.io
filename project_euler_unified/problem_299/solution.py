"""
Project Euler Problem 299: Three Similar Triangles

Points: A(a,0), B(b,0), C(0,c), D(0,d) with 0<a<b, 0<c<d, a=c.
P on line AC (integer coords) such that triangles ABP, CDP, BDP all similar.
Find # distinct triplets (a,b,d) with b+d < 10^8.

Two cases:

CASE 1 (Incenter/Pythagorean): b^2 + d^2 must be a perfect square.
  Primitive Pythagorean triples (m^2-n^2, 2mn, m^2+n^2) with m>n>0, gcd(m,n)=1, m+n odd.
  For each primitive triple with legs x, y: sum = x+y.
  Multiples k from 1 to (L-1)/sum. 2 triplets per k (swap b,d).

CASE 2 (Parallel, b=d): q^2 + 2f^2 = c^2.
  Parameterization: coprime (m,n), n odd.
  q = |n^2-2m^2|, f = 2mn, c = n^2+2m^2.
  b_prim = c + f = n^2 + 2mn + 2m^2, d = b = k*b_prim.
  b+d = 2*k*b_prim < L. Each k gives 1 triplet.
"""
from math import gcd

def solve():
    L = 100_000_000
    count = 0

    # CASE 1: Pythagorean triples
    n = 1
    while True:
        if 2*n*n + 4*n + 1 >= L:
            break
        m = n + 1
        while True:
            if (m + n) % 2 == 0:
                m += 1
                continue
            if gcd(m, n) != 1:
                m += 1
                continue
            x = m*m - n*n
            y = 2*m*n
            s = x + y
            if s >= L:
                break
            num_k = (L - 1) // s
            count += 2 * num_k
            m += 1
        n += 1

    # CASE 2: Parallel (Pell-type)
    n = 1
    while True:
        bmin = n*n + 2*n + 2
        if 2*bmin >= L:
            break
        if n % 2 == 1:  # n odd
            m = 1
            while True:
                if n*n == 2*m*m:
                    m += 1
                    continue
                if gcd(m, n) != 1:
                    m += 1
                    continue
                b_prim = n*n + 2*m*n + 2*m*m
                if 2*b_prim >= L:
                    break
                num_k = (L - 1) // (2 * b_prim)
                if num_k > 0:
                    count += num_k
                m += 1
        n += 1

    print(count)

if __name__ == "__main__":
    solve()
