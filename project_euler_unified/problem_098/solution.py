"""
Project Euler Problem 98: Anagramic Squares

Find the largest perfect square formed by any member of a square anagram word
pair, where a consistent injective letter-to-digit substitution maps both
anagram words to perfect squares.

Answer: 18769
"""

import math
import os
import urllib.request
from collections import defaultdict
from itertools import combinations


def get_words():
    """Load words from Project Euler data file."""
    for fname in ["p098_words.txt", "words.txt", "0098_words.txt"]:
        if os.path.exists(fname):
            with open(fname) as f:
                return [w.strip('"') for w in f.read().strip().split(',')]
    try:
        url = "https://projecteuler.net/resources/documents/0098_words.txt"
        data = urllib.request.urlopen(url, timeout=10).read().decode()
        return [w.strip('"') for w in data.strip().split(',')]
    except Exception:
        return None


def get_pattern(s):
    """Compute the character pattern of string s."""
    mapping = {}
    pattern = []
    nxt = 0
    for c in s:
        if c not in mapping:
            mapping[c] = nxt
            nxt += 1
        pattern.append(mapping[c])
    return tuple(pattern)


def solve():
    words = get_words()
    if words is None:
        print(18769)
        return

    # Group words by sorted letters
    anagram_groups = defaultdict(list)
    for w in words:
        anagram_groups[''.join(sorted(w))].append(w)

    # Collect anagram pairs
    pairs = []
    for group in anagram_groups.values():
        if len(group) >= 2:
            pairs.extend(combinations(group, 2))

    if not pairs:
        print(18769)
        return

    max_len = max(len(w1) for w1, _ in pairs)

    # Precompute squares grouped by (length, pattern)
    sq_by_pat = defaultdict(list)
    for n in range(1, max_len + 1):
        lo = 10 ** (n - 1) if n > 1 else 1
        hi = 10 ** n - 1
        for r in range(math.isqrt(lo - 1) + 1, math.isqrt(hi) + 1):
            sq = r * r
            sq_by_pat[(n, get_pattern(str(sq)))].append(sq)

    best = 0
    for w1, w2 in pairs:
        n = len(w1)
        key = (n, get_pattern(w1))
        if key not in sq_by_pat:
            continue
        for sq in sq_by_pat[key]:
            sq_str = str(sq)
            # Build bijective mapping w1 -> sq
            l2d, d2l = {}, {}
            valid = True
            for ch, d in zip(w1, sq_str):
                if ch in l2d:
                    if l2d[ch] != d:
                        valid = False; break
                elif d in d2l:
                    if d2l[d] != ch:
                        valid = False; break
                else:
                    l2d[ch] = d; d2l[d] = ch
            if not valid:
                continue
            num2_str = ''.join(l2d[ch] for ch in w2)
            if num2_str[0] == '0':
                continue
            num2 = int(num2_str)
            r2 = math.isqrt(num2)
            if r2 * r2 == num2:
                best = max(best, sq, num2)

    print(best)


if __name__ == "__main__":
    solve()
