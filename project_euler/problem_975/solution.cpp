#include <bits/stdc++.h>
using namespace std;
int main() {
    const int N = 1000000;
    vector<long long> fibs = {1, 2};
    while (fibs.back() < N) fibs.push_back(fibs[fibs.size()-1]+fibs[fibs.size()-2]);
    long long total = 0;
    for (int n = 1; n <= N; n++) {
        int rem = n, cnt = 0;
        for (int i = fibs.size()-1; i >= 0; i--)
            if (fibs[i] <= rem) { rem -= fibs[i]; cnt++; }
        total += cnt;
    }
    cout << total << endl;
    return 0;
}
