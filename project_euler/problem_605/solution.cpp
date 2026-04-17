#include <bits/stdc++.h>
using namespace std;
int main() {
    for (int n = 2; n <= 20; n++) {
        double p = n / pow(2.0, n - 1);
        cout << "n=" << n << ": P(unique winner)=" << fixed << setprecision(6) << p << endl;
    }
    return 0;
}
