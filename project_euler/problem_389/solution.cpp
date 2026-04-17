#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler Problem 389: Platonic Dice
 *
 * Chain: T~d4 -> C=sum(T x d6) -> O=sum(C x d8) -> D=sum(O x d12) -> I=sum(D x d20)
 *
 * Law of total variance applied recursively:
 *   E[sum]   = E[Y] * (n+1)/2
 *   Var[sum] = E[Y] * (n^2-1)/12 + Var[Y] * ((n+1)/2)^2
 *
 * Answer: 2406376.3623
 */

int main() {
    // die_mean(n) = (n+1)/2,  die_var(n) = (n*n - 1)/12
    int sides[] = {4, 6, 8, 12, 20};

    // Layer 0: single d4
    double e = (sides[0] + 1.0) / 2.0;
    double v = (sides[0] * sides[0] - 1.0) / 12.0;

    // Layers 1 through 4
    for (int i = 1; i < 5; i++) {
        int n = sides[i];
        double mu    = (n + 1.0) / 2.0;
        double sigma2 = (1.0 * n * n - 1.0) / 12.0;

        double new_e = e * mu;
        double new_v = e * sigma2 + v * mu * mu;

        e = new_e;
        v = new_v;
    }

    // Round to 4 decimal places
    double answer = round(v * 10000.0) / 10000.0;

    cout << fixed << setprecision(4);
    cout << "Var(I) = " << answer << endl;

    // Verify
    assert(abs(answer - 2406376.3623) < 1e-4);
    cout << "Assertion passed." << endl;

    return 0;
}
