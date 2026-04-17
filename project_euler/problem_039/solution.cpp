#include <bits/stdc++.h>
using namespace std;

int main() {
    const int LIMIT = 1000;
    int best_p = 0, best_count = 0;

    for (int p = 2; p <= LIMIT; p += 2) {
        int count = 0;
        for (int a = 1; a < p / 3; a++) {
            int num = p * (p - 2 * a);
            int den = 2 * (p - a);
            if (num % den == 0 && num / den >= a)
                count++;
        }
        if (count > best_count) {
            best_count = count;
            best_p = p;
        }
    }

    cout << best_p << endl;
    return 0;
}
