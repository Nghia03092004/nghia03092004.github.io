#include <bits/stdc++.h>
using namespace std;

int main() {
    const int N = 100000;
    const int K = 10000;

    vector<int> rad(N + 1, 1);
    for (int i = 2; i <= N; i++) {
        if (rad[i] == 1) {  // i is prime
            for (int j = i; j <= N; j += i)
                rad[j] *= i;
        }
    }

    vector<pair<int, int>> v;
    v.reserve(N);
    for (int i = 1; i <= N; i++)
        v.push_back({rad[i], i});
    sort(v.begin(), v.end());

    cout << v[K - 1].second << endl;
    return 0;
}
