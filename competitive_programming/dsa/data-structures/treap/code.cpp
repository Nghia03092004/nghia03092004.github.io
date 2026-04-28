mt19937 rng((uint32_t)chrono::steady_clock::now().time_since_epoch().count());

struct Treap {
    struct Node {
        int key, prior, sz;
        Node *left, *right;
        explicit Node(int key_)
            : key(key_), prior((int)rng()), sz(1), left(nullptr), right(nullptr) {}
    };

    using Ptr = Node*;
    Ptr root = nullptr;

    static int size(Ptr node) { return node ? node->sz : 0; }

    static void pull(Ptr node) {
        if (!node) return;
        node->sz = 1 + size(node->left) + size(node->right);
    }

    static void split(Ptr node, int key, Ptr& left, Ptr& right) {
        if (!node) {
            left = right = nullptr;
        } else if (node->key < key) {
            split(node->right, key, node->right, right);
            left = node;
            pull(left);
        } else {
            split(node->left, key, left, node->left);
            right = node;
            pull(right);
        }
    }

    static Ptr merge(Ptr left, Ptr right) {
        if (!left || !right) return left ? left : right;
        if (left->prior > right->prior) {
            left->right = merge(left->right, right);
            pull(left);
            return left;
        }
        right->left = merge(left, right->left);
        pull(right);
        return right;
    }

    bool contains(int key) const {
        Ptr cur = root;
        while (cur) {
            if (cur->key == key) return true;
            cur = key < cur->key ? cur->left : cur->right;
        }
        return false;
    }

    void insert(int key) {
        if (contains(key)) return;
        Ptr left, right;
        split(root, key, left, right);
        root = merge(merge(left, new Node(key)), right);
    }

    static Ptr erase(Ptr node, int key) {
        if (!node) return nullptr;
        if (node->key == key) {
            Ptr merged = merge(node->left, node->right);
            delete node;
            return merged;
        }
        if (key < node->key) node->left = erase(node->left, key);
        else node->right = erase(node->right, key);
        pull(node);
        return node;
    }

    void erase(int key) {
        root = erase(root, key);
    }

    static int kth(Ptr node, int k) {
        int left_size = size(node->left);
        if (k == left_size + 1) return node->key;
        if (k <= left_size) return kth(node->left, k);
        return kth(node->right, k - left_size - 1);
    }

    int kth(int k) const {
        return kth(root, k);
    }

    static int order_of_key(Ptr node, int key) {
        if (!node) return 0;
        if (key <= node->key) return order_of_key(node->left, key);
        return size(node->left) + 1 + order_of_key(node->right, key);
    }

    int order_of_key(int key) const {
        return order_of_key(root, key);
    }
};
