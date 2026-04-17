"""
Problem 54: Poker Hands

How many hands does Player 1 win in the poker game?

Usage:
    Place 'poker.txt' (or 'p054_poker.txt') in the same directory, then run:
        python solution.py
    Or pipe via stdin:
        python solution.py < poker.txt

    Download poker.txt from: https://projecteuler.net/problem=54
    (requires a free Project Euler account)
"""
import sys
import os
from collections import Counter


def card_value(c):
    values = {'2': 2, '3': 3, '4': 4, '5': 5, '6': 6, '7': 7, '8': 8,
              '9': 9, 'T': 10, 'J': 11, 'Q': 12, 'K': 13, 'A': 14}
    return values[c]


def evaluate(cards):
    """
    Compute the comparison key kappa(H) for a 5-card poker hand.
    Returns a tuple for lexicographic comparison.

    Categories: 0=High Card, 1=One Pair, 2=Two Pairs, 3=Three of a Kind,
    4=Straight, 5=Flush, 6=Full House, 7=Four of a Kind,
    8=Straight Flush, 9=Royal Flush
    """
    values = sorted([card_value(c[0]) for c in cards], reverse=True)
    suits = [c[1] for c in cards]

    is_flush = len(set(suits)) == 1

    unique_vals = sorted(set(values))
    is_straight = False
    straight_high = 0

    if len(unique_vals) == 5:
        if unique_vals[-1] - unique_vals[0] == 4:
            is_straight = True
            straight_high = unique_vals[-1]
        if unique_vals == [2, 3, 4, 5, 14]:
            is_straight = True
            straight_high = 5

    freq = Counter(values)
    groups = sorted(freq.items(), key=lambda x: (-x[1], -x[0]))
    counts = [f for v, f in groups]
    tiebreakers = tuple(v for v, f in groups)

    if is_straight and is_flush:
        rank = 9 if straight_high == 14 else 8
        return (rank, straight_high)
    elif counts[0] == 4:
        return (7,) + tiebreakers
    elif counts[0] == 3 and len(counts) > 1 and counts[1] == 2:
        return (6,) + tiebreakers
    elif is_flush:
        return (5,) + tiebreakers
    elif is_straight:
        return (4, straight_high)
    elif counts[0] == 3:
        return (3,) + tiebreakers
    elif counts[0] == 2 and len(counts) > 1 and counts[1] == 2:
        return (2,) + tiebreakers
    elif counts[0] == 2:
        return (1,) + tiebreakers
    else:
        return (0,) + tiebreakers


def solve():
    lines = None

    script_dir = os.path.dirname(os.path.abspath(__file__))
    for filename in ['poker.txt', 'p054_poker.txt', '0054_poker.txt']:
        filepath = os.path.join(script_dir, filename)
        if os.path.exists(filepath):
            with open(filepath) as f:
                lines = f.readlines()
            break

    if lines is None:
        if not sys.stdin.isatty():
            lines = sys.stdin.readlines()
        else:
            print("Error: poker.txt not found.")
            print("Download from https://projecteuler.net/problem=54")
            print("Or pipe via stdin: python solution.py < poker.txt")
            return

    p1_wins = 0
    for line in lines:
        line = line.strip()
        if not line:
            continue
        cards = line.split()
        if len(cards) < 10:
            continue
        hand1 = cards[:5]
        hand2 = cards[5:10]
        if evaluate(hand1) > evaluate(hand2):
            p1_wins += 1

    print(p1_wins)

solve()
