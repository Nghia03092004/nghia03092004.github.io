// IOI 1989 - Problem 3: Mobile
// Balance a binary mobile by computing minimal integer arm lengths.
// For each internal node: d_L * W_L = d_R * W_R => d_L = W_R/g, d_R = W_L/g
#include <bits/stdc++.h>
using namespace std;

struct Node {
    bool isLeaf;
    long long weight;
    Node *left, *right;
    long long dL, dR;
    Node() : isLeaf(false), weight(0), left(nullptr), right(nullptr), dL(0), dR(0) {}
};

// Read mobile: positive int = leaf weight, 0 = internal node followed by two subtrees
Node* readMobile() {
    Node* node = new Node();
    long long val;
    scanf("%lld", &val);
    if (val > 0) {
        node->isLeaf = true;
        node->weight = val;
    } else {
        node->left = readMobile();
        node->right = readMobile();
    }
    return node;
}

// Compute total weight and arm lengths bottom-up
long long solve(Node* node) {
    if (node->isLeaf) return node->weight;

    long long wL = solve(node->left);
    long long wR = solve(node->right);

    long long g = __gcd(wL, wR);
    node->dL = wR / g;
    node->dR = wL / g;
    node->weight = wL + wR;
    return node->weight;
}

// Print arm lengths for all internal nodes (pre-order)
void printResult(Node* node) {
    if (node->isLeaf) return;
    printf("%lld %lld\n", node->dL, node->dR);
    printResult(node->left);
    printResult(node->right);
}

void freeMobile(Node* node) {
    if (!node) return;
    freeMobile(node->left);
    freeMobile(node->right);
    delete node;
}

int main() {
    Node* root = readMobile();
    solve(root);
    printResult(root);
    freeMobile(root);
    return 0;
}
