#include <bits/stdc++.h>
using namespace std;

int main(){
    // Each digit 0-9 appears exactly twice in a double pandigital number (20 digits).
    // We choose c[d] in {0,1,2} copies of digit d to place at odd positions (10 positions).
    // Sum of c[d] must be 10.
    // Divisibility by 11: O - E ≡ 0 (mod 11) where O = sum of odd-position digits.
    // O + E = 90, so O ≡ 1 (mod 11).

    long long fact[11];
    fact[0] = 1;
    for(int i = 1; i <= 10; i++) fact[i] = fact[i-1] * i;

    // We enumerate choices c[0..9] in {0,1,2}, sum = 10
    // For each valid choice, count = (10! / prod c[d]!) * (10! / prod (2-c[d])!)
    // Then subtract leading-zero cases.

    long long total = 0;
    long long leading_zero = 0;

    // Use recursion or iterative enumeration
    // c[d] in {0,1,2}, 10 digits -> 3^10 = 59049 combos

    int c[10];
    // iterate over all 3^10 combinations
    for(int mask = 0; mask < 59049; mask++){
        int tmp = mask;
        int sum_c = 0;
        int sum_val = 0;
        bool valid = true;
        for(int d = 0; d < 10; d++){
            c[d] = tmp % 3;
            tmp /= 3;
            sum_c += c[d];
            sum_val += d * c[d];
        }
        if(sum_c != 10) continue;
        if(sum_val % 11 != 1) continue;

        // Count permutations for odd positions: 10! / prod(c[d]!)
        long long odd_perms = fact[10];
        for(int d = 0; d < 10; d++) odd_perms /= fact[c[d]];

        // Count permutations for even positions: 10! / prod((2-c[d])!)
        long long even_perms = fact[10];
        for(int d = 0; d < 10; d++) even_perms /= fact[2 - c[d]];

        long long ways = odd_perms * even_perms;
        total += ways;

        // Leading zero: digit 0 is at position 1 (odd position).
        // If c[0] >= 1, fix one 0 at position 1, remaining 9 odd positions
        // have the other digits. The count of such arrangements:
        // (c[0]/10) * odd_perms * even_perms ... but more precisely:
        // Among odd_perms arrangements, fraction c[0]/10 have 0 in the first spot.
        if(c[0] >= 1){
            // Number of odd arrangements with 0 in first position:
            // Fix 0 there. Remaining 9 positions: (9! / ((c[0]-1)! * prod_{d>0} c[d]!))
            long long odd_lz = fact[9];
            odd_lz /= fact[c[0] - 1];
            for(int d = 1; d < 10; d++) odd_lz /= fact[c[d]];
            leading_zero += odd_lz * even_perms;
        }
    }

    cout << total - leading_zero << endl;
    return 0;
}
