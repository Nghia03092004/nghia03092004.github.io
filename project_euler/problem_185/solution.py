"""
Problem 185: Number Mind

Find the unique 16-digit secret number from Number Mind clues.
Uses backtracking with constraint propagation.
"""

def solve():
    clues = [
        ("5616185650518293", 2),
        ("3847439647293047", 1),
        ("5855462940810587", 3),
        ("9742855507068353", 3),
        ("4296849643607543", 3),
        ("3174248439465858", 1),
        ("4513559094146117", 2),
        ("7890971548908067", 3),
        ("8157356344118483", 1),
        ("2615250744386899", 2),
        ("8690095851526254", 3),
        ("6375711915077050", 1),
        ("6913859173121360", 1),
        ("6442889055042768", 2),
        ("2321386104303845", 0),
        ("2326509471271448", 2),
        ("5765889547612327", 0),
        ("6880193648553567", 0),
        ("8265420455326143", 2),
    ]

    # Parse clues: convert guess strings to lists of ints
    parsed = [(list(map(int, g)), c) for g, c in clues]

    # Possible digits per position
    possible = [set(range(10)) for _ in range(16)]

    # Apply zero-clue constraints
    for guess, count in parsed:
        if count == 0:
            for i in range(16):
                possible[i].discard(guess[i])

    # Order positions by MRV (fewest candidates first)
    order = sorted(range(16), key=lambda i: len(possible[i]))

    secret = [None] * 16
    assigned = set()

    def feasible():
        for guess, count in parsed:
            matched = 0
            can_match = 0
            for i in range(16):
                if i in assigned:
                    if secret[i] == guess[i]:
                        matched += 1
                else:
                    if guess[i] in possible[i]:
                        can_match += 1
            if matched > count:
                return False
            if matched + can_match < count:
                return False
        return True

    def backtrack(depth):
        if depth == 16:
            # Verify exact match
            for guess, count in parsed:
                matched = sum(1 for i in range(16) if secret[i] == guess[i])
                if matched != count:
                    return False
            return True

        pos = order[depth]
        assigned.add(pos)
        for d in possible[pos]:
            secret[pos] = d
            if feasible():
                if backtrack(depth + 1):
                    return True
        secret[pos] = None
        assigned.discard(pos)
        return False

    backtrack(0)
    print(''.join(map(str, secret)))

if __name__ == "__main__":
    solve()
