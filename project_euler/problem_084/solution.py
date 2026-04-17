"""
Problem 84: Monopoly Odds

Using two 4-sided dice, find the six-digit modal string for the three
most popular squares in Monopoly via Monte Carlo simulation.

Answer: 101524
"""

import random


def simulate(sides=4, num_turns=10_000_000, seed=42):
    """Monte Carlo simulation of the Monopoly Markov chain."""
    rng = random.Random(seed)

    GO, JAIL, G2J = 0, 10, 30
    CC = {2, 17, 33}
    CH = {7, 22, 36}
    R = [5, 15, 25, 35]
    U = [12, 28]

    def next_r(pos):
        for r in R:
            if pos < r:
                return r
        return R[0]

    def next_u(pos):
        for u in U:
            if pos < u:
                return u
        return U[0]

    visits = [0] * 40
    pos = 0
    doubles_count = 0

    for _ in range(num_turns):
        d1 = rng.randint(1, sides)
        d2 = rng.randint(1, sides)
        is_double = (d1 == d2)

        if is_double:
            doubles_count += 1
        else:
            doubles_count = 0

        if doubles_count == 3:
            pos = JAIL
            doubles_count = 0
            visits[pos] += 1
            continue

        pos = (pos + d1 + d2) % 40

        if pos == G2J:
            pos = JAIL
        elif pos in CC:
            card = rng.randint(1, 16)
            if card == 1:
                pos = GO
            elif card == 2:
                pos = JAIL
        elif pos in CH:
            card = rng.randint(1, 16)
            if card == 1:
                pos = GO
            elif card == 2:
                pos = JAIL
            elif card == 3:
                pos = 11   # C1
            elif card == 4:
                pos = 24   # E2
            elif card == 5:
                pos = 39   # H2
            elif card == 6:
                pos = 5    # R1
            elif card == 7:
                pos = next_r(pos)
            elif card == 8:
                pos = next_r(pos)
            elif card == 9:
                pos = next_u(pos)
            elif card == 10:
                pos = (pos - 3) % 40
                if pos in CC:
                    cc_card = rng.randint(1, 16)
                    if cc_card == 1:
                        pos = GO
                    elif cc_card == 2:
                        pos = JAIL

        visits[pos] += 1

    return visits


def main():
    visits = simulate(sides=4, num_turns=10_000_000)
    ranked = sorted(range(40), key=lambda i: visits[i], reverse=True)
    modal_string = ''.join(f'{sq:02d}' for sq in ranked[:3])
    print(modal_string)


if __name__ == "__main__":
    main()
