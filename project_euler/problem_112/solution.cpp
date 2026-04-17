#include <bits/stdc++.h>
using namespace std;

bool is_bouncy(int n) {
    int prev = n % 10;
    n /= 10;
    bool inc = true, dec = true;
    while (n > 0) {
        int d = n % 10;
        if (d > prev) inc = false;
        if (d < prev) dec = false;
        prev = d;
        n /= 10;
    }
    return !inc && !dec;
}

int main() {
    int bouncy = 0;
    for (int n = 1;; n++) {
        if (is_bouncy(n)) bouncy++;
        if (100 * bouncy == 99 * n) {
            cout << n << endl;
            return 0;
        }
    }
}
