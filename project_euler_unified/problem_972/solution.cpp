#include <bits/stdc++.h>
using namespace std;
int main() {
    const int N = 1 << 20;
    vector<int> s(N+1, 0);
    s[1] = 1;
    for (int n = 2; n <= N; n++)
        s[n] = (n%2==0) ? s[n/2] : s[n/2] + s[n/2+1];
    long long ans = 0;
    for (int n = 1; n <= N; n++) ans += s[n];
    cout << ans << endl;
    return 0;
}
