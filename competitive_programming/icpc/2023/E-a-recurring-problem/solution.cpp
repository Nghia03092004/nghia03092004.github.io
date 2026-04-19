#include <bits/stdc++.h>
using namespace std;

namespace {

using int64 = long long;
constexpr int64 CAP = (1LL << 60);

struct Key {
    int len = 0;
    array<int64, 5> x{};
    array<int64, 5> a{};

    bool operator==(const Key& other) const {
        return len == other.len && x == other.x && a == other.a;
    }
};

struct KeyHash {
    size_t operator()(const Key& key) const {
        uint64_t hash = key.len;
        auto mix = [&](uint64_t value) {
            hash ^= value + 0x9e3779b97f4a7c15ULL + (hash << 6) + (hash >> 2);
        };
        for (int i = 0; i < 5; ++i) {
            mix(static_cast<uint64_t>(key.x[i]));
        }
        for (int i = 0; i < 5; ++i) {
            mix(static_cast<uint64_t>(key.a[i]));
        }
        return static_cast<size_t>(hash);
    }
};

unordered_map<Key, int64, KeyHash> memo_count;

int64 count_state(int len, const array<int64, 5>& x, const array<int64, 5>& a) {
    for (int i = 0; i < len; ++i) {
        if (x[i] < 0) {
            return 0;
        }
    }

    bool all_zero = true;
    for (int i = 0; i < len; ++i) {
        if (x[i] != 0) {
            all_zero = false;
            break;
        }
    }
    if (all_zero) {
        return 1;
    }

    if (len > 1 && count_state(len - 1, x, a) == 0) {
        return 0;
    }

    Key key;
    key.len = len;
    key.x = x;
    key.a = a;
    unordered_map<Key, int64, KeyHash>::const_iterator it = memo_count.find(key);
    if (it != memo_count.end()) {
        return it->second;
    }

    int64 total = 0;
    for (int64 a0 = 1; a0 <= x[0]; ++a0) {
        int64 max_coeff = x[0] / a0;
        for (int i = 1; i < len; ++i) {
            max_coeff = min(max_coeff, x[i] / a[i - 1]);
        }

        array<int64, 5> next_a{};
        if (len > 1) {
            next_a[0] = a0;
            for (int i = 1; i < len - 1; ++i) {
                next_a[i] = a[i - 1];
            }
        }

        for (int64 coeff = 1; coeff <= max_coeff; ++coeff) {
            array<int64, 5> next_x = x;
            next_x[0] -= coeff * a0;
            for (int i = 1; i < len; ++i) {
                next_x[i] -= coeff * a[i - 1];
            }
            total += count_state(len, next_x, next_a);
            if (total > CAP) {
                total = CAP;
                break;
            }
        }
        if (total == CAP) {
            break;
        }
    }

    memo_count[key] = total;
    return total;
}

int64 count_prefix(const vector<int64>& x, const vector<int64>& a) {
    array<int64, 5> xx{};
    array<int64, 5> aa{};
    int len = static_cast<int>(x.size());
    for (int i = 0; i < len; ++i) {
        xx[i] = x[i];
    }
    for (int i = 0; i + 1 < len; ++i) {
        aa[i] = a[i];
    }
    return count_state(len, xx, aa);
}

bool reconstruct_unique(int len, const array<int64, 5>& x, const array<int64, 5>& a,
                        vector<int64>& coeff_rev, vector<int64>& init_rev) {
    bool all_zero = true;
    for (int i = 0; i < len; ++i) {
        if (x[i] != 0) {
            all_zero = false;
            break;
        }
    }
    if (all_zero) {
        return true;
    }

    for (int64 a0 = 1; a0 <= x[0]; ++a0) {
        int64 max_coeff = x[0] / a0;
        for (int i = 1; i < len; ++i) {
            max_coeff = min(max_coeff, x[i] / a[i - 1]);
        }

        array<int64, 5> next_a{};
        if (len > 1) {
            next_a[0] = a0;
            for (int i = 1; i < len - 1; ++i) {
                next_a[i] = a[i - 1];
            }
        }

        for (int64 coeff = 1; coeff <= max_coeff; ++coeff) {
            array<int64, 5> next_x = x;
            next_x[0] -= coeff * a0;
            for (int i = 1; i < len; ++i) {
                next_x[i] -= coeff * a[i - 1];
            }
            if (count_state(len, next_x, next_a) > 0) {
                coeff_rev.push_back(coeff);
                init_rev.push_back(a0);
                return reconstruct_unique(len, next_x, next_a, coeff_rev, init_rev);
            }
        }
    }
    return false;
}

void enumerate_all(int len, const array<int64, 5>& x, const array<int64, 5>& a,
                   vector<int64>& coeff_rev, vector<int64>& init_rev,
                   vector<pair<vector<int64>, vector<int64>>>& out) {
    bool all_zero = true;
    for (int i = 0; i < len; ++i) {
        if (x[i] != 0) {
            all_zero = false;
            break;
        }
    }
    if (all_zero) {
        vector<int64> coeffs(coeff_rev.rbegin(), coeff_rev.rend());
        vector<int64> initials(init_rev.rbegin(), init_rev.rend());
        out.push_back(make_pair(coeffs, initials));
        return;
    }

    for (int64 a0 = 1; a0 <= x[0]; ++a0) {
        int64 max_coeff = x[0] / a0;
        for (int i = 1; i < len; ++i) {
            max_coeff = min(max_coeff, x[i] / a[i - 1]);
        }

        array<int64, 5> next_a{};
        if (len > 1) {
            next_a[0] = a0;
            for (int i = 1; i < len - 1; ++i) {
                next_a[i] = a[i - 1];
            }
        }

        for (int64 coeff = 1; coeff <= max_coeff; ++coeff) {
            array<int64, 5> next_x = x;
            next_x[0] -= coeff * a0;
            for (int i = 1; i < len; ++i) {
                next_x[i] -= coeff * a[i - 1];
            }
            if (count_state(len, next_x, next_a) == 0) {
                continue;
            }
            coeff_rev.push_back(coeff);
            init_rev.push_back(a0);
            enumerate_all(len, next_x, next_a, coeff_rev, init_rev, out);
            coeff_rev.pop_back();
            init_rev.pop_back();
        }
    }
}

int64 exact_count(const vector<int64>& generated_prefix) {
    vector<int64> prev_terms;
    if (!generated_prefix.empty()) {
        prev_terms.assign(generated_prefix.begin(), generated_prefix.end() - 1);
    }
    return count_prefix(generated_prefix, prev_terms);
}

vector<pair<int64, int64>> next_value_counts(const vector<int64>& prefix) {
    vector<pair<int64, int64>> result;
    int64 total = exact_count(prefix);
    int64 seen = 0;

    vector<int64> prev_terms = prefix;
    for (int64 value = prefix.back(); seen < total; ++value) {
        vector<int64> extended = prefix;
        extended.push_back(value);
        int64 ways = count_prefix(extended, prev_terms);
        if (ways > 0) {
            result.push_back(make_pair(value, ways));
            seen += ways;
        }
    }
    return result;
}

vector<int64> generated_terms(const vector<int64>& coeffs, const vector<int64>& initials, int count) {
    vector<int64> seq = initials;
    while (static_cast<int>(seq.size()) < static_cast<int>(coeffs.size()) + count) {
        int64 next = 0;
        for (int i = 0; i < static_cast<int>(coeffs.size()); ++i) {
            next += coeffs[i] * seq[seq.size() - coeffs.size() + i];
        }
        seq.push_back(next);
    }
    return vector<int64>(seq.begin() + coeffs.size(), seq.begin() + coeffs.size() + count);
}

void solve() {
    int64 n;
    cin >> n;

    memo_count.clear();
    memo_count.reserve(1 << 20);

    vector<int64> prefix;
    int64 skipped = 0;
    for (int64 first = 1;; ++first) {
        vector<int64> current(1, first);
        int64 ways = exact_count(current);
        if (skipped + ways >= n) {
            prefix = current;
            n -= skipped;
            break;
        }
        skipped += ways;
    }

    int64 branch_count = exact_count(prefix);
    while (branch_count > 1 && prefix.size() < 5) {
        vector<pair<int64, int64>> choices = next_value_counts(prefix);
        skipped = 0;
        for (int i = 0; i < static_cast<int>(choices.size()); ++i) {
            if (skipped + choices[i].second >= n) {
                prefix.push_back(choices[i].first);
                n -= skipped;
                branch_count = choices[i].second;
                break;
            }
            skipped += choices[i].second;
        }
    }

    vector<pair<vector<int64>, vector<int64>>> candidates;
    vector<int64> coeff_rev;
    vector<int64> init_rev;

    if (branch_count == 1) {
        array<int64, 5> x{};
        array<int64, 5> a{};
        for (int i = 0; i < static_cast<int>(prefix.size()); ++i) {
            x[i] = prefix[i];
        }
        for (int i = 0; i + 1 < static_cast<int>(prefix.size()); ++i) {
            a[i] = prefix[i];
        }
        reconstruct_unique(static_cast<int>(prefix.size()), x, a, coeff_rev, init_rev);
        candidates.push_back(make_pair(vector<int64>(coeff_rev.rbegin(), coeff_rev.rend()),
                                       vector<int64>(init_rev.rbegin(), init_rev.rend())));
    } else {
        array<int64, 5> x{};
        array<int64, 5> a{};
        for (int i = 0; i < static_cast<int>(prefix.size()); ++i) {
            x[i] = prefix[i];
        }
        for (int i = 0; i + 1 < static_cast<int>(prefix.size()); ++i) {
            a[i] = prefix[i];
        }
        enumerate_all(static_cast<int>(prefix.size()), x, a, coeff_rev, init_rev, candidates);
        int max_order = 0;
        for (int i = 0; i < static_cast<int>(candidates.size()); ++i) {
            max_order = max(max_order, static_cast<int>(candidates[i].first.size()));
        }
        int compare_terms = max(10, 2 * max_order);
        vector<pair<vector<int64>, vector<int64>>> ranked;
        ranked.reserve(candidates.size());
        for (int i = 0; i < static_cast<int>(candidates.size()); ++i) {
            ranked.push_back(make_pair(candidates[i].first,
                                       generated_terms(candidates[i].first, candidates[i].second, compare_terms)));
        }
        sort(ranked.begin(), ranked.end(),
             [](const pair<vector<int64>, vector<int64>>& lhs,
                const pair<vector<int64>, vector<int64>>& rhs) {
                 if (lhs.second != rhs.second) {
                     return lhs.second < rhs.second;
                 }
                 return lhs.first < rhs.first;
             });

        pair<vector<int64>, vector<int64>> picked = candidates[0];
        picked.first = ranked[n - 1].first;
        for (int i = 0; i < static_cast<int>(candidates.size()); ++i) {
            if (candidates[i].first == picked.first) {
                picked.second = candidates[i].second;
                break;
            }
        }
        candidates.clear();
        candidates.push_back(picked);
    }

    const vector<int64>& coeffs = candidates[0].first;
    const vector<int64>& initials = candidates[0].second;

    cout << coeffs.size() << '\n';
    for (int i = 0; i < static_cast<int>(coeffs.size()); ++i) {
        if (i) {
            cout << ' ';
        }
        cout << coeffs[i];
    }
    cout << '\n';
    for (int i = 0; i < static_cast<int>(initials.size()); ++i) {
        if (i) {
            cout << ' ';
        }
        cout << initials[i];
    }
    cout << '\n';

    vector<int64> first_ten = generated_terms(coeffs, initials, 10);
    for (int i = 0; i < 10; ++i) {
        if (i) {
            cout << ' ';
        }
        cout << first_ten[i];
    }
    cout << '\n';
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
