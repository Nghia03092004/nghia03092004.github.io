#include <bits/stdc++.h>
using namespace std;
int main() {
    long long total = 0;
    for (int n = 3; n <= 10000; n += 2) {
        long long val = 2;
        for (int k = 1; k < n; k++) {
            if (val == 1) { total += k; goto next; }
            val = val * 2 % n;
        }
        next:;
    }
    cout << total << endl;
    return 0;
}
