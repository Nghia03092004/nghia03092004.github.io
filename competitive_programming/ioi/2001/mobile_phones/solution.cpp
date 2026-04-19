// IOI 2001 - Mobile Phones
// 2D grid with point updates and rectangle sum queries.
// Uses a 2D Binary Indexed Tree (Fenwick Tree).
// Operations: 0=init, 1=update(x,y,v), 2=query(l,b,r,t), 3=terminate.
// Input coordinates are 0-indexed; internally we use 1-indexed BIT.
// Complexity: O(log^2 S) per operation, O(S^2) space.

#include <bits/stdc++.h>
using namespace std;

int S;
int tree[1025][1025];

void update(int x, int y, int v) {
    for (int i = x; i <= S; i += i & (-i))
        for (int j = y; j <= S; j += j & (-j))
            tree[i][j] += v;
}

int query(int x, int y) {
    int s = 0;
    for (int i = x; i > 0; i -= i & (-i))
        for (int j = y; j > 0; j -= j & (-j))
            s += tree[i][j];
    return s;
}

int rectQuery(int x1, int y1, int x2, int y2) {
    return query(x2, y2)
         - query(x1 - 1, y2)
         - query(x2, y1 - 1)
         + query(x1 - 1, y1 - 1);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int op;
    cin >> op; // op == 0: init
    cin >> S;

    memset(tree, 0, sizeof(tree));

    while (cin >> op) {
        if (op == 3) break; // terminate

        if (op == 1) {
            int x, y, v;
            cin >> x >> y >> v;
            x++; y++; // convert to 1-indexed
            update(x, y, v);
        } else if (op == 2) {
            int l, b, r, t;
            cin >> l >> b >> r >> t;
            l++; b++; r++; t++; // convert to 1-indexed
            cout << rectQuery(l, b, r, t) << "\n";
        }
    }

    return 0;
}
