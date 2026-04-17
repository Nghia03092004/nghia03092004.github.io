from functools import lru_cache

@lru_cache(maxsize=None)
def E(a2, a3, a4, a5):
    """Expected single-sheet encounters from state (a2,a3,a4,a5) onward,
    excluding the terminal pick at (0,0,0,1)."""
    if a2 == 0 and a3 == 0 and a4 == 0 and a5 == 0:
        return 0.0
    T = a2 + a3 + a4 + a5
    result = 0.0
    if T == 1 and not (a2 == 0 and a3 == 0 and a4 == 0 and a5 == 1):
        result = 1.0
    if a2 > 0:
        result += (a2 / T) * E(a2 - 1, a3 + 1, a4 + 1, a5 + 1)
    if a3 > 0:
        result += (a3 / T) * E(a2, a3 - 1, a4 + 1, a5 + 1)
    if a4 > 0:
        result += (a4 / T) * E(a2, a3, a4 - 1, a5 + 1)
    if a5 > 0:
        result += (a5 / T) * E(a2, a3, a4, a5 - 1)
    return result

print(f"{E(1, 1, 1, 1):.6f}")
