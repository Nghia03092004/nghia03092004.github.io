#include <bits/stdc++.h>
using namespace std;

int main() {
    long long total = 0;
    for (int a = 3; a <= 1000; a++) {
        if (a % 2 == 1) {
            total += (long long)a * (a - 1);
        } else {
            total += (long long)a * (a - 2);
        }
    }
    cout << total << endl;
    return 0;
}
