"""
Problem 882: Matchstick Equations
Matchstick cost per digit: 0->6, 1->2, 2->5, 3->5, 4->4, 5->5, 6->6, 7->3, 8->7, 9->6.
"""

COST = {0: 6, 1: 2, 2: 5, 3: 5, 4: 4, 5: 5, 6: 6, 7: 3, 8: 7, 9: 6}

def matchstick_cost(n):
    """Total matchstick cost of displaying number n."""
    if n == 0:
        return COST[0]
    total = 0
    while n > 0:
        total += COST[n % 10]
        n //= 10
    return total

def max_number_dp(m):
    """Find maximum number displayable with exactly m matchsticks."""
    if m <= 1:
        return -1
    # dp[s] = max number with exactly s matchsticks, or -1 if impossible
    dp = [-1] * (m + 1)
    dp[0] = 0
    for s in range(1, m + 1):
        for d in range(10):
            c = COST[d]
            if s >= c and dp[s - c] >= 0:
                candidate = dp[s - c] * 10 + d
                if candidate > dp[s]:
                    dp[s] = candidate
    return dp[m]

def max_number_greedy(m):
    """Greedy: maximize digits using 1s (cost 2) and 7 (cost 3)."""
    if m <= 1:
        return -1
    if m % 2 == 0:
        return int("1" * (m // 2))
    else:
        return int("7" + "1" * ((m - 3) // 2))

def count_numbers_with_cost(s, max_digits=6):
    """Count numbers whose matchstick cost is exactly s."""
    count = 0
    upper = 10 ** max_digits
    for n in range(0, min(upper, 10 ** 4)):
        if matchstick_cost(n) == s:
            count += 1
    return count

def count_equations(budget):
    """Count valid A + B = C where total matchstick cost = budget.
    Budget includes 4 for the + and = signs."""
    remaining = budget - 4  # matchsticks for A, B, C
    if remaining <= 0:
        return 0
    count = 0
    # Enumerate small A, B
    for A in range(0, 200):
        cA = matchstick_cost(A)
        if cA > remaining:
            continue
        for B in range(A, 200):  # A <= B to avoid double counting
            cB = matchstick_cost(B)
            if cA + cB >= remaining:
                continue
            C = A + B
            cC = matchstick_cost(C)
            if cA + cB + cC == remaining:
                count += 1
    return count

# --- Verification ---
print("=== Matchstick Costs ===")
for n in range(10):
    print(f"  cost({n}) = {COST[n]}")

print("\n=== Max Number (DP vs Greedy) ===")
for m in range(2, 21):
    dp_val = max_number_dp(m)
    greedy_val = max_number_greedy(m)
    print(f"  m={m:>2}: DP={dp_val:>10}, Greedy={greedy_val:>10}, "
          f"Match={'OK' if dp_val == greedy_val else 'FAIL'}")
    assert dp_val == greedy_val

print("\n=== Matchstick Cost Verification ===")
test_cases = [(1, 2), (7, 3), (11, 4), (71, 5), (111, 6)]
for n, expected in test_cases:
    actual = matchstick_cost(n)
    print(f"  cost({n}) = {actual}, expected {expected}: "
          f"{'OK' if actual == expected else 'FAIL'}")
    assert actual == expected

print("\n=== Small Equations ===")
for budget in range(10, 30):
    cnt = count_equations(budget)
    if cnt > 0:
        print(f"  budget={budget}: {cnt} valid equations")

answer = count_equations(20)
print(f"\nAnswer: equations with budget 20 = {answer}")

# --- 4-Panel Visualization ---
