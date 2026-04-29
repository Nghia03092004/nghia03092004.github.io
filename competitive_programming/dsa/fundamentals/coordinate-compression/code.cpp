template <class T>
struct CoordinateCompression {
    vector<T> values;

    CoordinateCompression() = default;

    explicit CoordinateCompression(vector<T> values) : values(move(values)) {
        sort(this->values.begin(), this->values.end());
        this->values.erase(unique(this->values.begin(), this->values.end()), this->values.end());
    }

    int index(const T& x) const {
        return (int)(lower_bound(values.begin(), values.end(), x) - values.begin());
    }

    T value(int idx) const {
        return values[idx];
    }

    int size() const {
        return (int)values.size();
    }
};

template <class T>
pair<vector<int>, vector<T>> compress_values(const vector<T>& a) {
    CoordinateCompression<T> cc(a);
    vector<int> compressed;
    compressed.reserve(a.size());
    for (const T& x : a) {
        compressed.push_back(cc.index(x));
    }
    return {compressed, cc.values};
}
