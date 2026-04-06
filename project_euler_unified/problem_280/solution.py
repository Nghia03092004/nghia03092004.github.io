#!/usr/bin/env python3
"""
Problem 280: Ant and Seeds

5x5 grid with seeds in top row (y=0). Ant at center (2,2).
Random walk to adjacent cells. Picks up seed from top-row cell (if not carrying).
Deposits seed in bottom-row cell (y=4) only if that cell is empty.
Goal: expected steps until each bottom-row cell has one seed.

State: (position, top_mask, bottom_mask, carrying)
Conservation: popcount(top) + popcount(bottom) + carrying = 5
Terminal: top=0, bottom=31, carrying=0

Solved via value iteration.
"""

def solve():
    NPOS = 25
    NMASK = 32

    # Precompute neighbors
    neighbors = [[] for _ in range(25)]
    for y in range(5):
        for x in range(5):
            p = y * 5 + x
            for dx, dy in [(0,1),(0,-1),(1,0),(-1,0)]:
                nx, ny = x+dx, y+dy
                if 0 <= nx < 5 and 0 <= ny < 5:
                    neighbors[p].append(ny*5+nx)

    def popcount(x):
        return bin(x).count('1')

    # Collect valid states
    states = []
    state_idx = {}
    for pos in range(NPOS):
        for top in range(NMASK):
            for bot in range(NMASK):
                for carry in range(2):
                    if popcount(top) + popcount(bot) + carry == 5:
                        state_idx[(pos,top,bot,carry)] = len(states)
                        states.append((pos,top,bot,carry))

    n = len(states)
    E = [0.0] * n

    # Start state
    start = state_idx[(12, 31, 0, 0)]
    # Terminal: top=0, bot=31, carry=0

    for iteration in range(100000):
        Enew = [0.0] * n
        maxdiff = 0.0
        for i, (pos, top, bot, carry) in enumerate(states):
            if top == 0 and bot == 31 and carry == 0:
                Enew[i] = 0.0
                continue

            val = 1.0
            nn = len(neighbors[pos])
            for np_ in neighbors[pos]:
                nx, ny = np_ % 5, np_ // 5
                nt, nb, nc = top, bot, carry

                if ny == 0 and nc == 0 and (nt & (1 << nx)):
                    nc = 1
                    nt &= ~(1 << nx)
                elif ny == 4 and nc == 1 and not (nb & (1 << nx)):
                    nc = 0
                    nb |= (1 << nx)

                j = state_idx[(np_, nt, nb, nc)]
                val += E[j] / nn

            Enew[i] = val
            maxdiff = max(maxdiff, abs(val - E[i]))

        E = Enew
        if maxdiff < 1e-10:
            break

    return E[start]

if __name__ == "__main__":
    answer = solve()
    print(f"{answer:.6f}")
