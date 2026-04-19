// IOI 1995 - Shopping Offers
// State-space DP over product quantities (at most 6^5 = 7776 states)
// Time: O(Q^b * (b + s)), Space: O(Q^b)
#include <bits/stdc++.h>
using namespace std;

int b; // number of products (1..5)
int price[6]; // unit price for each product (1-indexed)
int want[6];  // desired quantity for each product
int code[6];  // product code mapping

int s; // number of offers
struct Offer {
    int qty[6]; // quantity of each product in offer
    int cost;
};
Offer offers[100];

int dp[6][6][6][6][6];

int main() {
    scanf("%d", &s);

    // Read offers
    for (int i = 0; i < s; i++) {
        int nk;
        scanf("%d", &nk);
        memset(offers[i].qty, 0, sizeof(offers[i].qty));
        for (int j = 0; j < nk; j++) {
            int c, q;
            scanf("%d %d", &c, &q);
            offers[i].qty[c] = q;
        }
        scanf("%d", &offers[i].cost);
    }

    scanf("%d", &b);
    for (int i = 1; i <= b; i++)
        scanf("%d %d %d", &code[i], &want[i], &price[i]);

    // Remap offer quantities to product indices 1..b
    Offer mapped[100];
    for (int i = 0; i < s; i++) {
        mapped[i].cost = offers[i].cost;
        memset(mapped[i].qty, 0, sizeof(mapped[i].qty));
        for (int j = 1; j <= b; j++)
            mapped[i].qty[j] = offers[i].qty[code[j]];
    }

    // Initialize DP with large value
    memset(dp, 0x3f, sizeof(dp));
    dp[0][0][0][0][0] = 0;

    int w[6] = {};
    for (int i = 1; i <= 5; i++)
        w[i] = (i <= b) ? want[i] : 0;

    for (int q1 = 0; q1 <= w[1]; q1++)
    for (int q2 = 0; q2 <= w[2]; q2++)
    for (int q3 = 0; q3 <= w[3]; q3++)
    for (int q4 = 0; q4 <= w[4]; q4++)
    for (int q5 = 0; q5 <= w[5]; q5++) {
        if (q1 == 0 && q2 == 0 && q3 == 0 && q4 == 0 && q5 == 0) continue;

        int best = 0x3f3f3f3f;
        int qq[6] = {0, q1, q2, q3, q4, q5};

        // Buy single items
        for (int j = 1; j <= b; j++) {
            if (qq[j] > 0) {
                int prev[6] = {0, q1, q2, q3, q4, q5};
                prev[j]--;
                best = min(best,
                    dp[prev[1]][prev[2]][prev[3]][prev[4]][prev[5]] + price[j]);
            }
        }

        // Use offers
        for (int o = 0; o < s; o++) {
            bool ok = true;
            int prev[6] = {0, q1, q2, q3, q4, q5};
            for (int j = 1; j <= b; j++) {
                prev[j] -= mapped[o].qty[j];
                if (prev[j] < 0) { ok = false; break; }
            }
            if (ok) {
                best = min(best,
                    dp[prev[1]][prev[2]][prev[3]][prev[4]][prev[5]] + mapped[o].cost);
            }
        }

        dp[q1][q2][q3][q4][q5] = best;
    }

    printf("%d\n", dp[w[1]][w[2]][w[3]][w[4]][w[5]]);
    return 0;
}
