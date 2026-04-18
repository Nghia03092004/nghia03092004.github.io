"""
Project Euler Problem 95: Amicable Chains

Find the smallest member of the longest amicable chain with no element
exceeding one million.

Phase 1: Sieve for sum of proper divisors s(n) in O(N log N).
Phase 2: Detect cycles via functional iteration in O(N) total.

Answer: 14316
"""


def solve(limit=1_000_000):
    # Phase 1: sieve for sum of proper divisors
    s = [0] * (limit + 1)
    for i in range(1, limit + 1):
        for j in range(2 * i, limit + 1, i):
            s[j] += i

    # Phase 2: detect longest cycle
    visited = [False] * (limit + 1)
    best_len = 0
    best_min = 0

    for start in range(2, limit + 1):
        if visited[start]:
            continue

        chain = []
        positions = {}
        n = start

        while True:
            if n < 1 or n > limit or visited[n]:
                break
            if n in positions:
                break
            positions[n] = len(chain)
            chain.append(n)
            n = s[n]

        # Check if we found a cycle
        if n in positions:
            cycle_start = positions[n]
            cycle = chain[cycle_start:]
            if len(cycle) > best_len:
                best_len = len(cycle)
                best_min = min(cycle)

        for x in chain:
            visited[x] = True

    return best_min


if __name__ == "__main__":
    answer = solve()
    assert answer == 14316
    print(answer)
