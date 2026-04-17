"""
Problem 122: Efficient Exponentiation

Find sum of shortest addition chain lengths m(n) for n = 1 to 200.
Uses iterative deepening DFS with doubling-based pruning.
"""

import math

def shortest_addition_chain(n):
    if n == 1:
        return 0

    def dfs(chain, depth, max_depth):
        cur = chain[-1]
        if cur == n:
            return True
        if depth == max_depth:
            return False
        if cur << (max_depth - depth) < n:
            return False
        for i in range(depth, -1, -1):
            nxt = cur + chain[i]
            if nxt <= cur or nxt > n:
                continue
            chain.append(nxt)
            if dfs(chain, depth + 1, max_depth):
                return True
            chain.pop()
        return False

    lb = max(1, math.ceil(math.log2(n)))
    for max_depth in range(lb, 20):
        if dfs([1], 0, max_depth):
            return max_depth

def solve():
    print(sum(shortest_addition_chain(n) for n in range(1, 201)))

solve()
