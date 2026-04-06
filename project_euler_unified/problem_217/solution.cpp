#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 217: Balanced Numbers
 *
 * Sum of all balanced numbers below 10^47, mod 3^15.
 *
 * A k-digit number is balanced if the digit sum of the first floor(k/2)
 * digits equals the digit sum of the last floor(k/2) digits.
 *
 * We compute for each digit length k from 1 to 47.
 */

const long long MOD = 14348907LL; // 3^15

int main() {
    // Maximum half-length
    int maxH = 23; // floor(47/2)
    int maxSum = 9 * maxH; // max digit sum for h digits

    // For each h, compute:
    // cnt[h][s] = count of h-digit strings (with leading zeros) with digit sum s
    // sm[h][s]  = sum of values of such strings
    // cntL[h][s] = count of h-digit numbers (no leading zero) with digit sum s
    // smL[h][s]  = sum of such numbers

    // We'll compute these incrementally.
    // cnt[0][0] = 1, sm[0][0] = 0
    // For h digits, adding a new digit d at the most significant position:
    // Actually let's build from the least significant digit.
    // A string of h digits: d_{h-1} d_{h-2} ... d_0
    // Value = sum(d_i * 10^i), digit sum = sum(d_i).

    // DP: after placing i digits (positions 0..i-1), with digit sum s:
    // cnt_dp[i][s], sum_dp[i][s]

    // Transition: add digit d at position i:
    // cnt_dp[i+1][s+d] += cnt_dp[i][s]
    // sum_dp[i+1][s+d] += sum_dp[i][s] + d * 10^i * cnt_dp[i][s]

    // This gives us cnt(h, s) = cnt_dp[h][s] and sm(h, s) = sum_dp[h][s]
    // for h-digit strings with possible leading zeros.

    // For left halves (leading digit >= 1 at position h-1):
    // cntL(h, s) = cnt(h, s) - cnt(h-1, s)  [subtract those with d_{h-1}=0]
    // smL(h, s) = sm(h, s) - sm(h-1, s)

    // Actually more carefully: h-digit strings with leading zero are exactly
    // 0 followed by (h-1)-digit string. So cnt(h,s) with leading zero = cnt(h-1,s),
    // and sum with leading zero = sm(h-1,s) (value is same since leading 0 adds nothing).

    // So cntL(h,s) = cnt(h,s) - cnt(h-1,s), smL(h,s) = sm(h,s) - sm(h-1,s).

    // We need arrays up to h=23, sum up to 9*23=207.
    int MAXS = maxSum + 1;

    // cnt_dp[s] and sum_dp[s] for current h
    vector<vector<long long>> cnt(maxH + 1, vector<long long>(MAXS, 0));
    vector<vector<long long>> sm(maxH + 1, vector<long long>(MAXS, 0));

    cnt[0][0] = 1;
    sm[0][0] = 0;

    // pow10[i] = 10^i mod MOD
    vector<long long> pow10(48, 1);
    for (int i = 1; i < 48; i++) pow10[i] = pow10[i-1] * 10 % MOD;

    for (int i = 0; i < maxH; i++) {
        // Add digit d at position i (0-indexed from right)
        for (int s = 0; s < MAXS; s++) {
            if (cnt[i][s] == 0 && sm[i][s] == 0) continue;
            for (int d = 0; d <= 9; d++) {
                int ns = s + d;
                if (ns >= MAXS) break;
                cnt[i+1][ns] = (cnt[i+1][ns] + cnt[i][s]) % MOD;
                sm[i+1][ns] = (sm[i+1][ns] + sm[i][s] + (long long)d % MOD * pow10[i] % MOD * cnt[i][s]) % MOD;
            }
        }
    }

    long long total = 0;

    // 1-digit numbers (k=1): all are balanced (h=0, both halves empty).
    // Sum = 1+2+...+9 = 45
    total = 45 % MOD;

    for (int k = 2; k <= 47; k++) {
        int h = k / 2;
        bool odd = (k % 2 == 1);

        // For even k=2h: sum = sum_s [ smL(h,s) * 10^h * cnt(h,s) + cntL(h,s) * sm(h,s) ]
        // For odd k=2h+1: sum = sum_s [ 10 * smL(h,s) * 10^(h+1) * cnt(h,s)
        //                              + 45 * 10^h * cntL(h,s) * cnt(h,s)
        //                              + 10 * cntL(h,s) * sm(h,s) ]

        long long contribution = 0;

        for (int s = 1; s <= 9 * h; s++) { // s >= 1 since left half has no leading zero => digit sum >= 1
            long long cntH = cnt[h][s];
            long long smH = sm[h][s];
            long long cntL_s = (cnt[h][s] - (h >= 1 ? cnt[h-1][s] : 0) % MOD + MOD) % MOD;
            long long smL_s = (sm[h][s] - (h >= 1 ? sm[h-1][s] : 0) % MOD + MOD) % MOD;

            if (!odd) {
                // even: k = 2h
                long long term = (smL_s % MOD * pow10[h] % MOD * cntH % MOD
                                  + cntL_s % MOD * smH % MOD) % MOD;
                contribution = (contribution + term) % MOD;
            } else {
                // odd: k = 2h+1
                long long term = (10 * smL_s % MOD * pow10[h+1] % MOD * cntH % MOD) % MOD;
                term = (term + 45 * pow10[h] % MOD * cntL_s % MOD * cntH % MOD) % MOD;
                term = (term + 10 * cntL_s % MOD * smH % MOD) % MOD;
                contribution = (contribution + term) % MOD;
            }
        }

        total = (total + contribution) % MOD;
    }

    cout << total << endl;
    return 0;
}
