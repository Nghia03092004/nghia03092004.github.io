"""
Problem 103: Special Subset Sums: Optimum

Find the optimum special sum set of size 7, starting from the
near-optimum {20, 31, 38, 39, 40, 42, 45}.
"""
from itertools import product


def is_special_sum_set(a):
    a = sorted(a)
    n = len(a)
    # Property 2: sum of smallest k+1 > sum of largest k
    for k in range(1, n // 2 + 1):
        if sum(a[:k + 1]) <= sum(a[n - k:]):
            return False
    # Property 1: all subset sums distinct
    sums = set()
    for mask in range(1, 1 << n):
        s = sum(a[i] for i in range(n) if mask & (1 << i))
        if s in sums:
            return False
        sums.add(s)
    return True


def solve():
    base = [20, 31, 38, 39, 40, 42, 45]
    best_sum = sum(base)
    best_set = tuple(sorted(base))
    delta = 3

    for d in product(range(-delta, delta + 1), repeat=7):
        a = [base[i] + d[i] for i in range(7)]
        if any(x <= 0 for x in a):
            continue
        a_sorted = sorted(a)
        if len(set(a_sorted)) != 7:
            continue
        s = sum(a_sorted)
        if s > best_sum:
            continue
        if is_special_sum_set(a_sorted):
            if s < best_sum or (s == best_sum and tuple(a_sorted) < best_set):
                best_sum = s
                best_set = tuple(a_sorted)

    return "".join(str(x) for x in best_set)


if __name__ == "__main__":
    answer = solve()
    print(answer)
    assert answer == "20313839404245"
