#include <bits/stdc++.h>
using namespace std;
int main() {
    int p = 355, q = 113;
    int la = 0, lb = 1, ra = 1, rb = 0;
    int depth = 0;
    while (true) {
        int ma = la + ra, mb = lb + rb;
        depth++;
        if (ma == p && mb == q) break;
        if ((long long)p * mb < (long long)ma * q) { ra = ma; rb = mb; }
        else { la = ma; lb = mb; }
    }
    cout << depth << endl;
    return 0;
}
