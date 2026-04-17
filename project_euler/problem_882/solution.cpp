/*
 * Problem 882: Matchstick Equations
 * Matchstick costs: 0->6,1->2,2->5,3->5,4->4,5->5,6->6,7->3,8->7,9->6
 * Max number with m sticks: greedy (all 1s, or 7 + 1s for odd m).
 * DP for counting valid equations A + B = C.
 */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const int COST[] = {6, 2, 5, 5, 4, 5, 6, 3, 7, 6};

int matchstick_cost(int n) {
    if (n == 0) return COST[0];
    int total = 0;
    while (n > 0) { total += COST[n % 10]; n /= 10; }
    return total;
}

string max_number_greedy(int m) {
    if (m < 2) return "";
    if (m % 2 == 0) return string(m / 2, '1');
    return "7" + string((m - 3) / 2, '1');
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout << "=== Digit Costs ===" << endl;
    for (int d = 0; d <= 9; d++)
        cout << "cost(" << d << ") = " << COST[d] << endl;

    cout << "\n=== Max Number ===" << endl;
    for (int m = 2; m <= 20; m++)
        cout << "m=" << m << ": " << max_number_greedy(m) << endl;

    cout << "\n=== Cost Verification ===" << endl;
    for (int n : {0, 1, 7, 11, 71, 111, 1234}) {
        cout << "cost(" << n << ") = " << matchstick_cost(n) << endl;
    }

    // Count small equations
    cout << "\n=== Equations A+B=C ===" << endl;
    for (int budget = 10; budget <= 30; budget++) {
        int rem = budget - 4;
        int count = 0;
        for (int A = 0; A < 200; A++) {
            int cA = matchstick_cost(A);
            if (cA >= rem) continue;
            for (int B = A; B < 200; B++) {
                int cB = matchstick_cost(B);
                if (cA + cB >= rem) continue;
                int C = A + B;
                int cC = matchstick_cost(C);
                if (cA + cB + cC == rem) count++;
            }
        }
        if (count > 0)
            cout << "budget=" << budget << ": " << count << " equations" << endl;
    }

    return 0;
}
