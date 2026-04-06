#include <bits/stdc++.h>
using namespace std;

// Macro-step simulation of Conway's PRIMEGAME FRACTRAN program.
// Counts the number of FRACTRAN iterations to produce the 10001st prime.
//
// The FRACTRAN program: 17/91, 78/85, 19/51, 23/38, 29/33, 77/29,
//   95/23, 77/19, 1/17, 11/13, 13/11, 15/2, 1/7, 55/1
// Starting with n=2.
//
// The register machine uses exponents of primes 2,3,5,7,11,13,17,19,23,29
// as registers a,b,c,d,e,f,g,h,i,j.
//
// High-level: processes candidates m=1,2,3,... via trial division.
// Each candidate is tested by repeated subtraction rounds.
//
// State at round entry: a_acc, B=b, C=c, D=d, e=1 (others 0).
// A round:
//   f5/f6 x B:     2B steps.  D += B, B = 0.
//   f11:           1 step.
//   f1/f2 x k:     2k steps.  k = min(D, C). D-=k, C-=k, a_acc+=k, B+=k.
//
//   If D==0: f10: 1 step. Continue next round.
//   If D>0 and C==0 and B>0: f1,f3,f4/f7 x a_acc,f8. Move to next divisor.
//   If D>0 and C==0 and B==0: f1,f9,[f13 x D]. End of candidate.
//     Prime if D==0 after f1 (i.e., old D+B==1). Composite otherwise.

int main() {
    long long total_steps = 0;
    int primes_found = 0;
    int target = 10001;

    // Initial: a=1. f12 x1: b=1,c=1. f14: c=2,e=1. 2 steps.
    total_steps = 2;
    long long a_acc = 0, B = 1, C = 2, D = 0;

    while (primes_found < target) {
        // Phase 1: f5/f6 x B
        total_steps += 2 * B;
        D += B;
        B = 0;

        // Phase 2: f11
        total_steps += 1;

        // Phase 3: f1/f2 x k
        long long k = (D < C) ? D : C;
        total_steps += 2 * k;
        D -= k;
        C -= k;
        a_acc += k;
        B = k;

        if (D == 0) {
            // Case A: f10
            total_steps += 1;
        } else if (B > 0) {
            // Case B1: C == 0, B > 0. Transition to next divisor.
            // f1: D-=1. f3: B-=1. f4/f7 x a_acc. f8: D+=1.
            total_steps += 1 + 1 + 2 * a_acc + 1;
            D -= 1;
            B -= 1;
            C = a_acc;
            D += 1;
            a_acc = 0;
        } else {
            // Case B2: C == 0, B == 0. End of candidate.
            // f1: D-=1. f9: 1. f13 x D.
            D -= 1;
            total_steps += 1 + 1 + D;

            bool is_prime = (D == 0);
            D = 0;

            if (is_prime) {
                primes_found++;
                if (primes_found == target) break;
            }

            // Load next candidate: f12 x a_acc + f14.
            total_steps += a_acc + 1;
            B = a_acc;
            C = a_acc + 1;
            D = 0;
            a_acc = 0;
        }
    }

    printf("%lld\n", total_steps);
    return 0;
}
