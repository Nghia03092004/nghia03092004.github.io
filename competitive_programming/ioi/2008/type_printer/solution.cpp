#include <bits/stdc++.h>
using namespace std;

struct TrieNode {
    int ch[26];
    bool isEnd;
    bool onLongest; // on path to longest word
    TrieNode() {
        memset(ch, -1, sizeof(ch));
        isEnd = false;
        onLongest = false;
    }
};

vector<TrieNode> trie;
vector<char> ops; // operation sequence

int newNode() {
    trie.emplace_back();
    return trie.size() - 1;
}

void insert(const string& s) {
    int cur = 0;
    for (char c : s) {
        int ci = c - 'a';
        if (trie[cur].ch[ci] == -1) {
            trie[cur].ch[ci] = newNode();
        }
        cur = trie[cur].ch[ci];
    }
    trie[cur].isEnd = true;
}

// Mark path to longest word
string longestWord;

void markLongest() {
    int cur = 0;
    for (char c : longestWord) {
        int ci = c - 'a';
        cur = trie[cur].ch[ci];
        trie[cur].onLongest = true;
    }
}

void dfs(int v) {
    // Print if this node is end of a word
    if (trie[v].isEnd) {
        ops.push_back('P'); // Print
    }

    // Visit children, but visit the one on longest path LAST
    int lastChild = -1;
    for (int c = 0; c < 26; c++) {
        if (trie[v].ch[c] == -1) continue;
        int child = trie[v].ch[c];
        if (trie[child].onLongest) {
            lastChild = c;
            continue;
        }
        // Push, recurse, pop
        ops.push_back('a' + c);
        dfs(child);
        ops.push_back('-'); // Pop
    }

    // Visit the longest-path child last
    if (lastChild != -1) {
        ops.push_back('a' + lastChild);
        dfs(trie[v].ch[lastChild]);
        // Don't pop after the last path
        // Actually, we should pop if this node is not the root
        // and we need to return to the parent.
        // We only skip pops for the very last word.
        // Solution: pop only if v is not on the longest path OR
        // if there's more work to do after this subtree.
        // Hmm, the trick is: by visiting longest last, we descend
        // into it and never come back. So no pop needed.
        // But the caller will add a pop when returning from this node.
        // Fix: we handle pops in the DFS differently.
        // Actually, the standard approach is to NOT pop after the
        // DFS into the last-visited child. Since the caller pops
        // when returning, we need a different structure.

        // Let me restructure: in the DFS, the PARENT adds the pop.
        // So here we just push and recurse. The parent will pop
        // after we return. For the longest-path child, after DFS
        // returns, the parent won't pop (because it's the last thing).
        // But the parent IS on the longest path too, so ITS parent
        // also won't pop it. This propagates up to the root.

        // So the correct structure: at each node, after processing
        // the last child, DON'T add a pop. The parent handles it.
        // Since longest child is last, and parent also doesn't pop
        // the longest path, it works out.

        // But for non-longest-path children, we DO pop after returning.
        // This is already handled above.

        // For the longest-path child: we push the letter but don't pop.
        // The parent also doesn't pop us (because we're processed last
        // and the parent's parent also treats us as last).
        // This chain of "don't pop" extends from root to longest word.
        ;
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    newNode(); // root

    vector<string> words(N);
    int maxLen = 0;
    int maxIdx = 0;
    for (int i = 0; i < N; i++) {
        cin >> words[i];
        insert(words[i]);
        if ((int)words[i].size() > maxLen) {
            maxLen = words[i].size();
            maxIdx = i;
        }
    }

    longestWord = words[maxIdx];
    markLongest();

    // DFS from root, but we handle the recursion carefully
    // to avoid popping on the longest path.
    ops.clear();

    // Rewrite DFS to handle pops correctly:
    // At each node, for each child:
    //   push(letter), recurse, pop
    // EXCEPT the last child at each node (longest path child):
    //   push(letter), recurse, NO pop

    function<void(int, bool)> dfs2 = [&](int v, bool isLast) {
        if (trie[v].isEnd) {
            ops.push_back('P');
        }

        vector<int> children;
        for (int c = 0; c < 26; c++) {
            if (trie[v].ch[c] != -1)
                children.push_back(c);
        }

        // Put longest-path child last
        int longChild = -1;
        for (int i = 0; i < (int)children.size(); i++) {
            if (trie[trie[v].ch[children[i]]].onLongest) {
                longChild = i;
            }
        }
        if (longChild != -1) {
            // Move to end
            swap(children[longChild], children.back());
        }

        for (int i = 0; i < (int)children.size(); i++) {
            int c = children[i];
            bool last = (i == (int)children.size() - 1) && isLast;
            ops.push_back('a' + c);
            dfs2(trie[v].ch[c], last);
            if (!last) {
                ops.push_back('-'); // Pop
            }
        }
    };

    dfs2(0, true);

    cout << ops.size() << "\n";
    for (char c : ops) {
        if (c == 'P')
            cout << "P\n";
        else if (c == '-')
            cout << "-\n";
        else
            cout << c << "\n";
    }

    return 0;
}
