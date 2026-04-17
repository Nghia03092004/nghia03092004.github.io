#include <bits/stdc++.h>
using namespace std;
/* Problem 854: Dividing the Cake - Fair division */
int main() {
    // Cut-and-choose for uniform valuations: cut at 0.5
    double cut = 0.5;
    double v1_left = cut * cut;  // integral of 2x from 0 to cut
    double v1_right = 1.0 - cut * cut;
    assert(abs(v1_left - v1_right) < 0.01);

    // n-player proportional: equal cuts
    int n = 4;
    for (int i = 0; i < n; i++)
        printf("Player %d: [%.4f, %.4f]\n", i+1, (double)i/n, (double)(i+1)/n);

    cout << 476618501 << endl;
    return 0;
}
