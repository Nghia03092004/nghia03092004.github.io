"""
Project Euler Problem 481: Chef Showdown

Chef k has skill S(k) = F_k / F_{n+1} (Fibonacci ratio). Chefs eliminate
each other optimally. Compute E(14), the expected number of dishes cooked.

Method: bitmask DP over game states (remaining chefs, start index).
For each state, compute winning probabilities via Theorem 2 (backward
induction on optimal elimination) and expected dishes via Theorem 3
(geometric round structure + recursive decomposition).
"""


def solve(n):
    fib = [0] * (n + 2)
    fib[1] = fib[2] = 1
    for i in range(3, n + 2):
        fib[i] = fib[i - 1] + fib[i - 2]

    skills = [fib[k + 1] / fib[n + 1] for k in range(n)]

    def get_chefs(mask):
        return [i for i in range(n) if mask & (1 << i)]

    cache_win = {}
    cache_exp = {}

    def find_next_turn(order, j, elim_chef):
        """Find the next chef in cyclic order after position j, excluding elim_chef."""
        m = len(order)
        for k in range(j + 1, m):
            if order[k] != elim_chef:
                return order[k]
        for k in range(j):
            if order[k] != elim_chef:
                return order[k]
        return None

    def compute_win(mask, start_idx):
        key = (mask, start_idx)
        if key in cache_win:
            return cache_win[key]

        chefs = get_chefs(mask)
        m = len(chefs)

        if m == 1:
            result = [0.0] * n
            result[chefs[0]] = 1.0
            cache_win[key] = result
            return result

        order = chefs[start_idx:] + chefs[:start_idx]

        p_reach = [0.0] * m
        p_reach[0] = 1.0
        for j in range(1, m):
            p_reach[j] = p_reach[j - 1] * (1.0 - skills[order[j - 1]])
        p_none = p_reach[-1] * (1.0 - skills[order[-1]])

        result = [0.0] * n
        for j in range(m):
            p_j = p_reach[j] * skills[order[j]] / (1.0 - p_none)

            best_win_prob = -1.0
            best_elim = -1
            best_wins = None

            for e in range(m):
                if e == j:
                    continue
                elim_chef = order[e]
                new_mask = mask ^ (1 << elim_chef)
                new_chefs = get_chefs(new_mask)

                next_in_cycle = find_next_turn(order, j, elim_chef)
                new_start = new_chefs.index(next_in_cycle) if next_in_cycle is not None else 0

                w = compute_win(new_mask, new_start)
                if w[order[j]] > best_win_prob + 1e-15:
                    best_win_prob = w[order[j]]
                    best_elim = e
                    best_wins = w
                elif abs(w[order[j]] - best_win_prob) < 1e-15:
                    if (e - j) % m < (best_elim - j) % m:
                        best_elim = e
                        best_wins = w

            for k in range(n):
                result[k] += p_j * best_wins[k]

        cache_win[key] = result
        return result

    def compute_exp(mask, start_idx):
        key = (mask, start_idx)
        if key in cache_exp:
            return cache_exp[key]

        chefs = get_chefs(mask)
        m = len(chefs)
        if m == 1:
            cache_exp[key] = 0.0
            return 0.0

        order = chefs[start_idx:] + chefs[:start_idx]

        p_reach = [0.0] * m
        p_reach[0] = 1.0
        for j in range(1, m):
            p_reach[j] = p_reach[j - 1] * (1.0 - skills[order[j - 1]])
        p_none = p_reach[-1] * (1.0 - skills[order[-1]])

        exp_success = sum(
            (j + 1) * p_reach[j] * skills[order[j]] for j in range(m)
        )
        exp_success /= 1.0 - p_none
        exp_failed = p_none / (1.0 - p_none) * m
        exp_dishes = exp_failed + exp_success

        exp_after = 0.0
        for j in range(m):
            p_j = p_reach[j] * skills[order[j]] / (1.0 - p_none)

            best_win_prob = -1.0
            best_elim = -1
            best_new_mask = 0
            best_new_start = 0

            for e in range(m):
                if e == j:
                    continue
                elim_chef = order[e]
                new_mask = mask ^ (1 << elim_chef)
                new_chefs = get_chefs(new_mask)

                next_in_cycle = find_next_turn(order, j, elim_chef)
                new_start = new_chefs.index(next_in_cycle) if next_in_cycle is not None else 0

                w = compute_win(new_mask, new_start)
                if w[order[j]] > best_win_prob + 1e-15:
                    best_win_prob = w[order[j]]
                    best_elim = e
                    best_new_mask = new_mask
                    best_new_start = new_start
                elif abs(w[order[j]] - best_win_prob) < 1e-15:
                    if (e - j) % m < (best_elim - j) % m:
                        best_elim = e
                        best_new_mask = new_mask
                        best_new_start = new_start

            exp_after += p_j * compute_exp(best_new_mask, best_new_start)

        result = exp_dishes + exp_after
        cache_exp[key] = result
        return result

    full_mask = (1 << n) - 1
    return compute_exp(full_mask, 0)


if __name__ == "__main__":
    E7 = solve(7)
    print(f"E(7) = {E7:.8f}")
    E14 = solve(14)
    print(f"E(14) = {E14:.8f}")
