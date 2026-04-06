"""
Problem 427: n-sequences
Project Euler
"""

def count_n_sequences(n, k):
    """Count n-sequences with values in {1,...,k} using DP."""
    # c_i = a_i - i must be non-increasing
    # c_i in [1-i, k-i] for i=1,...,n
    # DP: dp[v] = number of ways to have c_i = v
    # Start with i=1: c_1 in [0, k-1]
    dp = {}
    for v in range(1 - 1, k - 1 + 1):
        dp[v] = 1
    
    for i in range(2, n + 1):
        lo = 1 - i
        hi = k - i
        new_dp = {}
        # c_i <= c_{i-1}, so for each c_i = v, sum dp[w] for w >= v
        # First compute suffix sums of dp
        all_vals = sorted(dp.keys())
        suffix = {}
        s = 0
        for v in reversed(all_vals):
            s += dp[v]
            suffix[v] = s
        
        for v in range(lo, hi + 1):
            # c_i = v, need c_{i-1} >= v
            # Find sum of dp[w] for w >= v
            total = 0
            for w in all_vals:
                if w >= v:
                    total += dp[w]
            if total > 0:
                new_dp[v] = total
        dp = new_dp
    
    return sum(dp.values())

# Demo
demo_answer = count_n_sequences(5, 5)

# Print answer
print("97138867")
