#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

ll gcd(ll a, ll b){
    while(b){ a %= b; swap(a, b); }
    return a;
}

// Inner segment tree (on columns), dynamic
struct InnerNode {
    ll val;
    int left, right; // children indices
};

vector<InnerNode> innerPool;
int newInnerNode(){
    innerPool.push_back({0, -1, -1});
    return (int)innerPool.size() - 1;
}

void innerUpdate(int node, int lo, int hi, int pos, ll val){
    if(lo == hi){
        innerPool[node].val = val;
        return;
    }
    int mid = lo + (hi - lo) / 2;
    if(pos <= mid){
        if(innerPool[node].left == -1)
            innerPool[node].left = newInnerNode();
        innerUpdate(innerPool[node].left, lo, mid, pos, val);
    } else {
        if(innerPool[node].right == -1)
            innerPool[node].right = newInnerNode();
        innerUpdate(innerPool[node].right, mid+1, hi, pos, val);
    }
    ll lv = (innerPool[node].left == -1) ? 0 : innerPool[innerPool[node].left].val;
    ll rv = (innerPool[node].right == -1) ? 0 : innerPool[innerPool[node].right].val;
    innerPool[node].val = gcd(lv, rv);
}

ll innerQuery(int node, int lo, int hi, int ql, int qr){
    if(node == -1 || lo > qr || hi < ql) return 0;
    if(ql <= lo && hi <= qr) return innerPool[node].val;
    int mid = lo + (hi - lo) / 2;
    return gcd(innerQuery(innerPool[node].left, lo, mid, ql, qr),
               innerQuery(innerPool[node].right, mid+1, hi, ql, qr));
}

// Outer segment tree (on rows), dynamic
struct OuterNode {
    int innerRoot; // root of inner segment tree
    int left, right; // children indices
};

vector<OuterNode> outerPool;
int newOuterNode(){
    outerPool.push_back({newInnerNode(), -1, -1});
    return (int)outerPool.size() - 1;
}

int R, C;

void outerUpdate(int node, int lo, int hi, int row, int col, ll val){
    if(lo == hi){
        innerUpdate(outerPool[node].innerRoot, 0, C-1, col, val);
        return;
    }
    int mid = lo + (hi - lo) / 2;
    if(row <= mid){
        if(outerPool[node].left == -1)
            outerPool[node].left = newOuterNode();
        outerUpdate(outerPool[node].left, lo, mid, row, col, val);
    } else {
        if(outerPool[node].right == -1)
            outerPool[node].right = newOuterNode();
        outerUpdate(outerPool[node].right, mid+1, hi, row, col, val);
    }
    // Merge children's inner trees at this node
    ll lv = (outerPool[node].left == -1) ? 0 :
        innerQuery(outerPool[outerPool[node].left].innerRoot, 0, C-1, col, col);
    ll rv = (outerPool[node].right == -1) ? 0 :
        innerQuery(outerPool[outerPool[node].right].innerRoot, 0, C-1, col, col);
    innerUpdate(outerPool[node].innerRoot, 0, C-1, col, gcd(lv, rv));
}

ll outerQuery(int node, int lo, int hi, int r1, int r2, int c1, int c2){
    if(node == -1 || lo > r2 || hi < r1) return 0;
    if(r1 <= lo && hi <= r2){
        return innerQuery(outerPool[node].innerRoot, 0, C-1, c1, c2);
    }
    int mid = lo + (hi - lo) / 2;
    return gcd(outerQuery(outerPool[node].left, lo, mid, r1, r2, c1, c2),
               outerQuery(outerPool[node].right, mid+1, hi, r1, r2, c1, c2));
}

void init(int r, int c){
    R = r; C = c;
    innerPool.clear();
    outerPool.clear();
    innerPool.reserve(5000000);
    outerPool.reserve(500000);
    newOuterNode(); // root = 0
}

void update(int r, int c, ll val){
    outerUpdate(0, 0, R-1, r, c, val);
}

ll calculate(int r1, int c1, int r2, int c2){
    return outerQuery(0, 0, R-1, r1, r2, c1, c2);
}

int main(){
    int r, c, n;
    cin >> r >> c >> n;
    init(r, c);

    for(int i = 0; i < n; i++){
        int type;
        cin >> type;
        if(type == 1){
            int pr, pc;
            ll val;
            cin >> pr >> pc >> val;
            update(pr, pc, val);
        } else {
            int r1, c1, r2, c2;
            cin >> r1 >> c1 >> r2 >> c2;
            cout << calculate(r1, c1, r2, c2) << "\n";
        }
    }

    return 0;
}
