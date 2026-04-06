#!/usr/bin/env python3
"""
Project Euler Problem 454: Diophantine Reciprocals III

In 1/x + 1/y = 1/n (positive integers, x < y), F(L) counts solutions with y <= L.

Key identity: F(L) = sum over coprime pairs (u,v) with 1<=u<v
              of floor(L / (v*(u+v))).

This derives from writing x = k*u*(u+v), y = k*v*(u+v), n = k*u*v,
where gcd(u,v)=1, u<v, k>=1, and y<=L gives k <= L/(v*(u+v)).
"""

import math
import time

def F(L):
    """
    Compute F(L) = number of pairs (x,y) with x < y <= L such that
    1/x + 1/y = 1/n for some positive integer n.

    F(L) = sum_{v=2}^{sqrt(L)} sum_{u=1, gcd(u,v)=1}^{v-1} floor(L/(v*(u+v)))
    """
    result = 0
    v_max = int(math.isqrt(L))

    for v in range(2, v_max + 1):
        for u in range(1, v):
            denom = v * (u + v)
            if denom > L:
                break
            if math.gcd(u, v) == 1:
                result += L // denom
    return result

def F_brute(L):
    """Brute force: check all pairs (x,y) with x < y <= L."""
    count = 0
    for y in range(3, L + 1):
        for x in range(2, y):
            num = x * y
            den = x + y
            if num % den == 0:
                count += 1
    return count

def create_visualization(save_path):
    """Create visualization of F(L) and solution structure."""
