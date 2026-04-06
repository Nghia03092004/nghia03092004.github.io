#include <bits/stdc++.h>
using namespace std;

int main() {
    const int LIMIT = 1000000;

    // Phase 1: Sieve for sum of proper divisors
    vector<int> s(LIMIT + 1, 0);
    for (int i = 1; i <= LIMIT; i++)
        for (int j = 2 * i; j <= LIMIT; j += i)
            s[j] += i;

    // Phase 2: Detect longest amicable chain
    vector<bool> visited(LIMIT + 1, false);
    int bestLen = 0, bestMin = 0;

    for (int start = 2; start <= LIMIT; start++) {
        if (visited[start]) continue;

        vector<int> chain;
        unordered_map<int, int> positions;
        int n = start;

        while (n >= 1 && n <= LIMIT && !visited[n] && !positions.count(n)) {
            positions[n] = (int)chain.size();
            chain.push_back(n);
            n = s[n];
        }

        // Check for cycle
        if (n >= 1 && n <= LIMIT && positions.count(n)) {
            int cycleStart = positions[n];
            int cycleLen = (int)chain.size() - cycleStart;
            int minElem = *min_element(chain.begin() + cycleStart, chain.end());
            if (cycleLen > bestLen) {
                bestLen = cycleLen;
                bestMin = minElem;
            }
        }

        for (int x : chain) visited[x] = true;
    }

    cout << bestMin << endl;
    return 0;
}
