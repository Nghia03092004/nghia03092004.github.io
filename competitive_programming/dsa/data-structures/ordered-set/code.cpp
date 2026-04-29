using namespace __gnu_pbds;

template <class T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

struct OrderedMultiset {
    ordered_set<pair<int, int>> os;
    int next_id = 0;

    void insert(int x) {
        os.insert({x, next_id++});
    }

    bool erase_one(int x) {
        auto it = os.lower_bound({x, -1});
        if (it == os.end() || it->first != x) {
            return false;
        }
        os.erase(it);
        return true;
    }

    int order_of_key(int x) const {
        return os.order_of_key({x, -1});
    }

    int kth(int k) const {
        return os.find_by_order(k)->first;
    }

    int size() const {
        return (int)os.size();
    }
};
