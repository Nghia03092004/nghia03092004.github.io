"""
Problem 305: Reflexive Position

S = "123456789101112131415..."  (Champernowne string)
f(n) = starting position of the n-th occurrence of str(n) in S.
Find sum of f(3^k) for k = 1..13.

Strategy:
- Binary search on position for the n-th occurrence.
- Count occurrences of pattern in S[1..P] using:
  (a) Digit DP for internal occurrences within single numbers.
  (b) Arithmetic counting for boundary occurrences spanning consecutive numbers.
"""

def num_digits(m):
    if m == 0:
        return 1
    d = 0
    while m > 0:
        d += 1
        m //= 10
    return d

def pos_of(m):
    """Starting position of number m in the Champernowne string (1-indexed)."""
    if m <= 0:
        return 1
    pos = 1
    d = num_digits(m)
    for i in range(1, d):
        pos += 9 * i * (10 ** (i - 1))
    pos += d * (m - 10 ** (d - 1))
    return pos

def number_at_pos(p):
    """Find which number contains position p and offset within it."""
    remaining = p
    d = 1
    while True:
        count_d = 9 * (10 ** (d - 1))
        chars_d = count_d * d
        if remaining <= chars_d:
            idx = (remaining - 1) // d
            off = (remaining - 1) % d
            num = 10 ** (d - 1) + idx
            return num, off
        remaining -= chars_d
        d += 1

def build_kmp(pat):
    """Build KMP failure function."""
    L = len(pat)
    fail = [-1] * L
    for i in range(1, L):
        j = fail[i - 1]
        while j >= 0 and pat[j + 1] != pat[i]:
            j = fail[j]
        fail[i] = j + 1 if pat[j + 1] == pat[i] else -1
    return fail

def build_kmp_table(pat, fail):
    """Build KMP transition table. State is shifted by +1 (state -1 -> index 0)."""
    L = len(pat)
    table = [[0] * 10 for _ in range(L + 1)]
    for s in range(-1, L):
        for d in range(10):
            c = chr(ord('0') + d)
            state = s
            while state >= 0 and pat[state + 1] != c:
                state = fail[state]
            if pat[state + 1] == c:
                state += 1
            table[s + 1][d] = state
    return table

def count_internal(N, pat, fail, kmp_table):
    """Count total occurrences of pat within digits of numbers 1..N using digit DP."""
    if N <= 0:
        return 0
    L = len(pat)
    digits = list(map(int, str(N)))
    n = len(digits)

    # Memoized DP
    # State: (pos, kmp_state, tight, started)
    # Returns: (number_of_valid_numbers, total_occurrences)
    from functools import lru_cache

    @lru_cache(maxsize=None)
    def dp(pos, kmp_st, tight, started):
        if pos == n:
            return (1, 0) if started else (0, 0)

        limit = digits[pos] if tight else 9
        ways = 0
        occ = 0
        for d in range(0, limit + 1):
            new_tight = tight and (d == limit)
            new_started = started or (d > 0)
            if not new_started:
                new_kmp = -1
            else:
                new_kmp = kmp_table[kmp_st + 1][d]

            match = 1 if new_kmp == L - 1 else 0
            next_kmp = new_kmp
            if match:
                next_kmp = fail[L - 1]

            w, o = dp(pos + 1, next_kmp, new_tight, new_started)
            ways += w
            occ += o + match * w

        return (ways, occ)

    _, total = dp(0, -1, True, False)
    dp.cache_clear()
    return total

def count_boundary(M, pat):
    """Count boundary occurrences of pat between consecutive numbers m, m+1 for m in [1, M-1]."""
    L = len(pat)
    total = 0

    for i in range(1, L):
        suffix = pat[:i]      # last i digits of m
        prefix = pat[i:]      # first (L-i) digits of m+1

        suf_val = int(suffix)
        if prefix[0] == '0':
            continue

        prefix_val = int(prefix)
        plen = len(prefix)

        for D in range(plen, 17):
            lo_m1 = prefix_val * (10 ** (D - plen))
            hi_m1 = (prefix_val + 1) * (10 ** (D - plen)) - 1

            if lo_m1 < 10 ** (D - 1):
                continue
            if lo_m1 >= 10 ** D:
                continue

            lo_m = lo_m1 - 1
            hi_m = hi_m1 - 1

            lo_m = max(lo_m, 1)
            hi_m = min(hi_m, M - 1)
            if lo_m > hi_m:
                continue

            mod = 10 ** i
            if suf_val >= mod:
                continue

            first_m = lo_m + (suf_val - lo_m % mod) % mod
            if first_m > hi_m:
                continue

            cnt = (hi_m - first_m) // mod + 1
            total += cnt

    return total

def count_occurrences(P, pat, fail, kmp_table):
    """Count occurrences of pat fully within S[1..P]."""
    L = len(pat)
    if P < L:
        return 0

    M, off = number_at_pos(P)
    d_M = num_digits(M)

    if off == d_M - 1:
        full_up_to = M
        partial_num = 0
        partial_digits = 0
    else:
        full_up_to = M - 1
        partial_num = M
        partial_digits = off + 1

    total = 0

    # Internal occurrences in 1..full_up_to
    total += count_internal(full_up_to, pat, fail, kmp_table)

    # Boundary occurrences for m in [1, full_up_to-1]
    if full_up_to >= 2:
        total += count_boundary(full_up_to, pat)

    # Partial number: internal occurrences in visible prefix
    if partial_num > 0 and partial_digits >= L:
        s = str(partial_num)[:partial_digits]
        pos = 0
        while True:
            pos = s.find(pat, pos)
            if pos == -1:
                break
            total += 1
            pos += 1

    # Boundary between full_up_to and full_up_to+1
    if full_up_to >= 1:
        s_left = str(full_up_to)
        if partial_num == full_up_to + 1:
            s_right = str(partial_num)[:partial_digits]
        else:
            s_right = ""

        for i in range(1, L):
            need_right = L - i
            if i > len(s_left):
                continue
            if need_right > len(s_right):
                continue
            candidate = s_left[-i:] + s_right[:need_right]
            if candidate == pat:
                total += 1

    return total

def find_f(n):
    """Find f(n) = starting position of n-th occurrence of str(n) in S."""
    pat = str(n)
    L = len(pat)

    fail = build_kmp(pat)
    kmp_table = build_kmp_table(pat, fail)

    lo, hi = 1, int(2e16)

    while lo < hi:
        mid = (lo + hi) // 2
        P = mid + L - 1
        if count_occurrences(P, pat, fail, kmp_table) >= n:
            hi = mid
        else:
            lo = mid + 1

    return lo

def solve():
    total = 0
    power = 1
    for k in range(1, 14):
        power *= 3
        result = find_f(power)
        total += result
        print(f"f(3^{k}) = f({power}) = {result}")

    print(total)

solve()
