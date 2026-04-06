"""
Problem 630: Crossed Lines

Generate points via pseudo-random sequence, form segments,
count proper crossings using orientation tests.

s_0 = 290797, s_{n+1} = s_n^2 mod 50515093.

Method 1: Brute force O(n^2) pairwise crossing check
Method 2: Sweep line (sketch)
"""

def generate_points(n_points, s0=290797, mod=50515093):
    """Generate points from the pseudo-random sequence."""
    s = s0
    points = []
    for _ in range(n_points):
        x = s
        s = s * s % mod
        y = s
        s = s * s % mod
        points.append((x, y))
    return points

def ccw(A, B, C):
    """Orientation test: positive if counterclockwise."""
    return (B[0] - A[0]) * (C[1] - A[1]) - (B[1] - A[1]) * (C[0] - A[0])

def segments_cross(A, B, C, D):
    """Check if segments AB and CD properly cross."""
    d1 = ccw(A, C, D)
    d2 = ccw(B, C, D)
    d3 = ccw(A, B, C)
    d4 = ccw(A, B, D)
    if ((d1 > 0 and d2 < 0) or (d1 < 0 and d2 > 0)) and \
       ((d3 > 0 and d4 < 0) or (d3 < 0 and d4 > 0)):
        return True
    return False

def count_crossings(segments):
    """Count crossings between all pairs of segments."""
    n = len(segments)
    count = 0
    for i in range(n):
        for j in range(i + 1, n):
            A, B = segments[i]
            C, D = segments[j]
            if segments_cross(A, B, C, D):
                count += 1
    return count

# Verify with small example
# Verify orientation test
assert ccw((0, 0), (1, 0), (0, 1)) > 0  # CCW
assert ccw((0, 0), (0, 1), (1, 0)) < 0  # CW

# Verify crossing
assert segments_cross((0, 0), (1, 1), (0, 1), (1, 0))  # X shape
assert not segments_cross((0, 0), (1, 0), (0, 1), (1, 1))  # parallel

# Generate points and segments
n_points = 20
points = generate_points(n_points)
segments = [(points[2*i], points[2*i+1]) for i in range(n_points // 2)]

crossings = count_crossings(segments)
print(f"Points: {n_points}, Segments: {len(segments)}, Crossings: {crossings}")
print("Verification passed.")
