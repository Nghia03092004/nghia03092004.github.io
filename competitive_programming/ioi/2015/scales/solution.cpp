#include <bits/stdc++.h>
using namespace std;

// Grader functions (provided by IOI grader):
int getMedian(int A, int B, int C);
int getLightest(int A, int B, int C);
int getHeaviest(int A, int B, int C);
int getNextLightest(int A, int B, int C, int D);
void answer(int W[]);  // W[0] = lightest, ..., W[5] = heaviest

// We precompute a decision tree. Each query is (type, a, b, c [, d]).
// Type: 0=lightest, 1=median, 2=heaviest, 3=nextLightest

struct Query {
    int type; // 0=lightest, 1=median, 2=heaviest, 3=nextLightest
    int a, b, c, d; // d only used for type 3
};

// Simulate a query on a known permutation (perm[i] = weight of coin i)
int simulate(const Query &q, int perm[]) {
    int wa = perm[q.a], wb = perm[q.b], wc = perm[q.c];
    int coins[3] = {q.a, q.b, q.c};
    int weights[3] = {wa, wb, wc};

    // Sort by weight
    for (int i = 0; i < 3; i++)
        for (int j = i+1; j < 3; j++)
            if (weights[i] > weights[j]) {
                swap(weights[i], weights[j]);
                swap(coins[i], coins[j]);
            }

    if (q.type == 0) return coins[0]; // lightest
    if (q.type == 1) return coins[1]; // median
    if (q.type == 2) return coins[2]; // heaviest
    // nextLightest: lightest among {a,b,c} that is heavier than d
    int wd = perm[q.d];
    for (int i = 0; i < 3; i++) {
        if (weights[i] > wd) return coins[i];
    }
    return coins[0]; // wrap around: lightest overall
}

// Generate all permutations
vector<vector<int>> allPerms;

void generatePerms() {
    allPerms.clear();
    vector<int> p = {1, 2, 3, 4, 5, 6};
    do {
        allPerms.push_back(p);
    } while (next_permutation(p.begin(), p.end()));
}

// Generate all possible queries
vector<Query> allQueries;

void generateQueries() {
    allQueries.clear();
    for (int a = 0; a < 6; a++)
    for (int b = a+1; b < 6; b++)
    for (int c = b+1; c < 6; c++) {
        allQueries.push_back({0, a, b, c, -1});
        allQueries.push_back({1, a, b, c, -1});
        allQueries.push_back({2, a, b, c, -1});
        for (int d = 0; d < 6; d++) {
            if (d != a && d != b && d != c) {
                allQueries.push_back({3, a, b, c, d});
            }
        }
    }
}

// Decision tree node
struct TreeNode {
    Query query;
    int children[6]; // indexed by answer (coin 0-5), -1 if leaf
    int permIdx;     // if leaf, which permutation
    bool isLeaf;
};

vector<TreeNode> tree;

// Build decision tree: given a set of candidate permutation indices, find best query
int buildTree(vector<int> &cands, int depth) {
    if (cands.size() == 1) {
        TreeNode node;
        node.isLeaf = true;
        node.permIdx = cands[0];
        memset(node.children, -1, sizeof(node.children));
        tree.push_back(node);
        return tree.size() - 1;
    }
    if (depth >= 8) return -1; // shouldn't happen

    // Try each query, find one that splits well
    Query bestQuery;
    int bestMax = INT_MAX;
    map<int, vector<int>> bestSplit;

    for (auto &q : allQueries) {
        map<int, vector<int>> split;
        for (int idx : cands) {
            int perm[6];
            for (int i = 0; i < 6; i++) perm[i] = allPerms[idx][i];
            int result = simulate(q, perm);
            split[result].push_back(idx);
        }
        int maxPart = 0;
        for (auto &[k, v] : split) maxPart = max(maxPart, (int)v.size());

        if (maxPart < bestMax) {
            bestMax = maxPart;
            bestQuery = q;
            bestSplit = split;
        }
    }

    TreeNode node;
    node.isLeaf = false;
    node.query = bestQuery;
    node.permIdx = -1;
    memset(node.children, -1, sizeof(node.children));
    tree.push_back(node);
    int nodeIdx = tree.size() - 1;

    for (auto &[coin, subcands] : bestSplit) {
        tree[nodeIdx].children[coin] = buildTree(subcands, depth + 1);
    }

    return nodeIdx;
}

int rootIdx;

void init(int T) {
    generatePerms();
    generateQueries();
    tree.clear();
    vector<int> all(720);
    iota(all.begin(), all.end(), 0);
    rootIdx = buildTree(all, 0);
}

void orderCoins() {
    int cur = rootIdx;
    while (!tree[cur].isLeaf) {
        Query &q = tree[cur].query;
        int result;
        if (q.type == 0) result = getLightest(q.a+1, q.b+1, q.c+1) - 1;
        else if (q.type == 1) result = getMedian(q.a+1, q.b+1, q.c+1) - 1;
        else if (q.type == 2) result = getHeaviest(q.a+1, q.b+1, q.c+1) - 1;
        else result = getNextLightest(q.a+1, q.b+1, q.c+1, q.d+1) - 1;

        cur = tree[cur].children[result];
    }

    // Found the permutation
    vector<int> &perm = allPerms[tree[cur].permIdx];
    // perm[i] = weight of coin i+1
    // answer wants W[0] = lightest coin, ..., W[5] = heaviest
    vector<pair<int,int>> wc(6);
    for (int i = 0; i < 6; i++) wc[i] = {perm[i], i + 1};
    sort(wc.begin(), wc.end());
    int W[6];
    for (int i = 0; i < 6; i++) W[i] = wc[i].second;
    answer(W);
}

int main() {
    // Grader handles interaction
    return 0;
}
