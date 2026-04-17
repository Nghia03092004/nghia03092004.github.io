#include <bits/stdc++.h>
using namespace std;

// Aho-Corasick automaton for multiple pattern matching
struct AhoCorasick {
    vector<map<char, int>> go;
    vector<int> fail, out;

    AhoCorasick() {
        go.push_back({});
        fail.push_back(0);
        out.push_back(0);
    }

    void addPattern(const string& s, int idx) {
        int cur = 0;
        for (char ch : s) {
            if (go[cur].find(ch) == go[cur].end()) {
                go[cur][ch] = go.size();
                go.push_back({});
                fail.push_back(0);
                out.push_back(0);
            }
            cur = go[cur][ch];
        }
        out[cur] |= (1 << idx);
    }

    void build(const string& alpha) {
        queue<int> q;
        for (char ch : alpha) {
            if (go[0].count(ch)) {
                fail[go[0][ch]] = 0;
                q.push(go[0][ch]);
            } else {
                go[0][ch] = 0;
            }
        }
        while (!q.empty()) {
            int r = q.front(); q.pop();
            for (char ch : alpha) {
                if (go[r].count(ch)) {
                    int s = go[r][ch];
                    q.push(s);
                    int st = fail[r];
                    while (st && !go[st].count(ch)) st = fail[st];
                    fail[s] = go[st].count(ch) ? go[st][ch] : 0;
                    if (fail[s] == s) fail[s] = 0;
                    out[s] |= out[fail[s]];
                } else {
                    int st = fail[r];
                    while (st && !go[st].count(ch)) st = fail[st];
                    go[r][ch] = go[st].count(ch) ? go[st][ch] : 0;
                }
            }
        }
    }
};

int main() {
    vector<string> keywords = {"FREE", "FARE", "AREA", "REEF"};
    string alpha = "AEFR";
    int N = 30;

    AhoCorasick ac;
    for (int i = 0; i < 4; i++) ac.addPattern(keywords[i], i);
    ac.build(alpha);

    int numNodes = ac.go.size();

    // dp[node][mask] = count
    map<pair<int,int>, long long> dp;
    dp[{0, 0}] = 1;

    for (int step = 0; step < N; step++) {
        map<pair<int,int>, long long> ndp;
        for (auto& [state, cnt] : dp) {
            auto [node, mask] = state;
            for (char ch : alpha) {
                int nn = ac.go[node][ch];
                int no = ac.out[nn];
                if (no & mask) continue; // keyword seen twice
                int nm = mask | no;
                ndp[{nn, nm}] += cnt;
            }
        }
        dp = ndp;
    }

    long long ans = 0;
    for (auto& [state, cnt] : dp) {
        if (state.second == 15) ans += cnt;
    }

    cout << ans << endl;
    return 0;
}
