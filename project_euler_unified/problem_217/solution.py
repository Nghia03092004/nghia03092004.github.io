"""
Problem 217: Balanced Numbers

Sum of all balanced numbers below 10^47, mod 3^15 = 14348907.

A k-digit number is balanced if the digit sum of the first floor(k/2)
digits equals the digit sum of the last floor(k/2) digits.
For odd k, the middle digit is ignored.

Answer: 6273134
"""

def solve():
    MOD = 3**15  # 14348907
    MAX_K = 47
    MAX_H = MAX_K // 2  # 23
    MAX_S = 9 * MAX_H   # 207

    # cnt[h][s] = count of h-digit strings (leading zeros OK) with digit sum s
    # sm[h][s]  = sum of values of such strings
    cnt = [[0] * (MAX_S + 1) for _ in range(MAX_H + 1)]
    sm = [[0] * (MAX_S + 1) for _ in range(MAX_H + 1)]

    cnt[0][0] = 1

    pow10 = [1] * 48
    for i in range(1, 48):
        pow10[i] = pow10[i-1] * 10 % MOD

    for i in range(MAX_H):
        for s in range(MAX_S + 1):
            if cnt[i][s] == 0 and sm[i][s] == 0:
                continue
            for d in range(10):
                ns = s + d
                if ns > MAX_S:
                    break
                cnt[i+1][ns] = (cnt[i+1][ns] + cnt[i][s]) % MOD
                sm[i+1][ns] = (sm[i+1][ns] + sm[i][s] + d * pow10[i] * cnt[i][s]) % MOD

    # 1-digit numbers: all balanced, sum = 45
    total = 45 % MOD

    for k in range(2, MAX_K + 1):
        h = k // 2
        odd = (k % 2 == 1)

        contribution = 0

        for s in range(1, 9 * h + 1):
            cntH = cnt[h][s]
            smH = sm[h][s]
            cntL = (cnt[h][s] - (cnt[h-1][s] if h >= 1 else 0)) % MOD
            smL = (sm[h][s] - (sm[h-1][s] if h >= 1 else 0)) % MOD

            if not odd:
                term = (smL * pow10[h] % MOD * cntH + cntL * smH) % MOD
            else:
                term = (10 * smL % MOD * pow10[h+1] % MOD * cntH) % MOD
                term = (term + 45 * pow10[h] % MOD * cntL % MOD * cntH) % MOD
                term = (term + 10 * cntL % MOD * smH) % MOD

            contribution = (contribution + term) % MOD

        total = (total + contribution) % MOD

    print(total % MOD)

if __name__ == "__main__":
    solve()
