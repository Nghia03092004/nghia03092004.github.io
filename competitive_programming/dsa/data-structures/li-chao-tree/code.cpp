struct LiChaoTree {
    struct Line {
        long long m;
        long long b;

        long long eval(long long x) const {
            return m * x + b;
        }
    };

    struct Node {
        Line line;
        bool has_line = false;
        Node* left = nullptr;
        Node* right = nullptr;
    };

    long long lo;
    long long hi;
    Node* root = nullptr;

    LiChaoTree(long long lo, long long hi) : lo(lo), hi(hi) {}

    void add_line(Line nw) {
        add_line(root, lo, hi, nw);
    }

    long long query(long long x) const {
        return query(root, lo, hi, x);
    }

private:
    static constexpr long long INF = (long long)4e18;

    void add_line(Node*& node, long long l, long long r, Line nw) {
        if (!node) {
            node = new Node();
        }
        if (!node->has_line) {
            node->line = nw;
            node->has_line = true;
            return;
        }

        long long mid = l + (r - l) / 2;
        bool left_better = nw.eval(l) < node->line.eval(l);
        bool mid_better = nw.eval(mid) < node->line.eval(mid);

        if (mid_better) {
            swap(node->line, nw);
        }
        if (l == r) {
            return;
        }
        if (left_better != mid_better) {
            add_line(node->left, l, mid, nw);
        } else {
            add_line(node->right, mid + 1, r, nw);
        }
    }

    long long query(Node* node, long long l, long long r, long long x) const {
        if (!node) {
            return INF;
        }
        long long answer = node->has_line ? node->line.eval(x) : INF;
        if (l == r) {
            return answer;
        }
        long long mid = l + (r - l) / 2;
        if (x <= mid) {
            return min(answer, query(node->left, l, mid, x));
        }
        return min(answer, query(node->right, mid + 1, r, x));
    }
};
