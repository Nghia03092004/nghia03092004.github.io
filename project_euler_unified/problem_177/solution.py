"""
Problem 177: Integer Angled Quadrilaterals

Count non-similar convex quadrilaterals ABCD where both diagonals split
all four angles into integer-degree sub-angles.

8 sub-angles: alpha=BAC, beta=CAD, gamma=ABD, delta=DBC,
              epsilon=BCA, zeta=ACD, eta=CDB, theta=BDA

Constraints from diagonal intersection angle p:
  alpha+gamma = 180-p, delta+epsilon = p, zeta+eta = 180-p, theta+beta = p

Closure: sin(alpha)*sin(delta)*sin(zeta)*sin(theta) = sin(beta)*sin(gamma)*sin(epsilon)*sin(eta)

Symmetry group (dihedral-8) reduces count by factor ~8.
"""

import math

sinv = [0.0] * 181
for i in range(181):
    sinv[i] = math.sin(i * math.pi / 180.0)

# Permutations for dihedral-8 symmetry group
# r: rotation ABCD->BCDA
# s: reflection ABCD->ADCB
r_perm = [3, 2, 4, 5, 6, 7, 1, 0]
s_perm = [1, 0, 7, 6, 5, 4, 3, 2]

perms = [None] * 8
perms[0] = list(range(8))
perms[1] = r_perm[:]
for k in range(2, 4):
    perms[k] = [perms[k-1][r_perm[i]] for i in range(8)]
perms[4] = s_perm[:]
for k in range(5, 8):
    perms[k] = [perms[4][perms[k-4][i]] for i in range(8)]

def canonical(t):
    best = t
    for s in range(1, 8):
        tt = tuple(t[perms[s][i]] for i in range(8))
        if tt < best:
            best = tt
    return best

seen = set()

for p in range(2, 179):
    sp = sinv[p]
    cp = math.cos(p * math.pi / 180.0)
    max_az = 179 - p
    max_d = p - 1

    for alpha in range(1, max_az + 1):
        gamma = 180 - p - alpha
        for delta in range(1, max_d + 1):
            epsilon = p - delta
            for zeta in range(1, max_az + 1):
                eta = 180 - p - zeta

                L = sinv[alpha] * sinv[delta] * sinv[zeta]
                R = sinv[gamma] * sinv[epsilon] * sinv[eta]

                y = R * sp
                x = L + R * cp
                theta_exact = math.atan2(y, x) * 180.0 / math.pi

                if theta_exact < 0.5 or theta_exact > max_d + 0.5:
                    continue

                t_round = round(theta_exact)
                for theta in range(max(1, t_round - 1), min(max_d, t_round + 1) + 1):
                    beta = p - theta
                    if beta < 1:
                        continue

                    lhs = sinv[alpha] * sinv[delta] * sinv[zeta] * sinv[theta]
                    rhs = sinv[beta] * sinv[gamma] * sinv[epsilon] * sinv[eta]
                    mx = max(abs(lhs), abs(rhs))
                    if mx == 0:
                        continue
                    if abs(lhs - rhs) / mx > 1e-9:
                        continue

                    A = alpha + beta
                    B = gamma + delta
                    C = epsilon + zeta
                    D = eta + theta
                    if A >= 180 or B >= 180 or C >= 180 or D >= 180:
                        continue

                    t = (alpha, beta, gamma, delta, epsilon, zeta, eta, theta)
                    seen.add(canonical(t))

print(len(seen))
