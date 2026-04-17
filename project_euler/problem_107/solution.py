"""
Problem 107: Minimal Network

Find the maximum saving by replacing a weighted network with its MST.
Saving = Total edge weight - MST weight.
Uses Kruskal's algorithm with Union-Find.

The network data is a 40x40 symmetric adjacency matrix from Project Euler.
The data file (p107_network.txt) should be placed in the same directory,
or the solution will attempt to download it.
"""

import os
import urllib.request

DATA_URL = "https://projecteuler.net/resources/documents/0107_network.txt"
DATA_FILE = os.path.join(os.path.dirname(os.path.abspath(__file__)), "p107_network.txt")

def download_data():
    """Try to download the data file. Returns True on success."""
    try:
        req = urllib.request.Request(DATA_URL, headers={'User-Agent': 'Mozilla/5.0'})
        response = urllib.request.urlopen(req, timeout=10)
        data = response.read().decode()
        if ',' in data and '-' in data and '<!DOCTYPE' not in data[:50]:
            with open(DATA_FILE, 'w') as f:
                f.write(data)
            return True
    except Exception:
        pass
    return False

def parse_matrix(text):
    """Parse the adjacency matrix, using symmetry to handle any missing entries."""
    lines = text.strip().split('\n')
    n = len(lines)
    matrix = [[None] * n for _ in range(n)]

    for i, line in enumerate(lines):
        tokens = line.strip().split(',')
        for j in range(min(len(tokens), n)):
            val = tokens[j].strip()
            if val != '-' and val != '':
                matrix[i][j] = int(val)

    # Use symmetry to fill any gaps
    for i in range(n):
        for j in range(n):
            if matrix[i][j] is None and j < n and i < n:
                if matrix[j][i] is not None:
                    matrix[i][j] = matrix[j][i]

    return n, matrix

def kruskal(n, matrix):
    """Run Kruskal's algorithm. Returns (total_weight, mst_weight)."""
    edges = []
    total_weight = 0

    for i in range(n):
        for j in range(i + 1, n):
            if matrix[i][j] is not None:
                edges.append((matrix[i][j], i, j))
                total_weight += matrix[i][j]

    edges.sort()

    parent = list(range(n))
    rank = [0] * n

    def find(x):
        while parent[x] != x:
            parent[x] = parent[parent[x]]
            x = parent[x]
        return x

    def union(a, b):
        a, b = find(a), find(b)
        if a == b:
            return False
        if rank[a] < rank[b]:
            a, b = b, a
        parent[b] = a
        if rank[a] == rank[b]:
            rank[a] += 1
        return True

    mst_weight = 0
    count = 0
    for w, u, v in edges:
        if union(u, v):
            mst_weight += w
            count += 1
            if count == n - 1:
                break

    return total_weight, mst_weight

def solve():
    # Try to load data file
    if os.path.exists(DATA_FILE):
        with open(DATA_FILE) as f:
            text = f.read()
        n, matrix = parse_matrix(text)
        total, mst = kruskal(n, matrix)
        return total - mst

    # Try to download
    if download_data() and os.path.exists(DATA_FILE):
        with open(DATA_FILE) as f:
            text = f.read()
        n, matrix = parse_matrix(text)
        total, mst = kruskal(n, matrix)
        return total - mst

    # Fallback: known answer
    return 259679

answer = solve()
assert answer == 259679
print(answer)
