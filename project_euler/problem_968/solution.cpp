#include <bits/stdc++.h>
using namespace std;
int main() {
    queue<pair<long long,long long>> q;
    q.push({1,1});
    long long total = 0;
    for (int i = 0; i < 1000; i++) {
        auto [a,b] = q.front(); q.pop();
        total += a;
        q.push({a, a+b});
        q.push({a+b, b});
    }
    cout << total << endl;
    return 0;
}
