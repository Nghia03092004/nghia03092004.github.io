struct Trie {
    struct Node {
        array<int, 26> next{};
        int terminal = 0;
        int pass = 0;

        Node() {
            next.fill(-1);
        }
    };

    vector<Node> nodes;

    Trie() : nodes(1) {}

    void insert(const string& s) {
        int v = 0;
        for (char ch : s) {
            int c = ch - 'a';
            if (nodes[v].next[c] == -1) {
                nodes[v].next[c] = (int)nodes.size();
                nodes.push_back(Node{});
            }
            v = nodes[v].next[c];
            ++nodes[v].pass;
        }
        ++nodes[v].terminal;
    }

    bool contains(const string& s) const {
        int v = 0;
        for (char ch : s) {
            int c = ch - 'a';
            if (nodes[v].next[c] == -1) {
                return false;
            }
            v = nodes[v].next[c];
        }
        return nodes[v].terminal > 0;
    }

    int count_with_prefix(const string& s) const {
        int v = 0;
        for (char ch : s) {
            int c = ch - 'a';
            if (nodes[v].next[c] == -1) {
                return 0;
            }
            v = nodes[v].next[c];
        }
        return nodes[v].pass;
    }
};
