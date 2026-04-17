"""
Problem 79: Passcode Derivation

Given 50 successful login attempts (3-digit subsequences), determine the
shortest possible secret passcode via topological sort of the precedence DAG.
Answer: 73162890
"""

from collections import defaultdict, deque


def main():
    attempts = [
        "319", "680", "180", "690", "129", "620", "762", "689", "762", "318",
        "368", "710", "720", "710", "629", "168", "160", "689", "716", "731",
        "736", "729", "316", "729", "729", "710", "769", "290", "719", "680",
        "318", "389", "162", "289", "162", "718", "729", "319", "790", "680",
        "890", "362", "319", "760", "316", "729", "380", "319", "728", "716",
    ]

    # Build the precedence DAG
    digits = set()
    edges = set()
    for s in attempts:
        a, b, c = int(s[0]), int(s[1]), int(s[2])
        digits.update([a, b, c])
        edges.add((a, b))
        edges.add((a, c))
        edges.add((b, c))

    adj = defaultdict(set)
    indeg = {d: 0 for d in digits}
    for u, v in edges:
        adj[u].add(v)
    for u in digits:
        for v in adj[u]:
            indeg[v] += 1

    # Kahn's algorithm (topological sort)
    queue = deque(d for d in sorted(digits) if indeg[d] == 0)
    result = []
    while queue:
        u = queue.popleft()
        result.append(u)
        for v in sorted(adj[u]):
            indeg[v] -= 1
            if indeg[v] == 0:
                queue.append(v)

    print("".join(str(d) for d in result))


if __name__ == "__main__":
    main()
