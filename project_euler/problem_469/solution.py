"""
Project Euler Problem 469: Empty Chairs

N chairs around a circle. Knights sit one at a time in random available chairs
(both neighbors must be empty). E(N) = expected fraction of empty chairs when
no more can sit.

Find E(10^18) to 14 decimal places.

Key result: E(N) -> (1 + e^(-2)) / 2 as N -> infinity.
"""

import math
import random
from fractions import Fraction
from decimal import Decimal, getcontext

getcontext().prec = 50

def simulate_circle(N, trials=10000):
    """Monte Carlo simulation of the empty chairs problem."""
    total_empty_frac = 0.0
    for _ in range(trials):
        occupied = [False] * N
        available = list(range(N))

        while available:
            idx = random.choice(available)
            occupied[idx] = True
            # Remove this chair and its neighbors from available
            new_available = []
            for c in available:
                if c == idx:
                    continue
                # Check if both neighbors of c are unoccupied
                left = (c - 1) % N
                right = (c + 1) % N
                # After placing at idx, check if c is still available
                occ_copy = occupied[:]  # already updated
                if not occ_copy[(c - 1) % N] and not occ_copy[(c + 1) % N] and not occ_copy[c]:
                    new_available.append(c)
            available = new_available

        empty_count = sum(1 for x in occupied if not x)
        total_empty_frac += empty_count / N

    return total_empty_frac / trials

def simulate_circle_fast(N, trials=50000):
    """Faster Monte Carlo simulation."""
    total_frac = 0.0
    for _ in range(trials):
        occupied = set()
        blocked = set()  # chairs adjacent to an occupied one

        # Initially all chairs available
        chairs = set(range(N))

        while True:
            available = chairs - occupied - blocked
            # Further filter: both neighbors must be empty (not occupied)
            truly_available = []
            for c in available:
                left = (c - 1) % N
                right = (c + 1) % N
                if left not in occupied and right not in occupied:
                    truly_available.append(c)
            if not truly_available:
                break
            choice = random.choice(truly_available)
            occupied.add(choice)
            # Mark neighbors as blocked (they can't be chosen since neighbor is occupied)
            blocked.add((choice - 1) % N)
            blocked.add((choice + 1) % N)

        empty_count = N - len(occupied)
        total_frac += empty_count / N

    return total_frac / trials

def exact_small(N):
    """Exact computation for small N using exhaustive enumeration."""
    from itertools import permutations

    def simulate_sequence(N, order):
        """Given a sequence of chair preferences, simulate the process."""
        occupied = [False] * N

        for chair in order:
            if occupied[chair]:
                continue
            left = (chair - 1) % N
            right = (chair + 1) % N
            if occupied[left] or occupied[right]:
                continue
            occupied[chair] = True

        return sum(1 for x in occupied if not x)

    # Actually, the process is: at each step, pick uniformly from AVAILABLE chairs
    # This is not the same as a fixed permutation
    # Need to do proper tree enumeration

    # Use recursive computation
    from functools import lru_cache

    # State: tuple of chair states (0=empty, 1=occupied)
    # Too expensive for large N, but fine for N <= 10

    def compute_expected(N):
        """Compute E(N) exactly using probability tree."""
        # State: frozenset of occupied positions
        # For each state, find available positions and average over choices

        from collections import defaultdict

        # BFS over states with probabilities
        # State: tuple (occupied) -- use bitmask for small N
        initial = 0  # no one seated
        # prob[state] = probability of reaching this state
        prob = {initial: Fraction(1)}

        while True:
            new_prob = defaultdict(Fraction)
            any_moved = False

            for state, p in prob.items():
                # Find available chairs
                available = []
                for c in range(N):
                    if state & (1 << c):
                        continue  # already occupied
                    left = (c - 1) % N
                    right = (c + 1) % N
                    if (state & (1 << left)) or (state & (1 << right)):
                        continue  # neighbor occupied
                    available.append(c)

                if not available:
                    new_prob[state] += p  # terminal state
                else:
                    any_moved = True
                    prob_each = Fraction(1, len(available))
                    for c in available:
                        new_state = state | (1 << c)
                        new_prob[new_state] += p * prob_each

            prob = dict(new_prob)
            if not any_moved:
                break

        # Compute expected empty fraction
        expected = Fraction(0)
        for state, p in prob.items():
            occupied_count = bin(state).count('1')
            empty_frac = Fraction(N - occupied_count, N)
            expected += p * empty_frac

        return expected

    return compute_expected(N)

def high_precision_answer():
    """Compute the answer to high precision."""
    getcontext().prec = 50
    e_neg2 = Decimal(1) / Decimal(math.e ** 2)
    # More precise: use mpmath or series
    # e^(-2) via Taylor series
    e_neg2 = Decimal(0)
    term = Decimal(1)
    for k in range(100):
        e_neg2 += term
        term *= Decimal(-2) / Decimal(k + 1)

    result = (Decimal(1) + e_neg2) / Decimal(2)
    return result

def create_visualization():
    """Create visualization."""
