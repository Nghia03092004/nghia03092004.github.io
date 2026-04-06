#include <bits/stdc++.h>
using namespace std;
int main() {
    long long N = 1000000;
    vector<long long> vals = {0};
    long long pw = 1;
    while (pw <= 2 * N) {
        vector<long long> nv;
        for (long long v : vals) {
            nv.push_back(v + pw);
            nv.push_back(v - pw);
        }
        for (long long v : nv) vals.push_back(v);
        pw *= 3;
    }
    long long sum = 0;
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
    for (long long v : vals)
        if (v >= 1 && v <= N) sum += v;
    cout << sum << endl;
    return 0;
}
