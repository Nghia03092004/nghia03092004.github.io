#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

struct AhoCorasick {
    int ALPHA;
    struct Node {
        vector<int> ch;
        int fail;
        bool forbidden;
        Node(int alpha) : ch(alpha, -1), fail(0), forbidden(false) {}
    };
    vector<Node> trie;

    AhoCorasick(int alpha) : ALPHA(alpha) {
        trie.emplace_back(ALPHA);
    }

    void insert(const string& s) {
        int cur = 0;
        for (char c : s) {
            int ci = c - 'a';
            if (trie[cur].ch[ci] == -1) {
                trie[cur].ch[ci] = trie.size();
                trie.emplace_back(ALPHA);
            }
            cur = trie[cur].ch[ci];
        }
        trie[cur].forbidden = true;
    }

    void build() {
        queue<int> q;
        for (int c = 0; c < ALPHA; c++) {
            if (trie[0].ch[c] == -1)
                trie[0].ch[c] = 0;
            else {
                trie[trie[0].ch[c]].fail = 0;
                q.push(trie[0].ch[c]);
            }
        }
        while (!q.empty()) {
            int u = q.front(); q.pop();
            trie[u].forbidden |= trie[trie[u].fail].forbidden;
            for (int c = 0; c < ALPHA; c++) {
                if (trie[u].ch[c] == -1)
                    trie[u].ch[c] = trie[trie[u].fail].ch[c];
                else {
                    trie[trie[u].ch[c]].fail = trie[trie[u].fail].ch[c];
                    q.push(trie[u].ch[c]);
                }
            }
        }
    }

    int size() { return trie.size(); }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K, ALPHA;
    cin >> N >> K >> ALPHA;
    // N = string length, K = number of forbidden patterns,
    // ALPHA = alphabet size (characters 'a' to 'a'+ALPHA-1)

    AhoCorasick ac(ALPHA);
    for (int i = 0; i < K; i++) {
        string p;
        cin >> p;
        ac.insert(p);
    }
    ac.build();

    int M = ac.size();

    // DP: dp[s] = number of valid strings of current length ending at state s
    vector<long long> dp(M, 0);
    dp[0] = 1; // empty string at root

    for (int i = 0; i < N; i++) {
        vector<long long> ndp(M, 0);
        for (int s = 0; s < M; s++) {
            if (dp[s] == 0) continue;
            if (ac.trie[s].forbidden) continue;
            for (int c = 0; c < ALPHA; c++) {
                int ns = ac.trie[s].ch[c];
                if (!ac.trie[ns].forbidden) {
                    ndp[ns] = (ndp[ns] + dp[s]) % MOD;
                }
            }
        }
        dp = ndp;
    }

    long long ans = 0;
    for (int s = 0; s < M; s++) {
        if (!ac.trie[s].forbidden) {
            ans = (ans + dp[s]) % MOD;
        }
    }

    cout << ans << "\n";
    return 0;
}
