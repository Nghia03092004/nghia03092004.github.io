#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler Problem 527: Randomized Binary Search
 *
 * R(n) = 2*H_n - 1, where H_n is the n-th harmonic number.
 * B(n) = average depth in the standard binary search tree.
 * Find R(10^10) - B(10^10) rounded to 8 decimal places.
 */

typedef long long ll;

// Compute B(n): average depth in standard binary search tree
// Uses recursive decomposition: root is at middle, left and right subtrees
// Returns total depth sum for a tree of size n
pair<double, ll> compute_B(ll n) {
    // Returns (sum_of_depths, n) where sum_of_depths = sum of d(i) for all i
    // d(i) is 1-indexed depth (root = 1)
    if (n == 0) return {0.0, 0};
    if (n == 1) return {1.0, 1};

    ll mid = (n + 1) / 2;  // 1-indexed position of root
    ll left_size = mid - 1;
    ll right_size = n - mid;

    auto [left_sum, ls] = compute_B(left_size);
    auto [right_sum, rs] = compute_B(right_size);

    // Root contributes depth 1
    // Left subtree: each node's depth increases by 1 -> add left_size
    // Right subtree: each node's depth increases by 1 -> add right_size
    double total = 1.0 + (left_sum + left_size) + (right_sum + right_size);
    return {total, n};
}

// Compute H_n using asymptotic expansion for large n
// H_n = ln(n) + gamma + 1/(2n) - 1/(12n^2) + 1/(120n^4) - 1/(252n^6) + ...
double harmonic_asymptotic(ll n) {
    double x = (double)n;
    double gamma = 0.57721566490153286;
    double result = log(x) + gamma;
    double inv_n = 1.0 / x;
    double inv_n2 = inv_n * inv_n;

    result += 0.5 * inv_n;
    result -= inv_n2 / 12.0;
    result += inv_n2 * inv_n2 / 120.0;
    result -= inv_n2 * inv_n2 * inv_n2 / 252.0;
    result += inv_n2 * inv_n2 * inv_n2 * inv_n2 / 240.0;
    result -= inv_n2 * inv_n2 * inv_n2 * inv_n2 * inv_n2 / 132.0;
    result += inv_n2 * inv_n2 * inv_n2 * inv_n2 * inv_n2 * inv_n2 * 691.0 / 32760.0;

    return result;
}

// Verify with small cases
double R_brute(int n) {
    vector<double> R(n + 1, 0);
    R[1] = 1.0;
    for (int k = 2; k <= n; k++) {
        double s = 0;
        for (int j = 1; j < k; j++) {
            s += j * R[j];
        }
        R[k] = 1.0 + 2.0 * s / ((double)k * k);
    }
    return R[n];
}

double B_brute(int n) {
    auto [sum, sz] = compute_B(n);
    return sum / n;
}

int main() {
    // Verify R(6) = 2.71666667 and B(6) = 2.33333333
    printf("R(6) = %.8f\n", R_brute(6));
    printf("B(6) = %.8f\n", B_brute(6));

    // Compute for 10^10
    ll N = 10000000000LL;

    // R(N) = 2*H_N - 1
    double H_N = harmonic_asymptotic(N);
    double R_N = 2.0 * H_N - 1.0;

    // B(N): compute via recursive tree decomposition
    auto [depth_sum, sz] = compute_B(N);
    double B_N = depth_sum / (double)N;

    double answer = R_N - B_N;
    printf("R(10^10) - B(10^10) = %.8f\n", answer);

    return 0;
}
