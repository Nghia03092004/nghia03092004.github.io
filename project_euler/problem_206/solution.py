"""
Problem 206: Concealed Square
Find the unique positive integer whose square has the form 1_2_3_4_5_6_7_8_9_0.
"""

import math

def check(n):
    """Check if n^2 matches the pattern 1_2_3_4_5_6_7_8_9_0."""
    sq = n * n
    s = str(sq)
    if len(s) != 19:
        return False
    pattern = "1_2_3_4_5_6_7_8_9_0"
    for i, c in enumerate(pattern):
        if c != '_' and s[i] != c:
            return False
    return True

def solve():
    lo = math.isqrt(1020304050607080900)
    hi = math.isqrt(1929394959697989990)

    # n must end in 30 or 70
    # Start from hi and work down (answer is near upper bound)
    # Or iterate up from lo
    for n in range(lo, hi + 1):
        r = n % 100
        if r != 30 and r != 70:
            continue
        if check(n):
            return n

    return None

# More efficient: step by 40/60 alternating from a starting point ending in 30
def solve_fast():
    lo = math.isqrt(1020304050607080900)
    hi = math.isqrt(1929394959697989990)

    # Align lo to end in 30
    r = lo % 100
    if r <= 30:
        start = lo - r + 30
    elif r <= 70:
        start = lo - r + 70
    else:
        start = lo - r + 130

    n = start
    while n <= hi:
        if check(n):
            return n
        # Alternate between 30 and 70 endings
        if n % 100 == 30:
            n += 40  # jump to next XX70
        else:
            n += 60  # jump to next XX30 (next hundred)

    return None

answer = solve_fast()
print(answer)
