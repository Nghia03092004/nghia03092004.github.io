#include <bits/stdc++.h>
using namespace std;

int main() {
    const int LIMIT = 1000000;
    int count = 0;

    // Pascal's triangle with capping to prevent overflow
    vector<long long> row(102, 0);
    row[0] = 1;

    for (int n = 1; n <= 100; n++) {
        for (int r = n; r >= 1; r--) {
            row[r] = row[r] + row[r - 1];
            if (row[r] > LIMIT) row[r] = LIMIT + 1;
        }
        for (int r = 0; r <= n; r++) {
            if (row[r] > LIMIT) count++;
        }
    }

    cout << count << endl;
    return 0;
}
