#include <bits/stdc++.h>
using namespace std;

namespace {

struct Node {
    int next[26];
    int link;
    int count;

    Node() : link(0), count(0) {
        memset(next, 0, sizeof(next));
    }
};

void solve() {
    int n, k;
    cin >> n >> k;

    vector<int> parent(n + 1);
    vector<int> letter(n + 1);
    for (int i = 1; i <= n; ++i) {
        char c;
        int p;
        cin >> c >> p;
        parent[i] = p;
        letter[i] = c - 'A';
    }

    vector<Node> trie(1);
    vector<int> query_state(k);
    for (int qi = 0; qi < k; ++qi) {
        string s;
        cin >> s;
        reverse(s.begin(), s.end());
        int v = 0;
        for (char ch : s) {
            int c = ch - 'A';
            if (trie[v].next[c] == 0) {
                trie[v].next[c] = (int)trie.size();
                trie.emplace_back();
            }
            v = trie[v].next[c];
        }
        query_state[qi] = v;
    }

    vector<int> order;
    order.reserve(trie.size());
    queue<int> q;
    for (int c = 0; c < 26; ++c) {
        int to = trie[0].next[c];
        if (to != 0) {
            q.push(to);
            order.push_back(to);
        }
    }

    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (int c = 0; c < 26; ++c) {
            int to = trie[v].next[c];
            if (to != 0) {
                trie[to].link = trie[trie[v].link].next[c];
                q.push(to);
                order.push_back(to);
            } else {
                trie[v].next[c] = trie[trie[v].link].next[c];
            }
        }
    }

    vector<int> lady_state(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        lady_state[i] = trie[lady_state[parent[i]]].next[letter[i]];
        ++trie[lady_state[i]].count;
    }

    for (int i = (int)order.size() - 1; i >= 0; --i) {
        int v = order[i];
        trie[trie[v].link].count += trie[v].count;
    }

    for (int qi = 0; qi < k; ++qi) {
        cout << trie[query_state[qi]].count << '\n';
    }
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
