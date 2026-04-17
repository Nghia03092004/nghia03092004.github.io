#include <bits/stdc++.h>
using namespace std;

// Backward induction for the bidding game
// State: (chips_A, chips_B, rounds_needed_A, rounds_needed_B)
map<tuple<int,int,int,int>, double> memo;

double solve(int a, int b, int ra, int rb) {
    if (ra == 0) return 1.0;
    if (rb == 0) return 0.0;
    if (a == 0 && b == 0) return (ra <= rb) ? 1.0 : 0.0;

    auto key = make_tuple(a, b, ra, rb);
    auto it = memo.find(key);
    if (it != memo.end()) return it->second;

    // Build payoff matrix and find pure-strategy minimax
    double best_row_min = -1.0;
    for (int i = 0; i <= a; i++) {
        double worst = 1e18;
        for (int j = 0; j <= b; j++) {
            double val;
            if (i > j)
                val = solve(a - i, b - j, ra - 1, rb);
            else if (i < j)
                val = solve(a - i, b - j, ra, rb - 1);
            else
                val = solve(a - i, b - j, ra - 1, rb);  // tie: A wins
            worst = min(worst, val);
        }
        best_row_min = max(best_row_min, worst);
    }

    memo[key] = best_row_min;
    return best_row_min;
}

int main() {
    int max_n = 15;
    double total = 0.0;

    for (int n = 1; n <= max_n; n++) {
        memo.clear();
        double v = solve(n, n, 2, 2);
        total += v;
        cout << "n=" << n << ": " << fixed << setprecision(6) << v << endl;
    }

    cout << "\nSum = " << fixed << setprecision(6) << total << endl;
    return 0;
}
