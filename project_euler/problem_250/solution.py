"""
Problem 250: 250250

Find non-empty subsets of {1^1, 2^2, ..., 250250^250250} with sum divisible by 250.
DP on residue classes mod 250. Answer mod 10^16.

Key: Only need i^i mod 250 for each element. DP array of size 250.
"""

def solve(N=250250, M=250, MOD=10**16):
    """DP on residues mod M. O(N * M) time."""
    dp = [0] * M
    dp[0] = 1
    for i in range(1, N + 1):
        ri = pow(i, i, M)
        ndp = dp.copy()
        for j in range(M):
            ndp[(j + ri) % M] = (ndp[(j + ri) % M] + dp[j]) % MOD
        dp = ndp
    return (dp[0] - 1) % MOD

def solve_brute(N=20, M=250):
    """Brute force for small N: enumerate all subsets."""
    residues = [pow(i, i, M) for i in range(1, N + 1)]
    count = 0
    for mask in range(1, 1 << N):
        s = sum(residues[i] for i in range(N) if mask & (1 << i))
        if s % M == 0:
            count += 1
    return count

# Cross-check
dp_small = solve(20, 250, 10**18)
bf_small = solve_brute(20, 250)
assert dp_small == bf_small, f"Mismatch: {dp_small} vs {bf_small}"

answer = solve()
assert answer == 1425480602091519, f"Expected 1425480602091519, got {answer}"
print(answer)
