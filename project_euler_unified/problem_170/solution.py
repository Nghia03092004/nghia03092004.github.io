"""
Problem 170: Find the largest 0-9 pandigital formed by concatenating products.

Products n*c_1 || n*c_2 || ... must be a 10-digit pandigital (0-9).
Inputs n || c_1 || c_2 || ... must also be a 10-digit pandigital (0-9).
At least 2 multipliers required.

Answer: 9857164023

For each n, enumerate multipliers c with digits from the available pool,
compute products, and use bitmask DP to find the best covering.
"""

def solve():
    FULL = (1 << 10) - 1
    p10 = [1] * 11
    for i in range(1, 11):
        p10[i] = p10[i-1] * 10

    def dmask(x):
        if x == 0:
            return -1
        m = 0
        while x > 0:
            d = x % 10
            if m & (1 << d):
                return -1
            m |= (1 << d)
            x //= 10
        return m

    def ndig(x):
        c = 0
        while x > 0:
            c += 1
            x //= 10
        return c if c else 1

    global_best = 0

    for n in range(2, 100):
        nm = dmask(n)
        if nm < 0:
            continue
        nnd = ndig(n)
        avail = FULL ^ nm
        avail_nd = 10 - nnd
        if avail_nd < 2:
            continue

        # DFS to enumerate multipliers c with digits from avail
        pairs = []  # list of (c_mask, p_mask, p_nd, p_val)
        stack = []
        for d in range(10):
            if not (avail & (1 << d)):
                continue
            if d == 0:
                continue
            stack.append((d, 1 << d, 1))

        while stack:
            val, mask, nd = stack.pop()
            prod = n * val
            pm = dmask(prod)
            if pm >= 0:
                pnd = ndig(prod)
                if bin(pm).count('1') == pnd:
                    pairs.append((mask, pm, pnd, prod))
            if nd < avail_nd:
                for d in range(10):
                    if not (avail & (1 << d)):
                        continue
                    if mask & (1 << d):
                        continue
                    stack.append((val * 10 + d, mask | (1 << d), nd + 1))

        if len(pairs) < 2:
            continue

        # Keep best product per (c_mask, p_mask)
        best_cp = {}
        for cm, pm, pnd, pv in pairs:
            key = (cm, pm)
            if key not in best_cp or pv > best_cp[key][0]:
                best_cp[key] = (pv, pnd)

        # Bitmask DP
        dp = {}  # key=(cu, pu) -> (best_val, nd, nseg)

        for (cm, pm), (pv, pnd) in best_cp.items():
            key = (cm, pm)
            if key not in dp or pv > dp[key][0]:
                dp[key] = (pv, pnd, 1)

        for _pass in range(9):
            changed = False
            entries = list(dp.items())
            for (ecu, epu), (ev, end, ens) in entries:
                for (cm, pm), (pv, pnd) in best_cp.items():
                    if ecu & cm:
                        continue
                    if epu & pm:
                        continue
                    ncu = ecu | cm
                    npu = epu | pm
                    c1 = ev * p10[pnd] + pv
                    c2 = pv * p10[end] + ev
                    bv = max(c1, c2)
                    nnd = end + pnd
                    ns = ens + 1
                    nkey = (ncu, npu)
                    if nkey not in dp or bv > dp[nkey][0]:
                        dp[nkey] = (bv, nnd, ns)
                        changed = True
            if not changed:
                break

        target = (avail, FULL)
        if target in dp and dp[target][2] >= 2 and dp[target][1] == 10:
            if dp[target][0] > global_best:
                global_best = dp[target][0]

    print(global_best)

solve()
