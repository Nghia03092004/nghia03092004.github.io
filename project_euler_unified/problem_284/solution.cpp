#include <bits/stdc++.h>
using namespace std;

// Big number in base 14, stored LSB first
const int BASE = 14;
const int MAXD = 10000;

int seven[MAXD], eight_arr[MAXD];

// Multiply a[0..n-1] * b[0..n-1], keep first 'keep' digits, store in res
void mulLow(const int* a, const int* b, int* res, int na, int nb, int keep) {
    memset(res, 0, keep * sizeof(int));
    for (int i = 0; i < min(na, keep); i++) {
        if (a[i] == 0) continue;
        long long carry = 0;
        int jmax = min(nb, keep - i);
        for (int j = 0; j < jmax; j++) {
            carry += (long long)res[i+j] + (long long)a[i] * b[j];
            res[i+j] = carry % BASE;
            carry /= BASE;
        }
        for (int j = jmax; i+j < keep && carry > 0; j++) {
            carry += res[i+j];
            res[i+j] = carry % BASE;
            carry /= BASE;
        }
    }
}

// Multiply a[0..n-1] by scalar s, keep first 'keep' digits, store in res
void mulScalar(const int* a, int s, int* res, int na, int keep) {
    long long carry = 0;
    for (int i = 0; i < keep; i++) {
        if (i < na) carry += (long long)a[i] * s;
        res[i] = carry % BASE;
        carry /= BASE;
    }
}

// res = a - b mod 14^keep (assuming result is non-negative mod 14^keep)
void subMod(const int* a, const int* b, int* res, int keep) {
    int borrow = 0;
    for (int i = 0; i < keep; i++) {
        int diff = a[i] - b[i] - borrow;
        if (diff < 0) { diff += BASE; borrow = 1; }
        else borrow = 0;
        res[i] = diff;
    }
}

int tmp1[MAXD], tmp2[MAXD], tmp3[MAXD], tmp4[MAXD];

int main() {
    // Start with seven[0] = 7
    memset(seven, 0, sizeof(seven));
    seven[0] = 7;

    // Fast doubling: n' = (3n^2 - 2n^3) mod 14^target
    int cur = 1;
    while (cur < MAXD) {
        int target = min(cur * 2, MAXD);

        // n^2
        mulLow(seven, seven, tmp1, cur, cur, target);
        // n^3
        mulLow(tmp1, seven, tmp2, target, cur, target);
        // 3*n^2
        mulScalar(tmp1, 3, tmp3, target, target);
        // 2*n^3
        mulScalar(tmp2, 2, tmp4, target, target);
        // result = 3n^2 - 2n^3 mod 14^target
        subMod(tmp3, tmp4, seven, target);

        cur = target;
    }

    // Compute eight = (1 - seven) mod 14^MAXD
    memset(eight_arr, 0, sizeof(eight_arr));
    int borrow = 0;
    for (int i = 0; i < MAXD; i++) {
        int val = (i == 0 ? 1 : 0) - seven[i] - borrow;
        if (val < 0) { val += BASE; borrow = 1; }
        else borrow = 0;
        eight_arr[i] = val;
    }

    // Verify
    assert(seven[0] == 7);
    assert(eight_arr[0] == 8);

    // For each digit position i (0-indexed), the digit seven[i] appears in all
    // steady squares of length k for k = i+1, i+2, ..., MAXD.
    // That's (MAXD - i) steady squares.
    // Similarly for eight_arr[i].
    //
    // But we also need to handle leading zeros:
    // A "k-digit steady square" means the number has exactly k digits in base 14.
    // The truncation of 'seven' to k digits might have seven[k-1] = 0, making it
    // less than k digits. In that case, it's NOT a k-digit steady square.
    //
    // Similarly, we need to subtract digit contributions when a truncation has leading zero.

    // For the "1" steady square: it's 1-digit. Its digit sum contribution is 1.

    long long digit_sum = 1;  // for the number 1 (1-digit steady square)

    // For seven chain: for each k from 1 to MAXD, if seven[k-1] != 0,
    // we have a k-digit steady square whose digits are seven[0..k-1].
    // Digit sum = sum of seven[0..k-1].
    //
    // Efficient approach: maintain running digit sum.
    // sum_of_digits(k) = sum_of_digits(k-1) + seven[k-1]
    // If seven[k-1] != 0, add sum_of_digits(k) to total.
    // If seven[k-1] == 0, this truncation is not a k-digit number. Skip.
    //
    // Actually, even with leading zero at position k-1, the number could still have
    // fewer digits but might still be valid for a smaller k. But we already counted it
    // at the smaller k. So we just skip.
    //
    // Wait, actually: the solutions to x^2 ≡ x mod 14^k include x = seven_k.
    // If seven_k < 14^(k-1), then seven_k has fewer than k digits and is also
    // a solution to x^2 ≡ x mod 14^(k-1). But it was already counted for k-1 digits.
    // As a k-digit steady square, it doesn't qualify. So we skip when seven[k-1] = 0.

    long long running_sum_s = 0, running_sum_e = 0;
    for (int k = 1; k <= MAXD; k++) {
        running_sum_s += seven[k-1];
        running_sum_e += eight_arr[k-1];

        if (seven[k-1] != 0) {
            digit_sum += running_sum_s;
        }
        if (eight_arr[k-1] != 0) {
            digit_sum += running_sum_e;
        }
    }

    // Convert digit_sum to base 14
    string result;
    const char digits[] = "0123456789abcd";
    long long tmp_val = digit_sum;
    if (tmp_val == 0) result = "0";
    while (tmp_val > 0) {
        result = digits[tmp_val % 14] + result;
        tmp_val /= 14;
    }
    cout << result << endl;

    return 0;
}
