#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 240: Top Dice
 *
 * How many ways can 20 twelve-sided dice be rolled so that the top 10 sum to 70?
 *
 * Approach: Enumerate sorted configurations (frequency vectors) by recursing
 * on face values from 12 down to 1, tracking top-10 sum.
 * For each valid configuration, add the multinomial coefficient 20!/prod(n_i!).
 */

typedef long long ll;
typedef unsigned long long ull;

const int NUM_DICE = 20;
const int NUM_SIDES = 12;
const int TOP_COUNT = 10;
const int TARGET = 70;

ll fact[NUM_DICE + 1];
ull total_answer;

void assign_bottom(int face, int dice_left, ll mult_denom) {
    if (dice_left == 0) {
        total_answer += fact[NUM_DICE] / mult_denom;
        return;
    }
    if (face == 0) return;
    if (face == 1) {
        total_answer += fact[NUM_DICE] / (mult_denom * fact[dice_left]);
        return;
    }
    for (int count = 0; count <= dice_left; count++) {
        assign_bottom(face - 1, dice_left - count, mult_denom * fact[count]);
    }
}

void recurse(int face, int dice_left, int top_left, int sum_left, ll mult_denom) {
    if (top_left == 0 && sum_left != 0) return;
    if (top_left == 0 && sum_left == 0) {
        assign_bottom(face, dice_left, mult_denom);
        return;
    }
    if (face == 0) {
        if (dice_left == 0 && top_left == 0 && sum_left == 0)
            total_answer += fact[NUM_DICE] / mult_denom;
        return;
    }
    if (dice_left < top_left) return;
    if ((ll)top_left * face < sum_left) return;
    if (sum_left < top_left) return;

    for (int count = 0; count <= dice_left; count++) {
        int top_from = min(count, top_left);
        int new_top = top_left - top_from;
        int new_sum = sum_left - top_from * face;
        if (new_sum < 0) break;

        recurse(face - 1, dice_left - count, new_top, new_sum,
                mult_denom * fact[count]);
    }
}

int main(){
    fact[0] = 1;
    for (int i = 1; i <= NUM_DICE; i++)
        fact[i] = fact[i-1] * i;

    total_answer = 0;
    recurse(NUM_SIDES, NUM_DICE, TOP_COUNT, TARGET, 1);

    cout << total_answer << endl;
    return 0;
}
