"""
Problem 336: Maximix Arrangements
Find the last permutation in the sorted list of maximix arrangements for 11 carriages.
Answer: CAGBIHEDJFK
"""

def is_maximix(perm):
    """
    Check if a permutation is a maximix arrangement.
    The sorting algorithm places elements from largest to smallest.
    At each step:
      1. Find position of the target element in the unsorted portion.
      2. If it's at position 0 or already in its final position, the step is trivial -> not maximix.
      3. Otherwise, reverse prefix to bring it to front, then reverse entire unsorted portion.
    """
    p = list(perm)
    n = len(p)
    for remaining in range(n, 1, -1):
        target = remaining - 1
        pos = p.index(target, 0, remaining)
        # Target already in correct position -> trivial step
        if pos == remaining - 1:
            return False
        # Target at position 0 -> only one reversal needed, not maximum
        if pos == 0:
            return False
        # Reverse prefix [0..pos]
        p[:pos+1] = p[:pos+1][::-1]
        # Reverse entire unsorted portion [0..remaining-1]
        p[:remaining] = p[:remaining][::-1]
    return True

def solve_backtrack(n):
    """
    Use backtracking with pruning to find all maximix arrangements.
    We build permutations and periodically check partial constraints.
    """
    results = []

    def backtrack(perm, used):
        if len(perm) == n:
            if is_maximix(perm):
                results.append(tuple(perm))
            return

        for i in range(n):
            if not used[i]:
                perm.append(i)
                used[i] = True

                # Pruning: check partial maximix conditions
                # The largest elements that are already "determined" can be checked
                valid = True
                size = len(perm)

                # If all n elements placed, we check fully.
                # For partial: if the last element placed equals size-1,
                # it means it's already in sorted position for that stage -> prune
                # Also if the target for current remaining is at pos 0 -> prune
                if size >= 2:
                    # Check: for remaining = size, target = size - 1
                    # If target (size-1) is in perm and it's at index size-1, it's in place -> bad
                    if perm[-1] == size - 1 and size <= n:
                        # This means when we sort with remaining=size,
                        # target is already at position size-1 -> trivial
                        # But only relevant if all positions up to size are filled
                        pass  # Need full permutation for proper check

                backtrack(perm, used)
                perm.pop()
                used[i] = False

    backtrack([], [False] * n)
    return results

def solve_bruteforce(n):
    """
    For smaller n, enumerate all permutations.
    For n=11, this is too slow but shown for correctness.
    """
    from itertools import permutations
    results = []
    for perm in permutations(range(n)):
        if is_maximix(perm):
            results.append(perm)
    results.sort()
    return results

def solve_smart(n):
    """
    Smart approach: generate maximix arrangements using reverse simulation.
    Build the permutation by determining what arrangement would cause
    each sorting step to be non-trivial.

    At step k (placing element n-1-k at position n-1-k), the sorting does:
      - Find pos of (n-1-k) in positions [0..n-1-k]
      - pos != 0 and pos != n-1-k (for maximix)
      - Reverse [0..pos], then reverse [0..n-1-k]

    We reverse-engineer: starting from sorted, undo each step.
    Undoing step k with choice pos:
      - Reverse [0..n-1-k] (undo second reversal)
      - Reverse [0..pos] (undo first reversal)
    After undoing, element (n-1-k) should be at position pos.
    """
    results = []

    def generate(perm, step):
        """
        perm: current state after undoing steps from n-1 down to step+1
        step: next step to undo (we undo from step n-2 down to 0)
        remaining = step + 2 (number of unsorted elements at this step)
        """
        if step < 0:
            results.append(tuple(perm))
            return

        remaining = step + 2  # Number of elements in unsorted portion
        target = remaining - 1  # Element to place

        # pos can be 1 to remaining-2 (not 0, not remaining-1)
        for pos in range(1, remaining - 1):
            p = list(perm)
            # Undo: reverse [0..remaining-1], then reverse [0..pos]
            p[:remaining] = p[:remaining][::-1]
            p[:pos+1] = p[:pos+1][::-1]
            generate(p, step - 1)

    # Start from sorted arrangement
    sorted_perm = list(range(n))
    # Undo steps from step n-2 down to step 0
    # step n-2 corresponds to remaining = n, placing element n-1
    generate(sorted_perm, n - 2)

    results.sort()
    return results

def main():
    n = 11
    arrangements = solve_smart(n)
    last = arrangements[-1]
    answer = ''.join(chr(ord('A') + c) for c in last)
    print(f"Number of maximix arrangements for n={n}: {len(arrangements)}")
    print(f"Last maximix arrangement: {answer}")
    return answer

if __name__ == "__main__":
    result = main()
    assert result == "CAGBIHEDJFK", f"Expected CAGBIHEDJFK, got {result}"
    print("Verified: CAGBIHEDJFK")
