#include <bits/stdc++.h>
using namespace std;

int main() {
    int pow5[10];
    for (int i = 0; i < 10; i++)
        pow5[i] = i * i * i * i * i;

    int total = 0;
    for (int n = 2; n <= 354294; n++) {
        int s = 0, t = n;
        while (t > 0) {
            s += pow5[t % 10];
            t /= 10;
        }
        if (s == n)
            total += n;
    }

    cout << total << endl;
    return 0;
}
