#include <bits/stdc++.h>
using namespace std;

int main() {
    // L values satisfy L_{n+1} = 18*L_n - L_{n-1}
    // L_1 = 17, L_2 = 305
    // Sum the first 12 L values.

    long long L_prev = 17;
    long long L_curr = 305;
    long long total = L_prev + L_curr;

    for (int i = 3; i <= 12; i++) {
        long long L_next = 18 * L_curr - L_prev;
        total += L_next;
        L_prev = L_curr;
        L_curr = L_next;
    }

    cout << total << endl;
    return 0;
}
