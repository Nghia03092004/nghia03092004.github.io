#include <bits/stdc++.h>
using namespace std;
int pisano(int m) {
    if (m == 1) return 1;
    int a = 0, b = 1;
    for (int k = 1; k <= 6*m+1; k++) {
        int c = (a+b)%m; a = b; b = c;
        if (a == 0 && b == 1) return k;
    }
    return -1;
}
int main() {
    long long total = 0;
    for (int m = 2; m <= 1000; m++) total += pisano(m);
    cout << total << endl;
    return 0;
}
