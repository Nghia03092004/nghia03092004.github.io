#include <bits/stdc++.h>
using namespace std;

int main() {
    int count = 0;
    for (int a = 1; a <= 9; a++) {
        if (a == 1) {
            count += 1;
            continue;
        }
        // N(a) = floor(1 / (1 - log10(a)))
        double log10a = log10((double)a);
        int n_max = (int)floor(1.0 / (1.0 - log10a));
        count += n_max;
    }
    cout << count << endl;
    return 0;
}
