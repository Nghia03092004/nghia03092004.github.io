from fractions import Fraction
from math import gcd
from functools import reduce

def solve():
    excluded = {
        79, 73, 71, 67, 61, 59, 53, 47, 43, 41, 37, 74, 31, 62, 29, 58,
        23, 46, 69, 19, 38, 57, 76, 34, 51, 26, 65, 78, 11, 22, 33, 44,
        55, 66, 77, 49, 25, 50, 75, 27, 54, 17, 68
    }
    candidates = sorted(k for k in range(2, 81) if k not in excluded)

    def lcm(a, b):
        return a * b // gcd(a, b)

    L = reduce(lcm, (k * k for k in candidates))
    target = L // 2
    vals = [L // (k * k) for k in candidates]
    n = len(candidates)

    suffix = [0] * (n + 1)
    for i in range(n - 1, -1, -1):
        suffix[i] = suffix[i + 1] + vals[i]

    count = 0

    def dfs(idx, current):
        nonlocal count
        if current == target:
            count += 1
            return
        if idx >= n:
            return
        need = target - current
        if need <= 0 or suffix[idx] < need:
            return
        for i in range(idx, n):
            if vals[i] > need:
                continue
            if suffix[i] < need:
                return
            dfs(i + 1, current + vals[i])

    dfs(0, 0)
    print(count)

solve()
