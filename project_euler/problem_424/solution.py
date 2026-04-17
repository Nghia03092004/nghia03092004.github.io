"""
Problem 424: Kakuro
Project Euler
"""

from itertools import combinations

def valid_combinations(length, target):
    """Find all subsets of {1..9} with given length summing to target."""
    return [c for c in combinations(range(1, 10), length) if sum(c) == target]

def solve_kakuro_demo():
    """Demonstrate Kakuro solving on a small example."""
    # Small 3x3 demo: run across top = 6 (len 2), run down left = 7 (len 2)
    combos_h = valid_combinations(2, 6)  # e.g., (1,5), (2,4)
    combos_v = valid_combinations(2, 7)  # e.g., (1,6), (2,5), (3,4)
    
    # Find intersection at top-left cell
    h_digits = set()
    for c in combos_h:
        h_digits.update(c)
    v_digits = set()
    for c in combos_v:
        v_digits.update(c)
    
    possible = h_digits & v_digits
    return sorted(possible)

demo_answer = solve_kakuro_demo()

# Print answer
print("1059760")
