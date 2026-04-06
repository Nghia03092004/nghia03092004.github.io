"""
Problem 310: Nim Square

Three-pile Nim variant where valid moves are removing k^2 stones (k >= 1).
Count losing positions (a, b, c) with 0 <= a <= b <= c <= 100000.

By Sprague-Grundy: position is losing iff G(a) XOR G(b) XOR G(c) = 0.

Answer: 2586528661783
"""

def solve():
    N = 100000

    # Compute Grundy values
    G = [0] * (N + 1)
    for n in range(1, N + 1):
        seen = set()
        k = 1
        while k * k <= n:
            seen.add(G[n - k * k])
            k += 1
        mex = 0
        while mex in seen:
            mex += 1
        G[n] = mex

    max_g = max(G)

    # XOR max
    xor_max = 1
    while xor_max <= max_g:
        xor_max <<= 1
    xor_max -= 1

    # Count losing positions with a <= b <= c
    freq = [0] * (max_g + 1)
    pair_count = [0] * (xor_max + 1)
    answer = 0

    for c in range(N + 1):
        gc = G[c]
        for g in range(max_g + 1):
            if freq[g] > 0:
                pair_count[g ^ gc] += freq[g]
        pair_count[0] += 1
        freq[gc] += 1
        answer += pair_count[gc]

    print(answer)
