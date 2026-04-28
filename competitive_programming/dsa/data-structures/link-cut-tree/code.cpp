struct LinkCut {
    struct Node {
        int ch[2] = {0, 0};
        int p = 0;
        bool rev = false;
        long long val = 0, sum = 0;
    };

    vector<Node> tr;

    LinkCut() = default;
    explicit LinkCut(int n) : tr(n + 1) {}

    bool is_root(int x) const {
        int p = tr[x].p;
        return p == 0 || (tr[p].ch[0] != x && tr[p].ch[1] != x);
    }

    void pull(int x) {
        tr[x].sum = tr[x].val;
        if (tr[x].ch[0]) tr[x].sum += tr[tr[x].ch[0]].sum;
        if (tr[x].ch[1]) tr[x].sum += tr[tr[x].ch[1]].sum;
    }

    void push(int x) {
        if (!x || !tr[x].rev) return;
        swap(tr[x].ch[0], tr[x].ch[1]);
        if (tr[x].ch[0]) tr[tr[x].ch[0]].rev ^= 1;
        if (tr[x].ch[1]) tr[tr[x].ch[1]].rev ^= 1;
        tr[x].rev = false;
    }

    void push_path(int x) {
        if (!is_root(x)) push_path(tr[x].p);
        push(x);
    }

    void rotate(int x) {
        int p = tr[x].p;
        int g = tr[p].p;
        int dir = (tr[p].ch[1] == x);
        int b = tr[x].ch[dir ^ 1];
        if (!is_root(p)) tr[g].ch[tr[g].ch[1] == p] = x;
        tr[x].p = g;
        tr[x].ch[dir ^ 1] = p;
        tr[p].p = x;
        tr[p].ch[dir] = b;
        if (b) tr[b].p = p;
        pull(p);
        pull(x);
    }

    void splay(int x) {
        push_path(x);
        while (!is_root(x)) {
            int p = tr[x].p;
            int g = tr[p].p;
            if (!is_root(p)) {
                bool zigzig = (tr[p].ch[0] == x) == (tr[g].ch[0] == p);
                rotate(zigzig ? p : x);
            }
            rotate(x);
        }
    }

    void access(int x) {
        int last = 0;
        for (int y = x; y; y = tr[y].p) {
            splay(y);
            tr[y].ch[1] = last;
            pull(y);
            last = y;
        }
        splay(x);
    }

    void make_root(int x) {
        access(x);
        tr[x].rev ^= 1;
    }

    int find_root(int x) {
        access(x);
        while (tr[x].ch[0]) {
            push(x);
            x = tr[x].ch[0];
        }
        splay(x);
        return x;
    }

    bool connected(int a, int b) {
        return find_root(a) == find_root(b);
    }

    bool link(int a, int b) {
        make_root(a);
        if (find_root(b) == a) return false;
        tr[a].p = b;
        return true;
    }

    bool cut(int a, int b) {
        make_root(a);
        access(b);
        if (tr[b].ch[0] != a || tr[a].ch[1] != 0) return false;
        tr[b].ch[0] = 0;
        tr[a].p = 0;
        pull(b);
        return true;
    }

    void set_value(int x, long long value) {
        access(x);
        tr[x].val = value;
        pull(x);
    }

    long long path_sum(int a, int b) {
        make_root(a);
        access(b);
        return tr[b].sum;
    }
};
