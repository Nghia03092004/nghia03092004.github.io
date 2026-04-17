#include <bits/stdc++.h>
using namespace std;

int main() {
    // Precompute digit factorials
    int fact[10];
    fact[0] = 1;
    for (int i = 1; i <= 9; i++) fact[i] = fact[i-1] * i;

    auto digit_fact_sum = [&](int n) -> int {
        if (n == 0) return 1;
        int s = 0;
        while (n > 0) {
            s += fact[n % 10];
            n /= 10;
        }
        return s;
    };

    const int LIMIT = 1000000;
    const int CACHE_SIZE = 3000001;  // f(n) <= 2,540,160 for n < 10^7
    vector<int> chain_len(CACHE_SIZE, 0);

    int count = 0;
    for (int start = 1; start < LIMIT; start++) {
        vector<int> chain;
        unordered_set<int> seen;
        int cur = start;
        while (seen.find(cur) == seen.end()) {
            if (cur < CACHE_SIZE && chain_len[cur] > 0) {
                int total = (int)chain.size() + chain_len[cur];
                for (int i = (int)chain.size() - 1; i >= 0; i--) {
                    int val = chain[i];
                    int len = total - i;
                    if (val < CACHE_SIZE && chain_len[val] == 0)
                        chain_len[val] = len;
                }
                if (total == 60) count++;
                goto next;
            }
            seen.insert(cur);
            chain.push_back(cur);
            cur = digit_fact_sum(cur);
        }
        {
            int total = (int)chain.size();
            for (int i = (int)chain.size() - 1; i >= 0; i--) {
                int val = chain[i];
                int len = total - i;
                if (val < CACHE_SIZE && chain_len[val] == 0)
                    chain_len[val] = len;
            }
            if (total == 60) count++;
        }
        next:;
    }

    cout << count << endl;
    return 0;
}
