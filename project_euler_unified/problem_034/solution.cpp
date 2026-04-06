#include <bits/stdc++.h>
using namespace std;

int main() {
    int fact[10] = {1};
    for (int i = 1; i <= 9; i++) fact[i] = fact[i - 1] * i;

    int total = 0;
    for (int n = 3; n <= 2540160; n++) {
        int s = 0, tmp = n;
        while (tmp > 0) { s += fact[tmp % 10]; tmp /= 10; }
        if (s == n) total += n;
    }
    cout << total << endl;  // 40730
    return 0;
}
