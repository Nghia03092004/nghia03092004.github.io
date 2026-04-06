#include <bits/stdc++.h>
using namespace std;
const long long MOD = 1e9+7;
int main() {
    int n = 1000;
    vector<long long> row(n+1, 0);
    row[0] = 1;
    for (int i = 1; i <= n; i++) {
        vector<long long> nr(n+1, 0);
        nr[0] = row[i-1];
        for (int j = 1; j <= i; j++)
            nr[j] = (nr[j-1] + row[j-1]) % MOD;
        row = nr;
    }
    cout << row[0] << endl;
    return 0;
}
