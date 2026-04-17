#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 168: Number Rotations
 *
 * Find all n with 10 <= n < 10^100 such that rotating the last digit to
 * the front gives a multiple of n. Sum the last 5 digits of all such n.
 *
 * For each multiplier k (1..9) and last digit r (1..9), we generate digits
 * right-to-left: starting with digit r and carry 0, next digit = (k*prev_digit + carry) % 10,
 * new carry = (k*prev_digit + carry) / 10. After d steps, if we return to
 * (digit=r, carry=0), we have a valid d-digit number. Also need first digit != 0.
 */

int main(){
    const long long MOD = 100000;
    long long total = 0;

    for(int k = 1; k <= 9; k++){
        for(int r = 1; r <= 9; r++){
            // Generate digits right to left
            // digits[0] = rightmost digit = r (this is a_d)
            // digits[1] = a_{d-1}, etc.
            // digits[d-1] = a_1 (leftmost)
            // After d steps of generation, we need current digit = r and carry = 0
            // Also a_1 (= digits[d-1]) must be nonzero

            int digit = r;
            int carry = 0;
            // Store last 5 digits as we go (tracking powers of 10 mod 100000)
            // The digits are generated right to left: digit 0 is units, digit 1 is tens, etc.
            // last5 of n = sum of digit[i] * 10^i for i=0..4

            long long last5 = 0;
            long long pow10 = 1; // 10^i mod MOD

            for(int step = 0; step < 100; step++){
                // Current digit goes at position 'step' (from the right)
                if(step < 5){
                    last5 = (last5 + (long long)digit * pow10) % MOD;
                    pow10 = (pow10 * 10) % MOD;
                }

                // Check if we've completed a valid cycle (step+1 digits = d)
                // Need d >= 2, and after generating all d digits, the NEXT step
                // would produce (r, 0) again.
                // Actually: after placing digit at position 'step', compute next:
                int next_digit = (k * digit + carry) % 10;
                int next_carry = (k * digit + carry) / 10;

                int d = step + 1;
                if(d >= 2 && next_digit == r && next_carry == 0){
                    // Valid! The number has d digits.
                    // Check that the leftmost digit (digits[d-1] = current 'digit') is nonzero
                    if(digit != 0){
                        total = (total + last5) % MOD;
                    }
                }

                digit = next_digit;
                carry = next_carry;
            }
        }
    }

    cout << total << endl;
    return 0;
}
