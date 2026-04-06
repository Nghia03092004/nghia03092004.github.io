def solve():
    """Find the sum of all 0-9 pandigital numbers with the sub-string
    divisibility property.

    Construction proceeds right-to-left, seeding with multiples of 17
    and extending leftward through divisors 13, 11, 7, 5, 3, 2.
    """
    primes = [17, 13, 11, 7, 5, 3, 2]

    # Seed: all 3-digit multiples of 17 with distinct digits
    candidates = []
    for m in range(17, 1000, 17):
        d8, d9, d10 = m // 100, (m // 10) % 10, m % 10
        if len({d8, d9, d10}) == 3:
            candidates.append(([d8, d9, d10], {d8, d9, d10}))

    # Extend leftward through each remaining prime divisor
    for p in primes[1:]:
        new_candidates = []
        for seq, used in candidates:
            for d in range(10):
                if d not in used:
                    if (100 * d + 10 * seq[0] + seq[1]) % p == 0:
                        new_candidates.append(([d] + seq, used | {d}))
        candidates = new_candidates

    # Assign remaining digit as d1 and compute sum
    total = 0
    for seq, used in candidates:
        d1 = (set(range(10)) - used).pop()
        total += int(''.join(map(str, [d1] + seq)))

    print(total)


solve()
