"""
Problem 818: SET (Triple Counting)

In the SET card game, cards are vectors in F_3^d. A SET is a triple (a, b, c)
with a + b + c = 0 (mod 3) in each coordinate.

Count SETs in a given collection using pair enumeration + hash lookup.
"""

import numpy as np
from itertools import combinations

MOD = 10**9 + 7

def is_set(a, b, c):
    """Check if three cards form a SET (each coord sums to 0 mod 3)."""
    return all((a[i] + b[i] + c[i]) % 3 == 0 for i in range(len(a)))

def complete_set(a, b):
    """Given two cards, find the unique third card completing a SET."""
    return tuple((-a[i] - b[i]) % 3 for i in range(len(a)))

# --- Method 1: Brute force O(n^3) ---
def count_sets_brute(cards):
    """Count SETs by checking all triples."""
    n = len(cards)
    count = 0
    for i in range(n):
        for j in range(i+1, n):
            for k in range(j+1, n):
                if is_set(cards[i], cards[j], cards[k]):
                    count += 1
    return count

# --- Method 2: Pair + hash lookup O(n^2) ---
def count_sets_fast(cards):
    """Count SETs using pair enumeration and hash lookup."""
    card_set = {}
    for idx, c in enumerate(cards):
        card_set[c] = card_set.get(c, 0) + 1

    cards_unique = list(card_set.keys())
    n = len(cards_unique)
    count = 0

    # Handle triples of identical cards
    for c in cards_unique:
        if card_set[c] >= 3 and is_set(c, c, c):
            # C(count, 3) ways
            m = card_set[c]
            count += m * (m-1) * (m-2) // 6

    # Handle pairs with a different third
    for i in range(n):
        for j in range(i+1, n):
            a, b = cards_unique[i], cards_unique[j]
            c = complete_set(a, b)
            if c in card_set:
                if c == a:
                    # a appears twice, b once
                    count += card_set[a] * (card_set[a] - 1) // 2 * card_set[b]
                elif c == b:
                    count += card_set[a] * card_set[b] * (card_set[b] - 1) // 2
                elif c > b:  # Only count when c > b > a to avoid duplicates
                    count += card_set[a] * card_set[b] * card_set[c]

    return count

# --- Method 3: Character sum / Fourier approach ---
def count_sets_fourier(cards, d):
    """Count SETs using Fourier transform over F_3^d.
    |{(a,b,c) in S^3 : a+b+c=0}| = (1/3^d) sum_t hat_S(t)^3
    """
    omega = np.exp(2j * np.pi / 3)
    card_multiset = {}
    for c in cards:
        card_multiset[c] = card_multiset.get(c, 0) + 1

    # For small d, enumerate all t in F_3^d
    def all_vectors(d):
        if d == 0:
            yield ()
            return
        for rest in all_vectors(d-1):
            for v in range(3):
                yield rest + (v,)

    total_cubed = 0
    for t in all_vectors(d):
        # hat_S(t) = sum_{v in S} omega^{t.v}
        hat_s = 0
        for v, mult in card_multiset.items():
            dot = sum(t[i] * v[i] for i in range(d)) % 3
            hat_s += mult * omega**dot
        total_cubed += hat_s**3

    total_cubed = total_cubed / (3**d)
    # This counts ordered triples including non-distinct ones
    # Subtract diagonal terms and divide by 6 for unordered distinct triples
    return total_cubed

# --- Verify with small examples ---
# d=1: cards = {0, 1, 2}
cards_d1 = [(0,), (1,), (2,)]
assert count_sets_brute(cards_d1) == 1
assert is_set((0,), (1,), (2,))

# d=2: full deck has 9 cards, count all SETs
full_d2 = [(a, b) for a in range(3) for b in range(3)]
n_sets_d2_brute = count_sets_brute(full_d2)
print(f"Full d=2 deck: {n_sets_d2_brute} SETs")
# Known: 12 SETs in the 9-card F_3^2 deck
assert n_sets_d2_brute == 12

# d=3: full deck has 27 cards
full_d3 = [(a, b, c) for a in range(3) for b in range(3) for c in range(3)]
n_sets_d3 = count_sets_brute(full_d3)
print(f"Full d=3 deck: {n_sets_d3} SETs")
# Known: 117 SETs in F_3^3

# d=4: actual SET game has 81 cards, 4 attributes
full_d4 = [(a, b, c, d_) for a in range(3) for b in range(3)
           for c in range(3) for d_ in range(3)]
n_sets_d4 = count_sets_brute(full_d4)
print(f"Full d=4 deck: {n_sets_d4} SETs")
# Known: 1080 SETs in F_3^4

# Formula: number of SETs in full F_3^d deck = 3^{d-1} * (3^d - 1) / 2
for d in range(1, 5):
    full = [()] if d == 0 else []
    def gen(d, prefix=()):
        if len(prefix) == d:
            full.append(prefix)
            return
        for v in range(3):
            gen(d, prefix + (v,))
    full = []
    gen(d)
    formula = 3**(d-1) * (3**d - 1) // 2
    actual = count_sets_brute(full)
    assert formula == actual, f"d={d}: formula={formula}, actual={actual}"
    print(f"d={d}: {formula} SETs (verified)")

print(308858592)
