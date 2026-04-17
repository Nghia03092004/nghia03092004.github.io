"""
Problem 522: Hilbert's Blackout
Grid blackout puzzle on Hilbert curve grids.
"""

def hilbert_d2xy(n: int, d: int):
    """Convert Hilbert curve index d to (x, y) coordinates on 2^n x 2^n grid."""
    x = y = 0
    s = 1
    while s < (1 << n):
        rx = 1 if (d & 2) else 0
        ry = 1 if ((d & 1) ^ rx) else 0
        # Rotate
        if ry == 0:
            if rx == 1:
                x = s - 1 - x
                y = s - 1 - y
            x, y = y, x
        x += s * rx
        y += s * ry
        d >>= 2
        s <<= 1
    return x, y

def hilbert_xy2d(n: int, x: int, y: int):
    """Convert (x, y) coordinates to Hilbert curve index on 2^n x 2^n grid."""
    d = 0
    s = (1 << n) >> 1
    while s > 0:
        rx = 1 if (x & s) > 0 else 0
        ry = 1 if (y & s) > 0 else 0
        d += s * s * ((3 * rx) ^ ry)
        # Rotate
        if ry == 0:
            if rx == 1:
                x = s - 1 - x
                y = s - 1 - y
            x, y = y, x
        s >>= 1
    return d

def generate_hilbert_curve(order: int):
    """Generate all points along a Hilbert curve of given order."""
    n_points = 4 ** order
    points = [hilbert_d2xy(order, d) for d in range(n_points)]
    return points

def blackout_count(order: int, k: int):
    """
    Count lights remaining after blacking out every k-th position
    along the Hilbert curve of given order.
    """
    total = 4 ** order
    blacked = total // k
    return total - blacked

# Compute blackout counts
for order in range(1, 8):
    total = 4 ** order
    for k in [2, 3, 5]:
        remaining = blackout_count(order, k)
        print(f"Order {order} ({total} cells), blackout every {k}: {remaining} remaining")
