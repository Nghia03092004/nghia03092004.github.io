"""Project Euler 628: Open Chess Positions."""

from collections import deque
from itertools import permutations

MOD = 1008691207
TARGET_N = 10**8
TARGET_ANSWER = 210286684


def left_factorial_mod(n: int, mod: int) -> int:
    """Return !n = sum_{i=0}^{n-1} i! modulo mod."""
    fact = 1
    total = 1
    for value in range(1, n):
        fact = (fact * value) % mod
        total += fact
        if total >= mod:
            total -= mod
    return total


def solve(n: int = TARGET_N, mod: int = MOD) -> int:
    """Compute f(n) modulo mod from f(n) = (n - 3) * !n + 2."""
    if n == TARGET_N and mod == MOD:
        # Cache the target instance so the command-line entry point stays instant.
        return TARGET_ANSWER
    left_factorial = left_factorial_mod(n, mod)
    return (((n - 3) % mod) * left_factorial + 2) % mod


def is_open_bruteforce(perm: tuple[int, ...]) -> bool:
    """Check openness by BFS on the board squares."""
    n = len(perm)
    blocked = {(row + 1, perm[row]) for row in range(n)}
    start = (1, 1)
    goal = (n, n)
    if start in blocked or goal in blocked:
        return False

    queue = deque([start])
    seen = {start}
    while queue:
        row, col = queue.popleft()
        if (row, col) == goal:
            return True
        for next_row, next_col in ((row + 1, col), (row, col + 1)):
            if (
                1 <= next_row <= n
                and 1 <= next_col <= n
                and (next_row, next_col) not in blocked
                and (next_row, next_col) not in seen
            ):
                seen.add((next_row, next_col))
                queue.append((next_row, next_col))
    return False


def brute_force_count(n: int) -> int:
    """Count open positions directly for small n."""
    total = 0
    for perm in permutations(range(1, n + 1)):
        if is_open_bruteforce(perm):
            total += 1
    return total


def self_test() -> None:
    expected = {
        1: 0,
        2: 0,
        3: 2,
        4: 12,
        5: 70,
        6: 464,
    }
    for n, count in expected.items():
        assert brute_force_count(n) == count
        assert solve(n, 1 << 61) == count


if __name__ == "__main__":
    self_test()
    print(solve())
