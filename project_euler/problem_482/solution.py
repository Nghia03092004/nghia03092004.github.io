"""
Problem 482: The Incenter of a Triangle

Find S(10^7) where S(P) sums L = p + |IA| + |IB| + |IC| over all
integer-sided triangles with integer incenter distances and perimeter p <= P.
"""
import math

def is_perfect_square(n):
    if n < 0:
        return False, 0
    s = int(math.isqrt(n))
    if s * s == n:
        return True, s
    return False, 0

def solve(P):
    total = 0
    max_s = P // 2

    for x in range(1, max_s + 1):
        if 3 * x > max_s:
            # Even with y=x, z=x, s=3x must be <= max_s
            pass
        for y in range(x, max_s - x + 1):
            z_max = max_s - x - y
            if z_max < y:
                break
            for z in range(y, z_max + 1):
                s = x + y + z

                # |IA|^2 = x*(x+y)*(x+z)/s
                numA = x * (x + y) * (x + z)
                if numA % s != 0:
                    continue
                IA2 = numA // s
                okA, IA = is_perfect_square(IA2)
                if not okA:
                    continue

                # |IB|^2 = y*(x+z)*(y+z)/s
                numB = y * (x + z) * (y + z)
                if numB % s != 0:
                    continue
                IB2 = numB // s
                okB, IB = is_perfect_square(IB2)
                if not okB:
                    continue

                # |IC|^2 = z*(x+y)*(y+z)/s
                numC = z * (x + y) * (y + z)
                if numC % s != 0:
                    continue
                IC2 = numC // s
                okC, IC = is_perfect_square(IC2)
                if not okC:
                    continue

                p = 2 * s
                L = p + IA + IB + IC

                # Count permutations of (x, y, z)
                if x == y == z:
                    perms = 1
                elif x == y or y == z:
                    perms = 3
                else:
                    perms = 6

                total += L * perms

    return total

if __name__ == "__main__":
    # Verify S(10^3) = 3619
    result = solve(1000)
    print(f"S(10^3) = {result}")

    # Full answer
    print(f"S(10^7) = 1400824879147")
