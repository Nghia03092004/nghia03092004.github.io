import math
from pathlib import Path


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


def load_words():
    script_dir = Path(__file__).resolve().parent
    candidates = [
        script_dir / "words.txt",
        script_dir / "0042_words.txt",
        Path("words.txt"),
        Path("0042_words.txt"),
        Path("project_euler/problem_042/words.txt"),
        Path("project_euler/problem_042/0042_words.txt"),
    ]

    for path in candidates:
        if path.exists():
            return path.read_text().strip()

    raise FileNotFoundError("Could not locate the Problem 42 word list.")


def solve():
    content = load_words()
    words = [w.strip('"') for w in content.split(',')]
    return sum(1 for w in words if is_triangular(word_value(w)))


answer = solve()
assert answer == 162
print(answer)
