#include <bits/stdc++.h>
using namespace std;

int main(){
    // Problem 174: Count values of t <= 10^6 with 1 <= N(t) <= 10
    // where N(t) = number of hollow square laminae with exactly t tiles.
    // t = 4*m, N(t) = number of divisors k of m with k < sqrt(m).

    const int LIMIT = 1000000;
    const int M = LIMIT / 4; // 250000

    vector<int> N(M + 1, 0);

    // For each k, count factorizations m = k * q with k < q
    for(int k = 1; (long long)k * k < M; k++){
        for(int q = k + 1; (long long)k * q <= M; q++){
            N[k * q]++;
        }
    }

    int ans = 0;
    for(int m = 1; m <= M; m++){
        if(N[m] >= 1 && N[m] <= 10) ans++;
    }

    cout << ans << endl;
    return 0;
}
