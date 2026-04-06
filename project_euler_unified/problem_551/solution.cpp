#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 551: Sum of Digits Sequence
 * a_0 = 1, a_n = a_{n-1} + digitsum(a_{n-1})
 * Find a_{10^15}.
 *
 * Approach: Block-based acceleration.
 * We split the number into blocks of digits and precompute jump tables.
 * For a given low-order block value, we know how many steps until a carry
 * propagates out of that block, and the total digit-sum contribution.
 *
 * We use a multi-level approach with blocks of B digits each.
 */

typedef long long ll;
typedef unsigned long long ull;

// We'll represent the current number as an array of digits (or blocks)
// and use a recursive approach to jump through steps.

// digit sum of a number
int digitsum(ll n) {
    int s = 0;
    while (n > 0) {
        s += n % 10;
        n /= 10;
    }
    return s;
}

int main() {
    ios_base::sync_with_stdio(false);

    const ll TARGET = 1000000000000000LL; // 10^15

    // Simple approach using block acceleration:
    // We process digit-by-digit with jump tables.
    // For each "level" (ones, tens, hundreds, etc.), we precompute
    // how many steps and how much total digit-sum increment happens
    // before a carry propagates to the next level.

    // Level 0: single digit d (0-9)
    // steps[0][d] = number of steps until d rolls over past 9
    // delta[0][d] = total increment (sum of digit sums) over those steps
    // But this is tricky because the digit sum depends on ALL digits.

    // Better approach: The digit sum contribution from digit position k is
    // just that digit's value. So digitsum(a) = sum of all digits.
    // When we add digitsum(a) to a, the low-order digits change,
    // and carries may propagate.

    // We use the following approach:
    // Maintain the number as a vector of digits.
    // For each "block" (individual digit), precompute:
    //   Given digit value d and a "carry-in" rate c (the digit sum of higher digits),
    //   how many steps until this digit overflows (causes carry to next position)?

    // Actually, let's use a well-known approach for this problem:
    // We define for each suffix length L:
    //   jump(L, suffix_value) = (steps, new_suffix_value, carry_count)
    // where steps = how many a-sequence steps until carry propagates past position L,
    // and carry_count = how many carries propagate (0 or 1 per step group).

    // For this problem, we use a simpler direct computation with jump tables.
    // Reference approach: process in blocks where we track the bottom k digits.

    // Actually, the most practical approach for this problem:
    // The number has at most ~17 digits for a_{10^15}.
    // We process one digit position at a time from least significant.
    //
    // For position p (0-indexed from right), we track:
    //   - how many steps we can take before digit at position p overflows
    //   - the accumulated effect on lower digits
    //
    // Since the digit sum = sum of all digits, and adding it affects the number:
    // The key insight is that when we add the digit sum, the contribution from
    // digits at positions >= p+1 is constant (they don't change until carry reaches them).

    // Let's use a direct simulation with memoization per digit level.
    // We'll represent the state as (bottom_digits_value, higher_digitsum)
    // At each level k, we merge steps until the k-th digit changes.

    // Practical implementation:
    // We keep the number in an array of 20 digits.
    // For each level from 0 to 19, we precompute (using lower-level tables):
    //   Given (lower_value, upper_dsum), compute (steps, new_lower_value, carries)
    //   where carries = number of carries propagated past this level.

    // Level-0: digit d, upper_dsum = S (sum of all other digits)
    //   Each step adds (d + S) to the number. This changes d to (d + (d+S)) mod 10
    //   with carry. But S also includes lower... wait, d IS the lowest digit at level 0.
    //   Actually for level 0, the full digit sum includes d and all higher digits (sum = S+d).
    //   Adding S+d to the number: the ones digit d becomes (d + (S+d)) mod 10 = (2d+S) mod 10.
    //   Carry out = (d + (S+d)) / 10 = (2d+S) / 10 which is 0 or 1.
    //   But wait, the NEXT step, d has changed, and also potentially higher digits changed
    //   (from carry). This gets complicated.

    // Simpler: direct simulation with acceleration.
    // At each step, compute digitsum, add to number.
    // But 10^15 steps is too many.
    //
    // We use the "carry-skip" technique:
    // The digit sum is at most 9*17 = 153.
    // So a_{n+1} - a_n <= 153.
    // For the lowest digit to "cycle" through 0-9, it takes at least 1 step.
    //
    // Multi-level jumping:
    // Level L processes the bottom L+1 digits.
    // jump_L(bottom_value, upper_dsum) returns (steps, new_bottom_value, total_carry)
    //   where steps = how many sequence steps until a carry propagates past digit L.
    //   new_bottom_value = value of bottom L+1 digits after those steps
    //   total_carry = total carry out of digit L (always 1 in the "until overflow" version)

    // For level 0 (ones digit only):
    //   d = current ones digit, S = digit sum of all other digits
    //   Each step: full digit sum = d + S + (adjustment from carries below, but level 0 has no below)
    //   new_d = d + (d + S), with carries.
    //   We step until the ones digit wraps around (causes carry).
    //   But each step changes d, and S is constant (no carry has reached higher digits yet).
    //   Steps until carry: we accumulate d = d, then d' = (2d + S) mod 10, etc.
    //   until the sum exceeds 10.

    // Let's implement this properly.

    // Represent number as array of 20 digits (index 0 = ones)
    int digits[20] = {};
    digits[0] = 1; // a_0 = 1
    int num_digits = 1;

    // For a given bottom-k digits state and upper digit sum, compute forward
    // until carry propagates past digit k.
    // We use recursive tables.

    // Cache for level k: map from (bottom_value, upper_dsum) -> (steps, new_bottom, carry)
    // But the state space is huge for large k.

    // Alternative: process level by level without full memoization.
    // For level 0: given d (ones digit) and S (sum of digits at positions >= 1),
    //   step forward until carry occurs. Return (steps, new_d, carry=1).
    // For level 1: given (d1, d0) and S (sum of digits at positions >= 2),
    //   repeatedly call level-0 jumps, accumulating steps.
    //   After each level-0 jump, d1 gets a carry added.
    //   Continue until d1 itself overflows.
    //   Return (total_steps, new_d1_d0, carry=1).
    // For level k: similar, call level-(k-1) repeatedly until digit k overflows.

    // This is the standard approach. The number of calls at each level is at most 10
    // (digit goes from current value to overflow), and there are ~17 levels.
    // Total work: 10^17 -- still too much!

    // We need memoization. State at level k: (bottom_k_digits, upper_dsum).
    // bottom_k_digits can be up to 10^k, upper_dsum up to 9*(20-k) ~ 153.
    // For k small (0,1,2), this is feasible. For larger k, we note that
    // upper_dsum is bounded by ~153, so table size = 10^k * 153.
    // For k up to 6: 10^6 * 153 ~ 1.5 * 10^8 entries -- borderline.
    // For k up to 4: 10^4 * 153 ~ 1.5 * 10^6 entries -- fine.

    // Better: use variable-size blocks. E.g., blocks of 4 digits.
    // Level 0: 4 lowest digits (0-9999), upper_dsum (0-144) -> 10000*145 ~ 1.45M entries
    // Level 1: next 4 digits... but state is (8 bottom digits, upper_dsum)
    //   = 10^8 * 100 -- too large.

    // Actually, at level 1, we don't store all 8 bottom digits.
    // We store just the level-1 block (4 digits, positions 4-7) and upper_dsum.
    // After level-1 jump, the level-0 block may have changed, but we only need
    // to track the level-1 block state and how many carries propagated.

    // Wait, I think the standard approach is:
    // Process each digit position independently, from low to high.
    // At digit position p, we know the current digit d_p and S = sum of digits above p.
    // We call the level-(p-1) jump to process all changes below digit p.
    // Each such call might or might not produce a carry into digit p.
    // We accumulate until digit p overflows, then return.

    // With memoization per level, the state is (d_p, S) where S = upper_dsum
    // and the result includes how the lower digits end up. But the lower digit
    // state matters for correct continuation. Hmm.

    // Actually a cleaner approach:
    // For each level k (digit position), we define:
    //   advance(k, digits[], target_steps)
    // which advances the sequence by target_steps, modifying digits[].
    // At level k, we use the level-(k-1) function to advance until digit k overflows.
    // The key insight: we can MEMOIZE the level-k transition because it depends only on
    // digits[0..k] and sum(digits[k+1..]), and the output is the new digits[0..k] and carry.

    // For this to work, we need: state = (digits[0..k], upper_sum) -> (steps, new digits[0..k], carry)
    // Size of state space = 10^(k+1) * max_upper_sum.
    // For k=3 (4 digits), max_upper_sum = 9*16 = 144, state space = 10000 * 145 = 1.45M
    // For k=7 (8 digits), state space = 10^8 * 100 -- too large.

    // So we use a two-level hierarchy:
    // Level A: 4 low digits, memoized with upper_dsum. Table: ~1.45M entries.
    // Level B: each of the higher digits, processed with level-A jumps.
    //   No memoization needed at level B since there are few (~13) higher digits,
    //   each going through at most 10 transitions, each requiring a level-A lookup chain.

    // Actually, the simplest efficient approach for this problem uses a
    // different technique: carry propagation tracking.

    // Let me implement the digit-by-digit level approach with memoization.
    // State: (value of bottom L+1 digits, digit sum of upper digits)
    // For L = 0..3 (4 digits), memoized.
    // For L = 4..19, process digit by digit using L=3 as the base.

    // Represent number in base-10, 20 digits max
    // digits[0] = ones, digits[1] = tens, etc.

    // Memoization table for level 3 (bottom 4 digits, values 0..9999, upper_dsum 0..160)
    const int BLOCK = 4;
    const int BLOCK_MOD = 10000; // 10^BLOCK
    const int MAX_UDSUM = 9 * 17 + 1; // max upper digit sum + 1

    // For block of bottom BLOCK digits with value V and upper digit sum U:
    // step forward until carry propagates out of these BLOCK digits.
    // Return: (steps_taken, new_V, carry_out)
    // carry_out is always 1 (we stop when overflow happens)
    // But there might be MULTIPLE overflows... no, we stop at first carry out.

    // memo[V][U] = (steps, new_V)
    // We compute this on-the-fly and cache it.

    struct Entry {
        ll steps;
        int new_val;
    };

    // Table: BLOCK_MOD * MAX_UDSUM
    static Entry memo[10000][160];
    static bool computed[10000][160];
    memset(computed, 0, sizeof(computed));

    // Function to compute one entry
    auto compute_entry = [&](int V, int U) -> Entry {
        // Simulate: bottom BLOCK digits have value V, upper digit sum is U.
        // Full digit sum = digitsum(V) + U.
        // Add digit sum to V. If V >= BLOCK_MOD, carry out, return.
        ll steps = 0;
        int v = V;
        while (true) {
            int ds = digitsum(v) + U;
            int nv = v + ds;
            steps++;
            if (nv >= BLOCK_MOD) {
                // Carry out. New value of bottom digits = nv - BLOCK_MOD
                return {steps, nv - BLOCK_MOD};
            }
            v = nv;
        }
    };

    auto get_entry = [&](int V, int U) -> Entry {
        if (U >= 160) {
            // Shouldn't happen for our problem, but handle gracefully
            return compute_entry(V, U);
        }
        if (!computed[V][U]) {
            memo[V][U] = compute_entry(V, U);
            computed[V][U] = true;
        }
        return memo[V][U];
    };

    // Now process the higher digits.
    // We have digits[0..19], with digits[0..BLOCK-1] forming the bottom block.
    // digits[BLOCK..19] are the upper digits.
    // We advance until reaching TARGET steps.

    ll steps_done = 0;

    while (steps_done < TARGET) {
        // Compute upper digit sum
        int upper_dsum = 0;
        for (int i = BLOCK; i < 20; i++) upper_dsum += digits[i];

        // Get bottom block value
        int bottom_val = 0;
        {
            int mul = 1;
            for (int i = 0; i < BLOCK; i++) {
                bottom_val += digits[i] * mul;
                mul *= 10;
            }
        }

        // Compute entry: how many steps until carry out of bottom block
        Entry e = get_entry(bottom_val, upper_dsum);

        if (steps_done + e.steps <= TARGET) {
            // Apply full block jump
            steps_done += e.steps;

            // Update bottom digits
            int nv = e.new_val;
            for (int i = 0; i < BLOCK; i++) {
                digits[i] = nv % 10;
                nv /= 10;
            }

            // Propagate carry into upper digits
            int carry = 1;
            for (int i = BLOCK; i < 20 && carry; i++) {
                digits[i] += carry;
                if (digits[i] >= 10) {
                    digits[i] -= 10;
                    carry = 1;
                } else {
                    carry = 0;
                }
            }
        } else {
            // Can't do full block jump. Do one step at a time.
            ll remaining = TARGET - steps_done;
            // Single step
            int ds = 0;
            for (int i = 0; i < 20; i++) ds += digits[i];
            // Add ds to the number
            int carry = ds;
            for (int i = 0; i < 20; i++) {
                digits[i] += carry;
                carry = digits[i] / 10;
                digits[i] %= 10;
                if (carry == 0) break;
            }
            steps_done++;
        }
    }

    // Print the result
    // Find highest non-zero digit
    int top = 19;
    while (top > 0 && digits[top] == 0) top--;

    // Print number
    for (int i = top; i >= 0; i--) cout << digits[i];
    cout << endl;

    return 0;
}
