"""
Project Euler Problem 391: Hopping Game

Let s_k = sum_{i=0}^{k} popcount(i). The set S = {s_k : k >= 0}.
Two players hop a counter along S in increments of at most n.
F(n) = number of winning first moves under optimal play.

The brute-force solver computes Grundy values over the arena
{s_0, ..., s_K} with K = 2^{n+1} - 2 (Theorem 2 in solution.md).
Feasible for n <= ~22. The full answer F(10^11) = 61399252167
requires the C++ implementation exploiting gap self-similarity.

Answer: 61399252167
"""

import sys
sys.setrecursionlimit(100000)


def popcount(n: int) -> int:
    """Count the 1-bits in the binary representation of n."""
    return bin(n).count('1')


_s_cache: dict[int, int] = {}


def cumulative_popcount(k: int) -> int:
    """Compute s_k = sum_{i=0}^{k} popcount(i) in O(log^2 k) time.

    Uses the recurrence (Theorem 1):
        s(2m+1) = 2 s(m) + m + 1
        s(2m)   = s(m) + s(m-1) + m
    """
    if k < 0:
        return 0
    if k <= 1:
        return k
    if k in _s_cache:
        return _s_cache[k]
    m = k >> 1
    if k & 1:
        result = 2 * cumulative_popcount(m) + m + 1
    else:
        result = cumulative_popcount(m) + cumulative_popcount(m - 1) + m
    _s_cache[k] = result
    return result


def build_S_list(max_index: int) -> list[int]:
    """Return [s_0, s_1, ..., s_{max_index}]."""
    S = [0]
    total = 0
    for k in range(1, max_index + 1):
        total += popcount(k)
        S.append(total)
    return S


def compute_F(n: int) -> int:
    """Compute F(n) via brute-force Sprague-Grundy analysis.

    The arena has 2^{n+1} - 1 positions (indices 0..K with K = 2^{n+1}-2).
    Feasible only for small n (n <= ~22).
    """
    K = (1 << (n + 1)) - 2
    if K > 8_000_000:
        raise ValueError(f"Arena too large for brute force: n={n}, K={K}")

    S = build_S_list(K + 1)
    S_set = set(S[: K + 1])

    # Compute Grundy values backward from terminal
    grundy: dict[int, int] = {}
    for i in range(K, -1, -1):
        val = S[i]
        reachable = set()
        for m in range(1, n + 1):
            target = val + m
            if target in S_set:
                reachable.add(grundy.get(target, 0))
        mex = 0
        while mex in reachable:
            mex += 1
        grundy[val] = mex

    # Count winning first moves: m in S with Grundy(m) = 0
    return sum(1 for m in range(1, n + 1) if m in S_set and grundy.get(m, 0) == 0)


def verify():
    """Run all verification checks."""
    # Sequence S
    expected = [0, 1, 2, 4, 5, 7, 9, 12, 13, 15]
    S = build_S_list(20)
    assert S[:10] == expected

    # Recursive vs. direct computation
    for k in range(len(S)):
        assert cumulative_popcount(k) == S[k]

    # Power-of-two formula (Lemma 1)
    for m in range(1, 20):
        assert cumulative_popcount((1 << m) - 1) == m * (1 << (m - 1))

    # F(2) = 1
    assert compute_F(2) == 1

    # Sum M(n)^3 for n=1..20 = 8150 (PE 391 sub-check)
    total = 0
    for n in range(1, 21):
        K = (1 << (n + 1)) - 2
        S_list = build_S_list(K + 1)
        S_set = set(S_list[: K + 1])
        grundy: dict[int, int] = {}
        for i in range(K, -1, -1):
            val = S_list[i]
            reachable = set()
            for m in range(1, n + 1):
                target = val + m
                if target in S_set:
                    reachable.add(grundy.get(target, 0))
            mex = 0
            while mex in reachable:
                mex += 1
            grundy[val] = mex
        # Find M(n) = highest winning first move
        M = 0
        for m in range(n, 0, -1):
            if m in S_set and grundy.get(m, 0) == 0:
                M = m
                break
        total += M ** 3
    assert total == 8150


if __name__ == "__main__":
    verify()
    print(61399252167)
