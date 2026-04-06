"""
Project Euler Problem 521: Smallest Prime Factor

Let smpf(n) = smallest prime factor of n.
Compute S(N) = sum_{n=2}^{N} smpf(n) mod 10^9.

Uses the Lucy DP (prime-counting sieve) combined with recursive
enumeration of composite contributions by smallest prime factor.
"""

import math

MOD = 10**9


def solve(N):
    """Compute S(N) = sum_{n=2}^{N} smpf(n) mod MOD via Lucy DP."""
    if N < 2:
        return 0

    sqrtN = int(math.isqrt(N))

    # Collect distinct floor quotients N//i
    vals = []
    i = 1
    while i <= N:
        v = N // i
        vals.append(v)
        i = N // v + 1
    vals = sorted(set(vals))

    # Index mapping: small values use direct index, large use N//v
    small = [0] * (sqrtN + 2)
    large = [0] * (sqrtN + 2)

    def idx(v):
        return v if v <= sqrtN else len(vals) - (N // v)

    # S0[v] = count of "surviving" integers in [2,v]
    # S1[v] = sum of "surviving" integers in [2,v]
    S0 = {}
    S1 = {}
    for v in vals:
        S0[v] = v - 1
        S1[v] = (v * (v + 1) // 2 - 1) % MOD

    # Lucy DP sieve
    primes_list = []
    for p in range(2, sqrtN + 1):
        if S0[p] == S0.get(p - 1, 0):
            continue
        primes_list.append(p)
        p_count = S0[p - 1]
        p_sum = S1[p - 1]
        p2 = p * p
        for v in reversed(vals):
            if v < p2:
                break
            vp = v // p
            cnt = S0[vp] - p_count
            S0[v] -= cnt
            S1[v] = (S1[v] - p % MOD * ((S1[vp] - p_sum) % MOD)) % MOD

    # Now S1[v] = sum of primes <= v (mod MOD), S0[v] = pi(v).
    # Recursive function to compute sum of smpf(n) for n in [2,v]
    # with smpf(n) >= primes_list[pidx].
    # T(v, pidx) = sum of smpf(n) for n in [2,v] where smpf(n) >= primes_list[pidx]
    # = (sum of primes in [primes_list[pidx], v])
    #   + sum over primes p = primes_list[pidx..] with p^2 <= v of
    #     p * (T(v/p, pidx_of_p) - (sum of primes >= p in [p, v/p]) + ...)
    # We implement this iteratively.

    def smpf_sum(v, pi):
        """
        Compute sum of smpf(n) for all n in [2,v] with smpf(n) >= primes_list[pi].
        """
        # Prime contributions: primes p in [primes_list[pi], v]
        if pi >= len(primes_list):
            # Only primes > sqrt(N) can appear; their contribution is S1[v] - S1[prev]
            return (S1[v] - (S1[primes_list[-1] - 1] if primes_list else 0)) % MOD if v >= 2 else 0
        p = primes_list[pi]
        if p > v:
            return 0
        # Sum of primes in [p, v] = S1[v] - S1[p-1]
        result = (S1[v] - S1[p - 1]) % MOD
        # Composite contributions with smpf = q for each prime q >= p with q^2 <= v
        for j in range(pi, len(primes_list)):
            q = primes_list[j]
            if q * q > v:
                break
            # Composites with smpf = q: n = q*m where smpf(m) >= q, m in [q, v/q]
            # Each contributes smpf(n) = q
            # But m can be prime or composite, and we need to count how many such m
            # Actually we need: for each such n = q*m, smpf(n) = q, so contribution is q.
            # Count of m in [q, v/q] with smpf(m) >= q:
            #   = (S0[v/q] - pi(q-1)) for m being prime, plus composites with smpf >= q
            # We iterate: n = q*m, q*m^2 could give deeper nesting.
            # Use: sum of smpf over composites with smpf = q in [2,v]
            #    = q * (number of m in [q, v//q] with smpf(m) >= q)
            #    PLUS for composites n = q*m where m itself is composite with smpf >= q,
            #    we still contribute smpf(n) = q, not smpf(m).
            # So: contribution = q * |{m in [q, v//q] : smpf(m) >= q}|
            # |{m in [q, v//q] : smpf(m) >= q}| = (S0[v//q] - S0[q-1]) + ...
            # Actually we need all m >= q in [2, v//q] with smpf(m) >= q.
            # = |{m in [2, v//q] : smpf(m) >= q}| - |{m in [2, q-1] : smpf(m) >= q}|
            # Since primes < q have smpf < q, and q-1 < q, there are no integers in [2,q-1] with smpf >= q
            # except... actually composites in [2, q-1] all have smpf < q. And primes in [2,q-1] have smpf = themselves < q.
            # So |{m in [2, q-1] : smpf(m) >= q}| = 0.
            # Thus count = |{m in [2, v//q] : smpf(m) >= q}| = (v//q - 1) - (number removed by primes < q)
            # After Lucy DP, the number of integers in [2, v//q] with smpf >= q... hmm.
            # Actually the Lucy DP S0 after sieving up to all primes gives pi(v).
            # We need intermediate values.
            # This approach requires storing intermediate DP states, which we didn't do.
            pass

        return result % MOD

    # Since the full recursive enumeration requires intermediate Lucy DP states
    # (which we overwrote during the sieve), we use an alternative approach:
    # Re-run the sieve but accumulate smpf contributions during the sieve itself.

    # Reset and re-run with smpf accumulation
    T = {}  # T[v] = sum of smpf(n) for n in [2,v], built incrementally
    for v in vals:
        T[v] = (v * (v + 1) // 2 - 1) % MOD  # initially assume smpf(n) = n

    # Re-initialise S0 for counting
    S0b = {}
    for v in vals:
        S0b[v] = v - 1

    for p in range(2, sqrtN + 1):
        if S0b[p] == S0b.get(p - 1, 0):
            continue
        p_count = S0b[p - 1]
        p2 = p * p
        for v in reversed(vals):
            if v < p2:
                break
            vp = v // p
            cnt = S0b[vp] - p_count
            S0b[v] -= cnt
            # These cnt composites previously had their "smpf" assumed to be themselves.
            # Actually they are multiples of p whose smpf is p.
            # We need to subtract their assumed contribution and add p * cnt.
            # The assumed contribution of these composites (their values) was accumulated
            # in the initial T[v] = sum of all integers in [2,v].
            # But we can't easily extract "the sum of values of composites with smpf=p".

    # The standard approach: use a separate DP that directly computes smpf sums.
    # Let F(v, p) = sum of smpf(n) for n in [2,v] where smpf(n) > p.
    # Then S(N) = F(N, 1) + ... = total smpf sum.
    # F(v, p_last) = sum of primes q in (p_last, v], each contributing q (as smpf(q)=q),
    #               + sum over primes q > p_last with q^2 <= v of
    #                 q * (number of composites with smpf = q in [2,v])
    # Actually we combine: S(N) = sum of primes up to N + sum over primes p with p^2 <= N
    #   of p * (count of m with smpf(m) >= p and p*m <= N, m >= 2).

    # For a correct O(N^{3/4}/log N) solution, we use the "min-25 sieve" variant.

    # Intermediate prime counts at each sieve level are needed.
    # We store them by re-running the sieve and saving snapshots.

    # Re-initialise
    S0c = {}
    for v in vals:
        S0c[v] = v - 1

    # We'll process primes and at each level, after sieving prime p,
    # we compute the composite contribution with smpf = p.
    result = 0

    # First pass: get prime sum = S1[N] (already computed)
    result = S1[N] % MOD

    # Second pass: accumulate composite contributions
    # Before sieving, S0c[v] = v - 1 (count of [2,v])
    for p in range(2, sqrtN + 1):
        if S0c[p] == S0c.get(p - 1, 0):
            continue
        # At this point, S0c[v] = count of integers in [2,v] with smpf >= p
        # (those not yet sieved out)
        p_count = S0c[p - 1]  # = pi(p-1)

        # Composites with smpf = p contribute:
        # For n = p*m with m in [p, N/p] and smpf(m) >= p:
        #   count(m) for each power: m can be p, p^2, ..., or any composite with smpf >= p
        # Actually, for the first level: number of n in [2, N] with smpf = p
        #   = (count of m in [2, N//p] with smpf >= p) - (if p itself is in that count, it's fine,
        #     p*p is a composite with smpf = p, p itself as m gives n = p^2 etc.)
        # count of m in [p, N//p] with smpf >= p = S0c[N//p] - S0c[p-1]
        # But wait, we also need to handle higher powers: p*m where m has smpf >= p
        # and m itself could be p (giving p^2), or m = p*m' (giving p^2*m'), etc.
        # For smpf sum, contribution of n with smpf(n) = p is just p per such n.
        # So total composite contribution = p * (count of composites with smpf = p in [2,N])

        # Count of integers in [2, N] with smpf = p:
        # = (count of integers in [2, N//p] with smpf >= p) where we map n -> n/p
        #   but n must be composite, i.e., n != p, so m = n/p >= 2, and... actually n = p is prime.
        # Integers with smpf = p in [2, N]:
        #   = {p} union {pm : m >= 2, smpf(m) >= p, pm <= N}
        #   = {p} union {pm : m in [2, N//p], smpf(m) >= p}
        # The prime p is already counted in S1[N], so we need composites only.
        # Composites with smpf = p: {pm : m in [2, N//p], smpf(m) >= p}
        # Count = (S0c[N//p] - p_count)  ... but this includes m being primes >= p
        # (giving n = p * prime, which is composite with smpf = p, correct)
        # and m being 1? No, m >= 2.
        # S0c[N//p] = count of integers in [2, N//p] with smpf >= p
        # So count of composites in [2, N] with smpf = p
        #   = S0c[N//p] - p_count  -- NO wait.
        # S0c[v] before this sieve step = (count of integers in [2,v] with smpf >= p)
        # = (v - 1) - (count of composites with smpf < p sieved out so far)
        # = (count of primes < p) + (count of integers in [2,v] with smpf >= p)
        # Hmm, S0c[v] after sieving primes < p = pi(p-1) + (count of integers with smpf >= p in [2,v])
        # Wait no. Let me reconsider.
        # Initially S0c[v] = v-1. After sieving by prime q, we remove composites with smpf = q.
        # So after sieving all primes < p:
        # S0c[v] = (# primes in [2,v]) that are < p, PLUS (# integers in [2,v] with smpf >= p)
        # = pi(p-1) + |{n in [2,v] : smpf(n) >= p}| ... wait that double counts.
        # Actually: S0c[v] = |{n in [2,v] : n is prime or smpf(n) >= p}|
        # = |{primes in [2,v] with value < p}| + |{primes in [2,v] with value >= p}|
        #   + |{composites in [2,v] with smpf >= p}|
        # = pi(v) restricted to... no. Think again.
        # At the start, all of [2,v] is in. After sieving prime 2, composites with smpf=2 removed.
        # After sieving 3, composites with smpf=3 removed. Etc.
        # After sieving all primes < p:
        # S0c[v] = |[2,v]| - |{composites in [2,v] with smpf < p}|
        # = |{primes in [2,v]}| + |{composites in [2,v] with smpf >= p}|
        # = pi(v) + |{composites in [2,v] with smpf >= p}|

        # So |{composites in [2,v] with smpf >= p}| = S0c[v] - pi(v)
        # And pi(p-1) = S0c[p-1] = p_count (since for v = p-1, there are no composites
        # in [2, p-1] with smpf >= p, so S0c[p-1] = pi(p-1)).

        # Composites with smpf = p in [2, N]:
        # = |{m in [2, N//p] : smpf(m) >= p}| (each gives composite n = pm with smpf = p)
        # = S0c[N//p] - pi(p-1) ... no.
        # |{m in [2, N//p] : smpf(m) >= p}| = S0c[N//p] - pi(p-1)
        #   only if all primes < p have been sieved and S0c properly reflects that.
        # Wait: S0c[N//p] = pi(N//p) + |{composites in [2, N//p] with smpf >= p}|
        # |{integers in [2, N//p] with smpf >= p}| includes primes >= p and composites with smpf >= p
        # = (pi(N//p) - pi(p-1)) + |{composites in [2, N//p] with smpf >= p}|
        # = S0c[N//p] - pi(p-1) - pi(p-1) + ... this is getting confused.

        # Simpler: |{m in [2, N//p] : m is prime and m >= p, OR m is composite with smpf >= p}|
        # = S0c[N//p] - pi(p-1)
        # Because S0c[N//p] counts primes in [2, N//p] plus composites with smpf >= p.
        # Primes in [2, p-1] number pi(p-1). The rest (primes >= p and composites with smpf >= p)
        # is S0c[N//p] - pi(p-1).

        # But we want m >= 2 with smpf(m) >= p. That's:
        # = (primes in [p, N//p]) + (composites in [2, N//p] with smpf >= p)
        # = S0c[N//p] - pi(p-1)
        # Yes, this is correct.

        # But for composites n = pm to satisfy smpf(n) = p, we need m >= 2.
        # If m is a prime >= p, then n = pm is composite with smpf = p. Correct.
        # If m is composite with smpf(m) = q >= p, then smpf(pm) = p. Correct.
        # If m = 1, n = p which is prime, not composite. But m = 1 isn't in [2, N//p].

        # However we also need to handle the multi-level nesting for higher powers of p.
        # Actually for computing smpf sum, we just need:
        # sum over composites n with smpf(n) = p of smpf(n) = p * (count of such composites)
        # And the count of composites in [2, N] with smpf = p is exactly:
        # |{m in [2, N//p] : smpf(m) >= p}| = S0c[N//p] - pi(p-1) = S0c[N//p] - p_count

        # But wait, this only counts composites n = pm with m in [2, N//p].
        # These include n = p*p, p*p*p, etc. (when m = p, p^2, ...).
        # And n = p*q for prime q >= p, n = p*q*r, etc.
        # All of these have smpf = p. So the count is correct.

        # But S0c[N//p] - p_count over-counts:
        # It counts m = p (prime) giving n = p^2 (composite, smpf = p) -- correct
        # It counts m = q > p (prime) giving n = pq (composite, smpf = p) -- correct
        # It counts m = p^2 (composite, smpf = p >= p) giving n = p^3 -- correct
        # Good. But do we miss anything? n = pk where smpf(k) >= p and k >= 2 and pk <= N.
        # That's exactly our set. Looks complete.

        # Actually there's a subtlety for multi-level: n = p^2 * q has smpf = p,
        # and is counted via m = p*q with smpf(m) = p >= p. Yes, correct.

        # So total composite smpf contribution for prime p:
        vp = N // p
        if vp >= 2:
            cnt_composites = S0c[vp] - p_count
            # But we also need to count contributions from higher powers:
            # n = p^a * m where a >= 1, smpf(m) >= p, m >= 1, and the FIRST p is what we want.
            # Actually the above cnt_composites already handles all composites with smpf = p.
            # For n = p^2, m = p, included. For n = p^3, m = p^2, included (smpf(p^2) = p >= p). Yes.
            result = (result + p % MOD * (cnt_composites % MOD)) % MOD

        # Now sieve: remove composites with smpf = p from S0c
        p2 = p * p
        for v in reversed(vals):
            if v < p2:
                break
            S0c[v] -= (S0c[v // p] - p_count)

    return result % MOD


print(solve(10**12))
