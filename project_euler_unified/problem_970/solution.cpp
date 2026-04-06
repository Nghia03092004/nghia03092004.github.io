#include <bits/stdc++.h>
using namespace std;
int main() {
    const int N = 1000000;
    vector<int> t(N+1, 0);
    for (int n = 2; n <= N; n++) {
        long long k = n; int steps = 0;
        while (k >= n) {
            k = (k%2==0) ? k/2 : 3*k+1;
            steps++;
        }
        t[n] = steps + t[(int)k];
    }
    long long ans = 0;
    for (int n = 1; n <= N; n++) ans += t[n];
    cout << ans << endl;
    return 0;
}
