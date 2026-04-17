#include <bits/stdc++.h>
using namespace std;
int main() {
    int n = 1000000;
    double E = 0.0;
    for (int k = 1; k <= n; k++)
        E += 1.0 / (2*k - 1);
    cout << (long long)E << endl;
    return 0;
}
