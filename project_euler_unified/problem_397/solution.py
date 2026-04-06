"""
Project Euler Problem 397 -- Triangle on Parabola

On the parabola y = x^2/k, three points with integer x-coordinates
a < b < c (with 0 < a < b < c <= k) form a triangle with area:

    Area = (b-a)(c-b)(c-a) / (2k)

S(k) = number of such triples with Area <= k,
i.e., (b-a)(c-b)(c-a) <= 2k^2.

Substituting u = b-a, v = c-b:
    u*v*(u+v) <= 2k^2,  u >= 1, v >= 1,  u+v <= k-1
    number of valid a values = k - u - v

S(k) = sum over valid (u,v) of (k - u - v).

For each u, find max v via solving u*v^2 + u^2*v <= 2k^2 (quadratic in v).
"""

import math
import time

def max_v_for_u(u: int, bound: int):
    """
    Find largest integer v >= 1 such that u * v * (u + v) <= bound.
    Equivalent to u*v^2 + u^2*v <= bound.
    Quadratic formula: v <= (-u + sqrt(u^2 + 4*bound/u)) / 2.
    """
    if u * 1 * (u + 1) > bound:
        return 0
    disc = u * u + 4.0 * bound / u
    v_max = int((-u + math.sqrt(disc)) / 2.0)
    # Correct for floating point imprecision
    while v_max >= 1 and u * v_max * (u + v_max) > bound:
        v_max -= 1
    while u * (v_max + 1) * (u + v_max + 1) <= bound:
        v_max += 1
    return max(v_max, 0)

def S(k: int):
    """Compute S(k): count of triples (a,b,c) with 0 < a < b < c <= k
    such that (b-a)(c-b)(c-a) <= 2k^2."""
    bound = 2 * k * k
    total = 0
    for u in range(1, k - 1):
        # Max v from area constraint
        vm_area = max_v_for_u(u, bound)
        if vm_area < 1:
            break  # monotone: larger u won't help
        # Max v from c <= k constraint: u + v <= k - 1
        vm_range = k - 1 - u
        if vm_range < 1:
            break
        vm = min(vm_area, vm_range)
        # Sum_{v=1}^{vm} (k - u - v) = vm*(k - u) - vm*(vm+1)/2
        total += vm * (k - u) - vm * (vm + 1) // 2
    return total

def S_brute(k: int):
    """Brute force S(k) for verification."""
    bound = 2 * k * k
    count = 0
    for a in range(1, k - 1):
        for b in range(a + 1, k):
            for c in range(b + 1, k + 1):
                if (b - a) * (c - b) * (c - a) <= bound:
                    count += 1
    return count

def verify_small():
    """Verify optimized solution against brute force for small k."""
    print("Verifying against brute force...")
    all_ok = True
    for k in range(3, 30):
        s_opt = S(k)
        s_brute_val = S_brute(k)
        status = "OK" if s_opt == s_brute_val else "FAIL"
        if status == "FAIL":
            all_ok = False
        print(f"  S({k:3d}) = {s_opt:8d}  (brute: {s_brute_val:8d})  [{status}]")
    print(f"Verification: {'PASSED' if all_ok else 'FAILED'}\n")
    return all_ok

def create_visualization(k_values, s_values):
    """Create visualization and save to visualization.png."""
