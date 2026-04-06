"""
Problem 240: Top Dice

How many ways can 20 twelve-sided dice be rolled so that the top 10 sum to 70?

Approach: Enumerate sorted dice configurations (frequency vectors).
For each face value from 12 down to 1, decide how many dice show that value.
The top-10 sum is determined by the sorted order.

For a non-increasing sequence d_1 >= d_2 >= ... >= d_20, the top 10 are d_1,...,d_10.
Their sum must equal 70.

We recurse on face value from 12 down to 1, tracking:
- Total dice assigned so far
- How many of those are in the top 10 (the first 10 positions)
- Sum of the top-10 portion

At each face value, the count of dice with that value determines how many
go to the "top" part (they fill from the top first in descending order).
"""

from math import factorial

def solve(num_dice, num_sides, top_count, target_sum):
    fact = [1] * (num_dice + 1)
    for i in range(1, num_dice + 1):
        fact[i] = fact[i-1] * i

    total = 0

    def recurse(face, dice_left, top_left, sum_left, multinomial_denom):
        """
        face: current face value (descending from num_sides to 1)
        dice_left: dice not yet assigned
        top_left: top-k positions not yet filled
        sum_left: remaining sum needed for top-k
        multinomial_denom: product of count! for each face assigned so far
        """
        nonlocal total

        if top_left == 0 and sum_left != 0:
            return
        if top_left == 0 and sum_left == 0:
            # Top is complete. Remaining dice go to bottom, each <= face.
            # Number of ways to assign dice_left dice with values in {1,...,face}:
            # This is a stars-and-bars with upper bounds problem.
            # We need the multinomial coefficient contribution.
            # Actually, we need to continue assigning to get the full frequency vector.
            if face == 0:
                if dice_left == 0:
                    total += fact[num_dice] // multinomial_denom
                return
            # Assign bottom dice: values from face down to 1
            assign_bottom(face, dice_left, multinomial_denom)
            return

        if face == 0:
            if dice_left == 0 and top_left == 0 and sum_left == 0:
                total += fact[num_dice] // multinomial_denom
            return

        if dice_left < top_left:
            return  # not enough dice to fill top

        # Pruning: max possible sum from remaining top positions
        if top_left * face < sum_left:
            return
        # Min possible sum
        if sum_left < top_left:  # each die is at least 1
            return

        # Choose count of dice with value 'face': 0 to dice_left
        for count in range(dice_left + 1):
            # These 'count' dice have value 'face'.
            # In sorted order, they fill top positions first.
            top_from_this = min(count, top_left)
            # This is fixed: we MUST put min(count, top_left) into top.
            # No choice here -- in sorted order, higher values come first.

            new_top_left = top_left - top_from_this
            new_sum = sum_left - top_from_this * face
            new_dice = dice_left - count

            if new_sum < 0:
                break

            recurse(face - 1, new_dice, new_top_left, new_sum,
                    multinomial_denom * fact[count])

    def assign_bottom(face, dice_left, multinomial_denom):
        """Assign remaining bottom dice with values in {1, ..., face}."""
        nonlocal total
        if dice_left == 0:
            total += fact[num_dice] // multinomial_denom
            return
        if face == 0:
            return  # no values left but dice remain
        if face == 1:
            # All remaining dice must be 1
            total += fact[num_dice] // (multinomial_denom * fact[dice_left])
            return
        # Pruning: need at least dice_left dice, each at least 1. Always satisfiable if face >= 1.
        for count in range(dice_left + 1):
            assign_bottom(face - 1, dice_left - count, multinomial_denom * fact[count])

    recurse(num_sides, num_dice, top_count, target_sum, 1)
    return total

# Verify
small = solve(5, 6, 3, 15)
print(f"Verification (5d6, top 3 = 15): {small}")
assert small == 1111, f"Expected 1111, got {small}"

# Solve
answer = solve(20, 12, 10, 70)
print(answer)
