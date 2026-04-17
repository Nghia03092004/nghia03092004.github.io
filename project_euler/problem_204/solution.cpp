#include <bits/stdc++.h>
using namespace std;
int primes[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};
long long LIMIT = 1000000000LL;
int dfs(int idx, long long cur) {
    if (idx == 25) return 1;
    int count = 0;
    long long power = 1;
    while (cur * power <= LIMIT) {
        count += dfs(idx + 1, cur * power);
        power *= primes[idx];
    }
    return count;
}
int main() {
    int answer = dfs(0, 1);
    assert(answer == 2944730);
    cout << answer << endl;
    return 0;
}
