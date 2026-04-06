#include <bits/stdc++.h>
using namespace std;

int main() {
    // Solve a^2 - 5*b^2 = 44
    // Fundamental solutions: (7,1), (8,2), (13,5), (17,7)
    // Recurrence: (a,b) -> (9a+20b, 4a+9b)
    // Need a > 7 and a % 5 == 2, then n = (a-7)/5

    vector<pair<long long, long long>> fund = {
        {7, 1}, {8, 2}, {13, 5}, {17, 7}
    };

    set<long long> nuggets;

    // Try all sign combinations for fundamental solutions
    for (auto [a0, b0] : fund) {
        vector<pair<long long, long long>> starts = {
            {a0, b0}, {-a0, -b0}, {-a0, b0}, {a0, -b0}
        };
        for (auto [sa, sb] : starts) {
            long long a = sa, b = sb;
            for (int iter = 0; iter < 40; iter++) {
                if (a > 7 && (a - 7) % 5 == 0) {
                    long long n = (a - 7) / 5;
                    if (n > 0) nuggets.insert(n);
                }
                long long na = 9 * a + 20 * b;
                long long nb = 4 * a + 9 * b;
                a = na;
                b = nb;
            }
        }
    }

    // Sum first 30
    long long sum = 0;
    int count = 0;
    for (long long n : nuggets) {
        if (count >= 30) break;
        sum += n;
        count++;
    }

    cout << sum << endl;
    return 0;
}
