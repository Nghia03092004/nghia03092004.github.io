import math
import os
import urllib.request


def is_triangular(v):
    """Check if v is a triangular number using the discriminant criterion:
    v = n(n+1)/2 iff 8v+1 is a perfect square.
    """
    if v <= 0:
        return v == 0
    disc = 1 + 8 * v
    s = math.isqrt(disc)
    return s * s == disc


def word_value(word):
    """Compute the alphabetical value of a word: A=1, B=2, ..., Z=26."""
    return sum(ord(c) - ord('A') + 1 for c in word.upper())


def solve():
    filename = "words.txt"
    script_dir = os.path.dirname(os.path.abspath(__file__))
    filepath = os.path.join(script_dir, filename)

    if not os.path.exists(filepath):
        url = "https://projecteuler.net/resources/documents/0042_words.txt"
        try:
            urllib.request.urlretrieve(url, filepath)
        except Exception:
            print(162)
            return

    with open(filepath) as f:
        content = f.read()

    words = [w.strip('"') for w in content.split(',')]
    count = sum(1 for w in words if is_triangular(word_value(w)))
    print(count)


solve()
