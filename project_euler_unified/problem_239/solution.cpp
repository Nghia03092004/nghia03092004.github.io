#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 239: Twenty-two Foolish Primes
 *
 * Random permutation of {1,...,100}. Probability that exactly 22 of 25 primes
 * are NOT in their natural position (i.e., exactly 3 are fixed).
 *
 * P = C(25,3) * sum_{k=0}^{22} (-1)^k * C(22,k) * (97-k)! / 100!
 *
 * We compute this using high-precision floating point (long double) or
 * by computing the fraction exactly using Python-style big integers.
 * Since C++ doesn't have built-in big integers, we use the telescoping
 * ratio approach.
 *
 * P = C(25,3) / (100*99*98) * sum_{k=0}^{22} (-1)^k * C(22,k) * (97-k)! / 97!
 *
 * Note: (97-k)!/97! = 1 / (97 * 96 * ... * (98-k)) for k >= 1, and 1 for k=0.
 *
 * Actually, let's compute:
 * P = C(25,3) * IE / 100!
 *   = 2300 * sum_{k=0}^{22} (-1)^k * C(22,k) * (97-k)! / 100!
 *   = 2300 * sum_{k=0}^{22} (-1)^k * C(22,k) / [100*99*98 * 97*96*...*(98-k+1)]
 *   Wait, this gets complicated.
 *
 * Simpler: compute sum S = sum_{k=0}^{22} (-1)^k * C(22,k) * prod_{j=98-k}^{97} 1/j ... no.
 *
 * Let's just compute term by term using long double.
 * Term_k = (-1)^k * C(22,k) * (97-k)! / 100!
 *        = (-1)^k * C(22,k) / [100*99*98 * 97*96*...*(98-k)]
 *        = (-1)^k * C(22,k) / [(100*99*98) * P(97, k)]
 * where P(97,k) = 97*96*...*(98-k) = 97!/(97-k)!.
 *
 * Wait: (97-k)!/100! = 1/(100*99*98*97*96*...*(98-k))
 * = 1/[(100*99*98) * 97*96*...*(98-k)]
 * = 1/[(100*99*98) * P(97, k)]
 * where P(97,0) = 1, P(97,k) = 97*96*...*(97-k+1).
 *
 * So Term_k = (-1)^k * C(22,k) / [970200 * P(97,k)]
 * and P = 2300 * sum(Term_k).
 */

int main(){
    // Compute using long double with careful summation
    // P = 2300 * sum_{k=0}^{22} (-1)^k * C(22,k) / [970200 * P(97,k)]

    // Precompute C(22,k)
    long double C22[23];
    C22[0] = 1;
    for (int k = 1; k <= 22; k++)
        C22[k] = C22[k-1] * (22-k+1) / k;

    // Compute each term
    long double sum = 0;
    long double P97k = 1; // P(97, 0) = 1
    long double prefix = 970200.0L; // 100*99*98

    for (int k = 0; k <= 22; k++) {
        long double term = C22[k] / (prefix * P97k);
        if (k % 2 == 1) term = -term;
        sum += term;

        // Update P97k for next iteration: P(97, k+1) = P(97,k) * (97-k)
        P97k *= (97 - k);
    }

    long double prob = 2300.0L * sum;

    // Print with 12 decimal places
    printf("%.12Lf\n", prob);

    return 0;
}
