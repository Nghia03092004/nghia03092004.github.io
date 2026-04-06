#include <bits/stdc++.h>
using namespace std;
int main() {
    const int N = 10000000;
    vector<int> d(N + 2, 0);
    for (int k = 1; k <= N + 1; k++)
        for (int j = k; j <= N + 1; j += k)
            d[j]++;
    int count = 0;
    for (int n = 2; n < N; n++)
        if (d[n] == d[n + 1]) count++;
    assert(count == 986262);
    cout << count << endl;
    return 0;
}
