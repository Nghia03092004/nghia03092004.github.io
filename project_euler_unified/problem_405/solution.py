"""
Project Euler Problem 405: A Rectangular Tiling

We tile a rectangle (length = 2 * width) by recursively substituting every tile:
  - Horizontal 2x1 -> 4x2 block (vert | horiz | horiz | vert)
  - Vertical   1x2 -> 2x4 block (horiz | vert | vert | horiz)

f(n) = number of interior points where exactly four tiles meet in T(n).

Recurrence: f(n) = 5*f(n-1) - 2*f(n-2) - 8*f(n-3) + 6
Closed form: f(n) = (6*4^n - 20*2^n - (-1)^n + 15) / 15

Answer: f(10^(10^18)) mod 17^7 = 237696125
"""

from collections import defaultdict

# ---------------------------------------------------------------------------
# Part 1: Tile substitution simulation
# ---------------------------------------------------------------------------

def replace_tile(x1, y1, x2, y2):
    """Replace a 2:1 tile at double scale with four tiles (alternating orientation)."""
    w, h = x2 - x1, y2 - y1
    nx1, ny1 = 2 * x1, 2 * y1
    nw, nh = 2 * w, 2 * h
    if w > h:  # horizontal tile
        return [
            (nx1, ny1, nx1 + h, ny1 + nh),            # left vertical
            (nx1 + h, ny1, nx1 + nw - h, ny1 + h),    # top-centre horizontal
            (nx1 + h, ny1 + h, nx1 + nw - h, ny1 + nh),  # bottom-centre horizontal
            (nx1 + nw - h, ny1, nx1 + nw, ny1 + nh),  # right vertical
        ]
    else:  # vertical tile
        return [
            (nx1, ny1, nx1 + nw, ny1 + w),            # top horizontal
            (nx1, ny1 + w, nx1 + w, ny1 + nh - w),    # left vertical
            (nx1 + w, ny1 + w, nx1 + nw, ny1 + nh - w),  # right vertical
            (nx1, ny1 + nh - w, nx1 + nw, ny1 + nh),  # bottom horizontal
        ]

def iterate(tiles):
    """Apply one substitution step to every tile."""
    result = []
    for t in tiles:
        result.extend(replace_tile(*t))
    return result

def count_four_corners(tiles):
    """Count interior points where exactly 4 tile corners meet."""
    corner_count = defaultdict(int)
    for x1, y1, x2, y2 in tiles:
        for pt in [(x1, y1), (x2, y1), (x1, y2), (x2, y2)]:
            corner_count[pt] += 1

    min_x = min(t[0] for t in tiles)
    max_x = max(t[2] for t in tiles)
    min_y = min(t[1] for t in tiles)
    max_y = max(t[3] for t in tiles)

    return sum(
        1 for (x, y), c in corner_count.items()
        if min_x < x < max_x and min_y < y < max_y and c == 4
    )

# ---------------------------------------------------------------------------
# Part 2: Recurrence / closed-form helpers
# ---------------------------------------------------------------------------

def f_recurrence(n):
    """Compute f(n) using the linear recurrence (exact integers)."""
    if n <= 0:
        return 0
    a, b, c = 0, 0, 0  # f(0), f(-1), f(-2) -- but we only need from f(1)
    vals = {0: 0, 1: 0, 2: 2, 3: 16}
    if n in vals:
        return vals[n]
    f1, f2, f3 = 16, 2, 0  # f(3), f(2), f(1)
    for _ in range(4, n + 1):
        fn = 5 * f1 - 2 * f2 - 8 * f3 + 6
        f3, f2, f1 = f2, f1, fn
    return f1

def f_closed(n):
    """Compute f(n) via the closed-form expression (exact integers)."""
    return (6 * 4**n - 20 * 2**n - (-1)**n + 15) // 15

# ---------------------------------------------------------------------------
# Part 3: Matrix exponentiation modulo m
# ---------------------------------------------------------------------------

def mat_mul(A, B, mod):
    """Multiply two 4x4 matrices modulo mod."""
    n = len(A)
    return [
        [sum(A[i][k] * B[k][j] for k in range(n)) % mod for j in range(n)]
        for i in range(n)
    ]

def mat_pow(M, p, mod):
    """Raise a 4x4 matrix to power p modulo mod via binary exponentiation."""
    n = len(M)
    result = [[int(i == j) for j in range(n)] for i in range(n)]
    base = [row[:] for row in M]
    while p > 0:
        if p & 1:
            result = mat_mul(result, base, mod)
        base = mat_mul(base, base, mod)
        p >>= 1
    return result

