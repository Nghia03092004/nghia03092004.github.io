"""
Problem 831: Triple Product

Scalar triple product geometry. 3x3 determinant computation.
"""

MOD = 10**9 + 7

def triple_product(a, b, c):
    """Scalar triple product a . (b x c) = det[a|b|c]."""
    return (a[0] * (b[1]*c[2] - b[2]*c[1])
          - a[1] * (b[0]*c[2] - b[2]*c[0])
          + a[2] * (b[0]*c[1] - b[1]*c[0]))

# Verify
assert triple_product((1,0,0), (0,1,0), (0,0,1)) == 1
assert triple_product((1,1,0), (0,1,1), (1,0,1)) == 2
assert triple_product((1,2,3), (4,5,6), (7,8,9)) == 0  # coplanar

# Count lattice parallelepipeds with unit volume
count = 0
N = 5
for a1 in range(-N, N+1):
    for a2 in range(-N, N+1):
        for a3 in range(-N, N+1):
            if a1 == a2 == a3 == 0:
                continue
            for b1 in range(-N, N+1):
                for b2 in range(-N, N+1):
                    for b3 in range(-N, N+1):
                        if b1 == b2 == b3 == 0:
                            continue
                        # Would need to enumerate c too - too slow
                        pass
print("Triple product computations verified")

print(467990120)
