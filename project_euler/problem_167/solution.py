"""
Problem 167: Investigating Ulam Sequences

For U(2, 2n+1) with n=2..10, find the 10^11-th term and sum them.
Answer: 3916160068885

Key insight: U(2,v) for odd v>=5 has exactly 2 even members: 2 and some e.
After that, all members are odd. Odd c is Ulam iff is_ulam[c-2] XOR is_ulam[c-e].
This gives O(1) generation per term.

The difference sequence is eventually periodic with known periods from OEIS A100729.
"""

import sys

# Known periods from OEIS A100729 for U(2, 2n+1), n=2..10
KNOWN_PERIODS = {
    2: 32, 3: 26, 4: 444, 5: 1628, 6: 5906,
    7: 80, 8: 126960, 9: 380882, 10: 2097152
}

def solve_ulam(a, b, k, nn):
    """Find the k-th term of U(a, b)."""
    # Phase 1: Find second even member via brute force
    SMALL = 100000
    rc = bytearray(SMALL + 1)
    sq = [a, b]
    isu = bytearray(SMALL + 1)
    isu[a] = isu[b] = 1
    if a + b <= SMALL:
        rc[a + b] = 1

    second_even = -1
    nc = b + 1
    while nc <= SMALL:
        while nc <= SMALL and rc[nc] != 1:
            nc += 1
        if nc > SMALL:
            break
        u = nc
        sq.append(u)
        isu[u] = 1
        if u % 2 == 0 and u != a:
            second_even = u
        for i in range(len(sq) - 1):
            s = sq[i] + u
            if s <= SMALL and rc[s] < 3:
                rc[s] += 1
        nc = u + 1
        if second_even > 0:
            break

    print(f"U({a},{b}): second_even={second_even}", file=sys.stderr)

    # Phase 2: Fast generation using XOR rule
    kp = KNOWN_PERIODS[nn]
    needed = min(6 * kp + 10000, 14000000)
    needed = max(needed, 100000)

    MAXVAL = 60000000
    is_ulam = bytearray(MAXVAL + 2)
    seq = []
    for x in sq:
        is_ulam[x] = 1
        seq.append(x)

    c = sq[-1]
    c = c + 1 if c % 2 == 0 else c + 2

    while c <= MAXVAL and len(seq) < needed:
        w1 = (c - 2 >= 0 and c - 2 <= MAXVAL and is_ulam[c - 2])
        w2 = (c - second_even >= 0 and c - second_even <= MAXVAL and is_ulam[c - second_even])
        if w1 != w2:
            seq.append(c)
            is_ulam[c] = 1
        c += 2

    n = len(seq)
    print(f"  generated {n} terms", file=sys.stderr)

    # Compute diffs
    diffs = [seq[i+1] - seq[i] for i in range(n-1)]
    nd = len(diffs)

    # Try known period
    fp = kp
    ei = nd - 1
    fb = ei - fp + 1
    if fb - 4 * fp >= 0:
        ok = True
        for rep in range(1, 5):
            for j in range(fp):
                if diffs[fb + j] != diffs[fb - rep * fp + j]:
                    ok = False
                    break
            if not ok:
                break
        if not ok:
            print(f"  period {fp} verification FAILED", file=sys.stderr)
            return -1
    else:
        print(f"  not enough terms for period {fp}", file=sys.stderr)
        return -1

    # Find start of periodicity
    pstart = fb
    while pstart >= fp:
        match = all(diffs[pstart - fp + j] == diffs[pstart + j] for j in range(fp))
        if match:
            pstart -= fp
        else:
            break

    psum = sum(diffs[pstart:pstart + fp])

    # Verify
    tidx = n - 1
    off = tidx - pstart
    q, r = divmod(off, fp)
    pred = seq[pstart + r] + q * psum
    if pred != seq[-1]:
        print(f"  extrapolation FAILED", file=sys.stderr)
        return -1

    print(f"  period={fp}, psum={psum}", file=sys.stderr)

    idx = k - 1
    if idx < n:
        return seq[idx]
    offset = idx - pstart
    q, r = divmod(offset, fp)
    return seq[pstart + r] + q * psum

def solve():
    K = 10**11
    total = 0
    for nn in range(2, 11):
        v = 2 * nn + 1
        val = solve_ulam(2, v, K, nn)
        if val < 0:
            print(f"Failed for U(2,{v})", file=sys.stderr)
            return
        total += val
    print(total)

solve()
