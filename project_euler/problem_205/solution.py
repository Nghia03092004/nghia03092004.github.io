"""
Problem 205: Dice Game

Peter has 9 four-sided dice; Colin has 6 six-sided dice.
Find P(Peter beats Colin), rounded to 7 decimal places.

Compute sum distributions via convolution, then sum over all (p, c) pairs
where Peter's total p > Colin's total c.
"""

def solve():
    from itertools import product as iprod

    def dice_distribution(n_dice, faces):
        """Compute frequency distribution for sum of n_dice dice with given faces."""
        freq = {0: 1}
        for _ in range(n_dice):
            new_freq = {}
            for s, cnt in freq.items():
                for f in range(1, faces + 1):
                    new_freq[s + f] = new_freq.get(s + f, 0) + cnt
            freq = new_freq
        return freq

    peter = dice_distribution(9, 4)   # sums 9..36
    colin = dice_distribution(6, 6)   # sums 6..36

    # Cumulative distribution for Colin: P(Colin < p)
    colin_cum = {}
    running = 0
    for s in range(0, 37):
        colin_cum[s] = running
        running += colin.get(s, 0)

    # P(Peter wins) = sum_p peter[p] * colin_cum[p] / (4^9 * 6^6)
    win_count = sum(peter[p] * colin_cum[p] for p in peter)

    total = (4**9) * (6**6)
    prob = win_count / total

    print(f"{prob:.7f}")

if __name__ == "__main__":
    solve()
