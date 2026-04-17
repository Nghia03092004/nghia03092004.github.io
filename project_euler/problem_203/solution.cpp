#include <bits/stdc++.h>
using namespace std;

int main(){
    // Find sum of distinct squarefree binomial coefficients in rows 0..50.
    // By Kummer's theorem, for n<=50, only primes 2,3,5,7 can appear with
    // exponent >= 2. So check divisibility by 4, 9, 25, 49.

    const int N = 50;
    set<long long> vals;

    // Generate Pascal's triangle
    vector<vector<long long>> C(N+1);
    for(int n = 0; n <= N; n++){
        C[n].resize(n+1);
        C[n][0] = C[n][n] = 1;
        for(int k = 1; k < n; k++){
            C[n][k] = C[n-1][k-1] + C[n-1][k];
        }
        for(int k = 0; k <= n; k++){
            vals.insert(C[n][k]);
        }
    }

    long long ans = 0;
    for(long long v : vals){
        if(v % 4 != 0 && v % 9 != 0 && v % 25 != 0 && v % 49 != 0){
            ans += v;
        }
    }

    cout << ans << endl;
    return 0;
}