def f_mod_matrix(n, mod):
    """Compute f(n) mod m using 4x4 matrix exponentiation."""
    if n <= 1:
        return 0
    if n == 2:
        return 2 % mod
    if n == 3:
        return 16 % mod
    M = [[5, -2, -8, 6],
         [1,  0,  0, 0],
         [0,  1,  0, 0],
         [0,  0,  0, 1]]
    Mp = mat_pow(M, n - 3, mod)
    v = [16, 2, 0, 1]
    return sum(Mp[0][j] * v[j] for j in range(4)) % mod

def f_mod_closed(n_exp_reducer, mod):
    """
    Compute f(N) mod m where N = 10^(10^18) using the closed form.

    f(N) = (6*4^N - 20*2^N - (-1)^N + 15) / 15  mod m

    Parameters
    ----------
    n_exp_reducer : callable
        Function that, given (base, mod), returns base^N mod m
        using the tower of exponents with Euler's theorem.
    mod : int
        The modulus.
    """
    inv15 = pow(15, -1, mod)
    pow4N = n_exp_reducer(4, mod)
    pow2N = n_exp_reducer(2, mod)
    # N = 10^(10^18) is even, so (-1)^N = 1
    neg1N = 1
    return (6 * pow4N - 20 * pow2N - neg1N + 15) * inv15 % mod

# ---------------------------------------------------------------------------
# Part 4: Solve PE 405
# ---------------------------------------------------------------------------

def solve_pe405():
    """
    Compute f(10^(10^18)) mod 17^7.

    Uses the closed form with modular exponentiation, reducing exponents
    via Euler's theorem and CRT.
    """
    MOD = 17**7  # 410338673
    phi_mod = 16 * 17**6  # phi(17^7) = 386201104

    # We need base^N mod MOD where N = 10^(10^18).
    # By Euler's theorem: base^N mod MOD = base^(N mod phi(MOD)) mod MOD
    # provided gcd(base, MOD) = 1 (true for base=2,4 since MOD is a power of 17).
    #
    # N mod phi_mod, where phi_mod = 2^4 * 17^6:
    #   mod 16:   10^(10^18) = 0 mod 16  (since 10^k = 0 mod 16 for k >= 4)
    #   mod 17^6: 10^(10^18) mod 17^6
    #             = 10^(10^18 mod phi(17^6)) mod 17^6
    #             phi(17^6) = 16 * 17^5 = 22717712
    #             10^18 mod 22717712 is computed directly.
    #             Then pow(10, that, 17^6).

    mod_17_6 = 17**6
    phi_17_6 = 16 * 17**5

    exp_inner = pow(10, 18) % phi_17_6        # 10^18 mod phi(17^6)
    r_17_6 = pow(10, exp_inner, mod_17_6)     # 10^(10^18) mod 17^6

    # CRT: x = 0 mod 16,  x = r_17_6 mod 17^6
    inv16 = pow(16, -1, mod_17_6)
    t = (r_17_6 * inv16) % mod_17_6
    N_mod_phi = (16 * t) % phi_mod            # N mod phi(17^7)

    # Now compute f(N) via closed form
    pow2N = pow(2, N_mod_phi, MOD)
    twoN_mod_phi = (2 * N_mod_phi) % phi_mod
    pow4N = pow(2, twoN_mod_phi, MOD)         # 4^N = 2^(2N)
    neg1N = 1                                  # N is even

    inv15 = pow(15, -1, MOD)
    numerator = (6 * pow4N - 20 * pow2N - neg1N + 15) % MOD
    return (numerator * inv15) % MOD

# ---------------------------------------------------------------------------
# Part 5: Visualization
# ---------------------------------------------------------------------------

def draw_tiling(tiles, ax, title="", cmap_name="Set3"):
    """Draw a set of rectangular tiles on the given Axes."""
    cmap = plt.colormaps[cmap_name]
    min_x = min(t[0] for t in tiles)
    max_x = max(t[2] for t in tiles)
    min_y = min(t[1] for t in tiles)
    max_y = max(t[3] for t in tiles)

    for i, (x1, y1, x2, y2) in enumerate(tiles):
        color = cmap(i % 12 / 12.0)
        rect = patches.Rectangle(
            (x1, y1), x2 - x1, y2 - y1,
            linewidth=1.2, edgecolor="black", facecolor=color
        )
        ax.add_patch(rect)

    ax.set_xlim(min_x - 0.2, max_x + 0.2)
    ax.set_ylim(min_y - 0.2, max_y + 0.2)
    ax.set_aspect("equal")
    ax.invert_yaxis()
    ax.set_title(title, fontsize=11)
    ax.axis("off")

def create_visualization(save_path="visualization.png"):
    """Generate a multi-panel figure showing T(0)..T(4) and f(n) growth."""
