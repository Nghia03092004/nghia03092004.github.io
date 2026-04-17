/*
 * Problem 891: Pentagon Tilings
 * Euler's pentagonal theorem and partition function recurrence.
 */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

vector<ll> partition_function(int N) {
    vector<ll> p(N + 1, 0);
    p[0] = 1;
    for (int n = 1; n <= N; n++) {
        for (int k = 1; ; k++) {
            int w1 = k * (3*k - 1) / 2;
            int w2 = k * (3*k + 1) / 2;
            int sign = (k % 2 == 1) ? 1 : -1;
            if (w1 > n) break;
            p[n] += sign * p[n - w1];
            if (w2 <= n) p[n] += sign * p[n - w2];
        }
    }
    return p;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    auto p = partition_function(200);
    cout << "=== Partition Function ===" << endl;
    for (int n : {0,1,2,3,4,5,10,20,50,100,200})
        cout << "p(" << n << ") = " << p[n] << endl;

    cout << "\n=== Pentagonal Numbers ===" << endl;
    for (int k = 1; k <= 10; k++) {
        cout << "w(" << k << ") = " << k*(3*k-1)/2
             << ", w(" << -k << ") = " << k*(3*k+1)/2 << endl;
    }

    cout << "\nAnswer: p(100) = " << p[100] << endl;
    return 0;
}
