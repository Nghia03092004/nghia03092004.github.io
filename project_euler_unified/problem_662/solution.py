"""
Problem 662: Fibonacci Paths

Count lattice paths from (0,0) to (W,H) where each step has length
equal to a Fibonacci number. F(W,H) mod 10^9+7.

Steps: all (x,y) with x,y >= 0 and x^2 + y^2 = fib_k^2.
"""

MOD = 10**9 + 7

def fibonacci_up_to(limit):
    """Generate Fibonacci numbers up to limit."""
    fibs = [1, 2]
    while True:
        nxt = fibs[-1] + fibs[-2]
        if nxt > limit:
            break
        fibs.append(nxt)
    return fibs

def get_steps(max_coord):
    """Get all valid step vectors (dx, dy) with dx,dy >= 0."""
    fibs = fibonacci_up_to(int(max_coord * 1.5) + 1)
    steps = set()
    for f in fibs:
        f2 = f * f
        for x in range(f + 1):
            y2 = f2 - x * x
            y = int(y2 ** 0.5 + 0.5)
            if y * y == y2 and y >= 0:
                if x <= max_coord and y <= max_coord:
                    steps.add((x, y))
    # Remove (0,0)
    steps.discard((0, 0))
    return sorted(steps)

def solve_dp(W, H, mod=MOD):
    """Compute F(W, H) mod p via 2D DP."""
    steps = get_steps(max(W, H))

    # DP table, process row by row
    # f[x][y] = number of paths to (x, y)
    max_dy = max(dy for _, dy in steps) if steps else 0

    # Use rolling rows: keep last max_dy rows
    # Actually for memory, use full 2D array for small W, H
    dp = [[0] * (H + 1) for _ in range(W + 1)]
    dp[0][0] = 1

    for x in range(W + 1):
        for y in range(H + 1):
            if dp[x][y] == 0:
                continue
            val = dp[x][y]
            for dx, dy in steps:
                nx, ny = x + dx, y + dy
                if nx <= W and ny <= H:
                    dp[nx][ny] = (dp[nx][ny] + val) % mod

    return dp[W][H]

# --- Verify small cases ---
print("Computing F(3,4)...")
f34 = solve_dp(3, 4)
print(f"F(3,4) = {f34} (expected 278)")
assert f34 == 278, f"Mismatch: {f34}"

print("Computing F(10,10)...")
f1010 = solve_dp(10, 10)
print(f"F(10,10) = {f1010} (expected 215846462)")
assert f1010 == 215846462, f"Mismatch: {f1010}"

# F(10000, 10000) would require optimized implementation
print("F(10000,10000) requires optimized C++ implementation")

# Show steps
steps = get_steps(20)
print(f"Valid steps (up to coord 20): {steps}")
