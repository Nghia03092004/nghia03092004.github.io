"""
Project Euler Problem 727: Triangle of Circular Arcs

Compute E(d) where d = |DE|, D = circumcenter of tangency triangle,
E = inner Soddy circle center.
Triples (ra, rb, rc) with 1 <= ra < rb < rc <= 100, gcd = 1.
"""

import numpy as np
from math import gcd, sqrt, atan2, cos, sin

def gcd3(a, b, c):
    return gcd(a, gcd(b, c))

def circumcenter(A, B, C):
    """Circumcenter of triangle with vertices A, B, C (as 2D arrays)."""
    ax, ay = A
    bx, by = B
    cx, cy = C
    D = 2 * (ax*(by-cy) + bx*(cy-ay) + cx*(ay-by))
    if abs(D) < 1e-15:
        return np.array([0.0, 0.0])
    ux = ((ax**2+ay**2)*(by-cy) + (bx**2+by**2)*(cy-ay) + (cx**2+cy**2)*(ay-by)) / D
    uy = ((ax**2+ay**2)*(cx-bx) + (bx**2+by**2)*(ax-cx) + (cx**2+cy**2)*(bx-ax)) / D
    return np.array([ux, uy])

def soddy_center(ka, kb, kc, Oa, Ob, Oc):
    """Compute inner Soddy circle center using complex Descartes theorem."""
    ks = ka + kb + kc + 2*sqrt(ka*kb + kb*kc + kc*ka)

    # Complex positions
    za = complex(Oa[0], Oa[1])
    zb = complex(Ob[0], Ob[1])
    zc = complex(Oc[0], Oc[1])

    # w = ka*kb*za*zb + kb*kc*zb*zc + kc*ka*zc*za
    w = ka*kb*za*zb + kb*kc*zb*zc + kc*ka*zc*za

    # sqrt of complex number
    w_abs = abs(w)
    w_arg = atan2(w.imag, w.real)
    sq_w = sqrt(w_abs) * complex(cos(w_arg/2), sin(w_arg/2))

    zs = (ka*za + kb*zb + kc*zc + 2*sq_w) / ks
    return np.array([zs.real, zs.imag])

def compute():
    total_d = 0.0
    count = 0

    for ra in range(1, 101):
        for rb in range(ra+1, 101):
            for rc in range(rb+1, 101):
                if gcd3(ra, rb, rc) != 1:
                    continue
                count += 1

                # Circle centers
                Oa = np.array([0.0, 0.0])
                Ob = np.array([float(ra + rb), 0.0])

                d_ac = ra + rc
                d_ab = ra + rb
                d_bc = rb + rc
                Ocx = (d_ac**2 - d_bc**2 + d_ab**2) / (2.0 * d_ab)
                Ocy = sqrt(max(0, d_ac**2 - Ocx**2))
                Oc = np.array([Ocx, Ocy])

                # Tangency points
                Tab = (rb * Oa + ra * Ob) / (ra + rb)
                Tac = (rc * Oa + ra * Oc) / (ra + rc)
                Tbc = (rc * Ob + rb * Oc) / (rb + rc)

                # Circumcenter D
                D = circumcenter(Tab, Tac, Tbc)

                # Inner Soddy circle center E
                ka, kb, kc_val = 1.0/ra, 1.0/rb, 1.0/rc
                E = soddy_center(ka, kb, kc_val, Oa, Ob, Oc)

                dist = np.linalg.norm(D - E)
                total_d += dist

    expected = total_d / count
    print(f"Count of valid triples: {count}")
    print(f"E(d) = {expected:.8f}")
    return expected

result = compute()
