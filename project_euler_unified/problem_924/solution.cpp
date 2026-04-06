#include <bits/stdc++.h>
using namespace std;
int main() {
    int n = 100;
    double c = -log(log(2));
    double p = (log(n) + c) / n;
    cout << (int)ceil(p * 1e6) << endl;
    return 0;
}
