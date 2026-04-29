struct AhoCorasick {
    struct Node {
        array<int, 26> next{};
        int link = 0;
        vector<int> out;

        Node() {
            next.fill(-1);
        }
    };

    vector<Node> trie;

    AhoCorasick() : trie(1) {}

    void add_pattern(const string& s, int id) {
        int v = 0;
        for (char ch : s) {
            int c = ch - 'a';
            if (trie[v].next[c] == -1) {
                trie[v].next[c] = (int)trie.size();
                trie.push_back(Node{});
            }
            v = trie[v].next[c];
        }
        trie[v].out.push_back(id);
    }

    void build() {
        queue<int> q;
        for (int c = 0; c < 26; ++c) {
            int to = trie[0].next[c];
            if (to == -1) {
                trie[0].next[c] = 0;
            } else {
                trie[to].link = 0;
                q.push(to);
            }
        }

        while (!q.empty()) {
            int v = q.front();
            q.pop();
            int link = trie[v].link;
            for (int id : trie[link].out) {
                trie[v].out.push_back(id);
            }
            for (int c = 0; c < 26; ++c) {
                int to = trie[v].next[c];
                if (to == -1) {
                    trie[v].next[c] = trie[link].next[c];
                } else {
                    trie[to].link = trie[link].next[c];
                    q.push(to);
                }
            }
        }
    }

    vector<int> match_counts(const string& text, int pattern_count) const {
        vector<int> cnt(pattern_count, 0);
        int v = 0;
        for (char ch : text) {
            int c = ch - 'a';
            v = trie[v].next[c];
            for (int id : trie[v].out) {
                ++cnt[id];
            }
        }
        return cnt;
    }
};
