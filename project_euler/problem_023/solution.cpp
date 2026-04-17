#include <bits/stdc++.h>
using namespace std;

int main() {
    const int N = 28123;

    vector<int> spd(N + 1, 0);
    for (int i = 1; i <= N; i++)
        for (int j = 2 * i; j <= N; j += i)
            spd[j] += i;

    vector<int> abundant;
    for (int i = 1; i <= N; i++)
        if (spd[i] > i)
            abundant.push_back(i);

    vector<bool> is_sum(N + 1, false);
    for (int i = 0; i < (int)abundant.size(); i++)
        for (int j = i; j < (int)abundant.size(); j++) {
            int s = abundant[i] + abundant[j];
            if (s > N) break;
            is_sum[s] = true;
        }

    long long ans = 0;
    for (int i = 1; i <= N; i++)
        if (!is_sum[i])
            ans += i;

    cout << ans << endl;
    return 0;
}
