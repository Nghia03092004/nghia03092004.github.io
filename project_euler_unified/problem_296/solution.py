"""
Project Euler Problem 296: Angular Bisector and Tangent

Triangle ABC with integer sides a=BC <= b=AC <= c=AB, perimeter <= 100000.
BE = c*a / (a+b) must be a positive integer.

With g = gcd(a,b), a = g*a', b = g*b', gcd(a',b')=1:
  BE = c*a'/(a'+b'), so (a'+b') | c, i.e., c = k*(a'+b').

Constraints:
  a' <= b' (from a <= b)
  k >= ceil(g*b'/(a'+b'))  (from b <= c)
  k < g  (from triangle inequality a+b > c)
  (g+k)*(a'+b') <= P  (perimeter)
"""
from math import gcd

def solve():
    P = 100000
    count = 0

    for bp in range(1, P // 2 + 1):
        if (bp + 1) * 2 > P:
            break
        for ap in range(1, bp + 1):
            if gcd(ap, bp) != 1:
                continue
            s = ap + bp
            if s * 2 > P:
                break
            max_gs = P // s  # g + k <= max_gs

            for g in range(2, max_gs):
                k_min = (g * bp + s - 1) // s  # ceil(g*bp/s)
                k_max = min(g - 1, max_gs - g)
                if k_min > k_max:
                    continue
                count += k_max - k_min + 1

    print(count)

if __name__ == "__main__":
    solve()
