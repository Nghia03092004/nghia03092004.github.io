#include <bits/stdc++.h>
using namespace std;

int main() {
    const int N = 1000000;
    vector<int> cache(N + 1, 0);
    cache[1] = 1;

    for (int i = 2; i < N; i++) {
        long long n = i;
        int steps = 0;
        while (n >= N || cache[n] == 0) {
            n = (n % 2 == 0) ? n / 2 : 3 * n + 1;
            steps++;
        }
        cache[i] = steps + (int)cache[n];
    }

    int best = 1;
    for (int i = 2; i < N; i++)
        if (cache[i] > cache[best])
            best = i;

    cout << best << endl;
    return 0;
}
