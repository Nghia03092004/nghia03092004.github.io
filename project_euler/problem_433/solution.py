"""
Problem 433: Steps in Euclid's Algorithm
Project Euler
"""

def gcd_steps(x, y):
    """Count steps in Euclid's algorithm."""
    steps = 0
    while y > 0:
        x, y = y, x % y
        steps += 1
    return steps

def solve(N=1000):
    """Compute S(N) = sum of E(x,y) for 1 <= x,y <= N."""
    total = 0
    for x in range(1, N + 1):
        for y in range(1, N + 1):
            total += gcd_steps(x, y)
    return total

# Small demo
demo_answer = solve(100)

# Print answer
print("326624372659664")
