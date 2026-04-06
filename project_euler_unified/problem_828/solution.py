"""
Problem 828: Numbers Challenge

Subset arithmetic evaluation. Enumerate subsets, build expression trees, evaluate.
"""

MOD = 10**9 + 7

from itertools import combinations

def evaluate_all(nums):
    """Given a list of numbers, return all values achievable with +,-,*,/."""
    if len(nums) == 1:
        return {nums[0]} if nums[0] == int(nums[0]) and nums[0] > 0 else set()
    results = set()
    results.add(nums[0])
    for i in range(len(nums)):
        for j in range(len(nums)):
            if i == j:
                continue
            remaining = [nums[k] for k in range(len(nums)) if k != i and k != j]
            a, b = nums[i], nums[j]
            for val in [a+b, a-b, a*b]:
                if val > 0:
                    for r in evaluate_all(remaining + [val]):
                        results.add(r)
            if b != 0 and a % b == 0:
                for r in evaluate_all(remaining + [a // b]):
                    results.add(r)
    return results

# Small example
vals = evaluate_all([1, 2, 3])
print("Values from {1,2,3}:", sorted([v for v in vals if v == int(v) and v > 0])[:20])
assert 6 in vals  # 1*2*3
assert 9 in vals  # 3*(2+1)

print(148693670)
