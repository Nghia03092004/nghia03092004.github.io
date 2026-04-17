import math

def solve():
    """
    Problem 246: Tangents to an Ellipse
    Ellipse: foci M=(-2000,1500), G=(8000,1500), sum of distances = 15000
    Center (3000,1500), a=7500, c=5000, b^2=31250000
    Count lattice points P outside ellipse where angle RPS > 45 degrees.
    Uses isoptic curve theory with exact integer arithmetic at boundaries.
    """
    A2 = 56250000     # a^2 = 7500^2
    B2 = 31250000     # b^2
    S_val = A2 + B2   # a^2 + b^2 = 87500000
    A2B2 = A2 * B2    # 1757812500000000
    cx, cy = 3000, 1500

    def is_valid(X, Y2):
        """Check if lattice point (cx+X, y) with Y^2=Y2 satisfies angle > 45."""
        X2 = X * X
        # Outside ellipse?
        ev = X2 * B2 + Y2 * A2
        if ev <= A2B2:
            return False
        # Inside director circle?
        r2 = X2 + Y2
        if r2 <= S_val:
            return True
        # Inside outer isoptic? f < 0?
        diff = r2 - S_val
        lhs = diff * diff
        rhs = 4 * (A2 * Y2 + B2 * X2 - A2B2)
        return lhs < rhs

    count = 0

    for y in range(-17500, 20501):
        Y = y - cy
        Y2 = Y * Y

        # Approximate isoptic boundary using float
        p_val = 2.0*Y2 - 2.0*A2 - 6.0*B2
        q_val = float(Y2)*Y2 - (6.0*A2+2.0*B2)*Y2 + float(A2)*A2 + 6.0*A2*B2 + float(B2)*B2
        disc = p_val*p_val - 4*q_val
        if disc < 0:
            continue
        iso_hi_sq = (-p_val + math.sqrt(disc)) / 2.0
        if iso_hi_sq <= 0:
            continue
        iso_hi = math.sqrt(iso_hi_sq)

        ell_rhs_num = A2 * (B2 - Y2)
        if ell_rhs_num <= 0:
            ell_lo = 0.0
        else:
            ell_lo = math.sqrt(ell_rhs_num / B2)

        if ell_lo >= iso_hi + 2:
            continue

        # Positive X side with exact boundary checks
        x_lo = math.ceil(cx + ell_lo)
        x_hi = math.floor(cx + iso_hi) + 1

        while x_lo <= x_hi and not is_valid(x_lo - cx, Y2):
            x_lo += 1
        while x_hi >= x_lo and not is_valid(x_hi - cx, Y2):
            x_hi -= 1
        while x_hi + 1 <= cx + int(iso_hi) + 3 and is_valid(x_hi + 1 - cx, Y2):
            x_hi += 1

        if x_lo <= x_hi:
            count += x_hi - x_lo + 1

        # Negative X side (use cx-1 as upper bound when ell_lo=0 to avoid
        # double-counting the x=cx column already covered by positive side)
        x_lo2 = math.ceil(cx - iso_hi) - 1
        x_hi2 = (cx - 1) if ell_lo < 0.5 else math.floor(cx - ell_lo)

        while x_lo2 <= x_hi2 and not is_valid(x_lo2 - cx, Y2):
            x_lo2 += 1
        while x_hi2 >= x_lo2 and not is_valid(x_hi2 - cx, Y2):
            x_hi2 -= 1
        while x_lo2 - 1 >= cx - int(iso_hi) - 3 and is_valid(x_lo2 - 1 - cx, Y2):
            x_lo2 -= 1

        if x_lo2 <= x_hi2:
            count += x_hi2 - x_lo2 + 1

    print(count)

solve()
