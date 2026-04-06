#include <bits/stdc++.h>
using namespace std;
int main() {
    double n = 1e4;
    double gamma_em = 0.5772156649;
    double E_n = (n / 2.0) * (log(n) + gamma_em);
    double baseline = (n / 2.0) * log(n);
    double f_n = E_n / baseline;
    cout << (long long)(f_n * 1e4) << endl;
    return 0;
}
