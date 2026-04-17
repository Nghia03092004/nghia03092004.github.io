"""
Problem 387: Harshad Numbers
Find the sum of all strong, right truncatable Harshad primes less than 10^14.
"""

def is_prime(n: int) -> bool:
    """Deterministic Miller-Rabin primality test for n < 3.317e14."""
    if n < 2:
        return False
    if n < 4:
        return True
    if n % 2 == 0 or n % 3 == 0:
        return False
    # Small primes check
    if n < 25:
        return n in {2, 3, 5, 7, 11, 13, 17, 19, 23}
    # Write n-1 = d * 2^r
    d, r = n - 1, 0
    while d % 2 == 0:
        d //= 2
        r += 1
    # Witnesses sufficient for n < 3.317e14
    for a in [2, 3, 5, 7, 11, 13, 17]:
        if a >= n:
            continue
        x = pow(a, d, n)
        if x == 1 or x == n - 1:
            continue
        for _ in range(r - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        else:
            return False
    return True

def solve(limit: int = 10**14):
    """
    BFS over right truncatable Harshad numbers (RTHN).
    For each strong RTHN, try appending a digit to form a prime.
    """
    total = 0
    # frontier: list of (number, digit_sum)
    # Seeds: single-digit Harshad numbers 1..9
    frontier = [(d, d) for d in range(1, 10)]
    prefix_limit = limit // 10  # RTHN must be < limit/10

    while frontier:
        next_frontier = []
        for n, s in frontier:
            # Check if n is a strong Harshad number
            if n % s == 0 and is_prime(n // s):
                # Try appending digits that could make a prime
                for d in [1, 3, 7, 9]:
                    candidate = 10 * n + d
                    if candidate < limit and is_prime(candidate):
                        total += candidate

            # Expand: try appending each digit 0..9
            for d in range(10):
                new_n = 10 * n + d
                new_s = s + d
                if new_n < prefix_limit and new_s > 0 and new_n % new_s == 0:
                    next_frontier.append((new_n, new_s))

        frontier = next_frontier

    return total

# Compute and verify
answer_small = solve(10_000)
assert answer_small == 90619, f"Small test failed: {answer_small}"

answer = solve(10**14)
assert answer == 696067597313468

print(answer)
