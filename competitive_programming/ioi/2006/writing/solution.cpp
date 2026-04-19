#include <bits/stdc++.h>
using namespace std;

struct AhoCorasick {
    static const int ALPHA = 26; // adjust for problem's alphabet
    struct Node {
        int ch[26];
        int fail;
        int dictSuffix; // nearest pattern-ending node via fail chain
        int patternId;  // -1 if not end of pattern
        Node() {
            memset(ch, -1, sizeof(ch));
            fail = 0;
            dictSuffix = -1;
            patternId = -1;
        }
    };

    vector<Node> trie;

    AhoCorasick() {
        trie.emplace_back(); // root = node 0
    }

    void insert(const string& s, int id) {
        int cur = 0;
        for (char c : s) {
            int ci = c - 'a'; // adjust for alphabet
            if (trie[cur].ch[ci] == -1) {
                trie[cur].ch[ci] = trie.size();
                trie.emplace_back();
            }
            cur = trie[cur].ch[ci];
        }
        trie[cur].patternId = id;
    }

    void build() {
        queue<int> q;
        // Initialize root's children
        for (int c = 0; c < ALPHA; c++) {
            if (trie[0].ch[c] == -1) {
                trie[0].ch[c] = 0; // loop back to root
            } else {
                trie[trie[0].ch[c]].fail = 0;
                q.push(trie[0].ch[c]);
            }
        }

        while (!q.empty()) {
            int u = q.front(); q.pop();
            // Dictionary suffix link
            if (trie[trie[u].fail].patternId != -1) {
                trie[u].dictSuffix = trie[u].fail;
            } else {
                trie[u].dictSuffix = trie[trie[u].fail].dictSuffix;
            }

            for (int c = 0; c < ALPHA; c++) {
                if (trie[u].ch[c] == -1) {
                    trie[u].ch[c] = trie[trie[u].fail].ch[c];
                } else {
                    trie[trie[u].ch[c]].fail = trie[trie[u].fail].ch[c];
                    q.push(trie[u].ch[c]);
                }
            }
        }
    }

    // Search text and count occurrences of each pattern
    vector<int> search(const string& text, int numPatterns) {
        vector<int> count(numPatterns, 0);
        int cur = 0;
        for (char c : text) {
            cur = trie[cur].ch[c - 'a'];
            // Report all patterns ending here
            int tmp = cur;
            while (tmp > 0) {
                if (trie[tmp].patternId != -1) {
                    count[trie[tmp].patternId]++;
                }
                tmp = trie[tmp].dictSuffix;
                if (tmp == -1) break;
            }
        }
        return count;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int K;
    cin >> K;

    AhoCorasick ac;
    for (int i = 0; i < K; i++) {
        string p;
        cin >> p;
        ac.insert(p, i);
    }
    ac.build();

    string text;
    cin >> text;

    vector<int> counts = ac.search(text, K);

    for (int i = 0; i < K; i++) {
        cout << counts[i] << "\n";
    }

    return 0;
}
