struct SplayTree {
    struct Node {
        int key, sz;
        Node *p, *ch[2];
        explicit Node(int key_) : key(key_), sz(1), p(nullptr), ch{nullptr, nullptr} {}
    };

    using Ptr = Node*;
    Ptr root = nullptr;

    static int size(Ptr x) { return x ? x->sz : 0; }

    static void pull(Ptr x) {
        if (!x) return;
        x->sz = 1 + size(x->ch[0]) + size(x->ch[1]);
    }

    static bool is_right_child(Ptr x) {
        return x->p && x->p->ch[1] == x;
    }

    void connect(Ptr parent, Ptr child, int dir) {
        if (parent) parent->ch[dir] = child;
        if (child) child->p = parent;
    }

    void rotate(Ptr x) {
        Ptr p = x->p;
        Ptr g = p->p;
        int dx = is_right_child(x);
        Ptr b = x->ch[dx ^ 1];

        if (g) g->ch[g->ch[1] == p] = x;
        x->p = g;

        connect(p, b, dx);
        connect(x, p, dx ^ 1);

        pull(p);
        pull(x);
        if (!x->p) root = x;
    }

    void splay(Ptr x) {
        if (!x) return;
        while (x->p) {
            Ptr p = x->p;
            Ptr g = p->p;
            if (!g) {
                rotate(x);
            } else if ((g->ch[0] == p) == (p->ch[0] == x)) {
                rotate(p);
                rotate(x);
            } else {
                rotate(x);
                rotate(x);
            }
        }
        root = x;
    }

    Ptr find_node(int key) {
        Ptr cur = root;
        Ptr last = nullptr;
        while (cur && cur->key != key) {
            last = cur;
            cur = cur->ch[key > cur->key];
        }
        splay(cur ? cur : last);
        return root && root->key == key ? root : nullptr;
    }

    bool contains(int key) {
        return find_node(key) != nullptr;
    }

    void insert(int key) {
        if (!root) {
            root = new Node(key);
            return;
        }
        Ptr cur = root;
        while (true) {
            if (key == cur->key) {
                splay(cur);
                return;
            }
            int dir = key > cur->key;
            if (!cur->ch[dir]) {
                cur->ch[dir] = new Node(key);
                cur->ch[dir]->p = cur;
                splay(cur->ch[dir]);
                return;
            }
            cur = cur->ch[dir];
        }
    }

    int kth(int k) {
        Ptr cur = root;
        while (cur) {
            int left_size = size(cur->ch[0]);
            if (k == left_size + 1) {
                splay(cur);
                return cur->key;
            }
            if (k <= left_size) cur = cur->ch[0];
            else {
                k -= left_size + 1;
                cur = cur->ch[1];
            }
        }
        throw runtime_error("k is out of range");
    }

    void erase(int key) {
        Ptr target = find_node(key);
        if (!target) return;

        Ptr left = target->ch[0];
        Ptr right = target->ch[1];
        if (left) left->p = nullptr;
        if (right) right->p = nullptr;
        delete target;

        if (!left) {
            root = right;
            return;
        }

        root = left;
        Ptr cur = root;
        while (cur->ch[1]) cur = cur->ch[1];
        splay(cur);
        root->ch[1] = right;
        if (right) right->p = root;
        pull(root);
    }
};
