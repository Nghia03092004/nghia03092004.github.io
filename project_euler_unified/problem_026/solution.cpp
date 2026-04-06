#include <bits/stdc++.h>
using namespace std;

int main() {
    int best_d = 0, best_cycle = 0;

    for (int d = 2; d < 1000; d++) {
        unordered_map<int, int> seen;
        int r = 1, pos = 0, cycle_len = 0;

        while (r != 0) {
            if (seen.count(r)) {
                cycle_len = pos - seen[r];
                break;
            }
            seen[r] = pos;
            r = (r * 10) % d;
            pos++;
        }

        if (cycle_len > best_cycle) {
            best_cycle = cycle_len;
            best_d = d;
        }
    }

    cout << best_d << endl;
    return 0;
}
