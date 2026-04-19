#include <bits/stdc++.h>
using namespace std;

namespace {

struct BigInt {
    vector<uint64_t> limbs;

    void trim() {
        while (!limbs.empty() && limbs.back() == 0) {
            limbs.pop_back();
        }
    }

    bool is_zero() const {
        return limbs.empty();
    }

    void set_bit(int bit) {
        const int word = bit >> 6;
        const int off = bit & 63;
        if ((int)limbs.size() <= word) {
            limbs.resize(word + 1, 0);
        }
        limbs[word] |= (uint64_t(1) << off);
    }

    void add_small(uint64_t value) {
        if (value == 0) {
            return;
        }
        size_t i = 0;
        uint64_t carry = value;
        while (carry != 0) {
            if (i == limbs.size()) {
                limbs.push_back(0);
            }
            const uint64_t before = limbs[i];
            limbs[i] += carry;
            carry = (limbs[i] < before ? 1 : 0);
            ++i;
        }
    }

    void add(const BigInt& other) {
        const size_t n = max(limbs.size(), other.limbs.size());
        limbs.resize(n, 0);
        uint64_t carry = 0;
        for (size_t i = 0; i < n; ++i) {
            const uint64_t a = limbs[i];
            const uint64_t b = (i < other.limbs.size() ? other.limbs[i] : 0);
            limbs[i] = a + b + carry;
            if (carry == 0) {
                carry = (limbs[i] < a || limbs[i] < b) ? 1 : 0;
            } else {
                carry = (limbs[i] <= a || limbs[i] <= b) ? 1 : 0;
            }
        }
        if (carry != 0) {
            limbs.push_back(carry);
        }
    }

    BigInt shifted(int bits) const {
        if (is_zero()) {
            return {};
        }
        const int word_shift = bits >> 6;
        const int bit_shift = bits & 63;
        BigInt out;
        out.limbs.assign(word_shift + limbs.size() + 1, 0);
        uint64_t carry = 0;
        for (size_t i = 0; i < limbs.size(); ++i) {
            const uint64_t x = limbs[i];
            out.limbs[word_shift + i] |= (x << bit_shift);
            if (bit_shift != 0) {
                out.limbs[word_shift + i + 1] |= (x >> (64 - bit_shift));
            }
        }
        out.trim();
        return out;
    }

    int compare(const BigInt& other) const {
        if (limbs.size() != other.limbs.size()) {
            return limbs.size() < other.limbs.size() ? -1 : 1;
        }
        for (int i = (int)limbs.size() - 1; i >= 0; --i) {
            if (limbs[i] != other.limbs[i]) {
                return limbs[i] < other.limbs[i] ? -1 : 1;
            }
        }
        return 0;
    }

    string to_binary() const {
        if (is_zero()) {
            return "0";
        }
        const int last = (int)limbs.size() - 1;
        int highest = 63;
        while (((limbs[last] >> highest) & 1ULL) == 0) {
            --highest;
        }
        string out;
        out.reserve(last * 64 + highest + 1);
        for (int bit = highest; bit >= 0; --bit) {
            out.push_back(char('0' + ((limbs[last] >> bit) & 1ULL)));
        }
        for (int i = last - 1; i >= 0; --i) {
            for (int bit = 63; bit >= 0; --bit) {
                out.push_back(char('0' + ((limbs[i] >> bit) & 1ULL)));
            }
        }
        return out;
    }
};

int other_peg(int a, int b) {
    return 3 - a - b;
}

BigInt standard_cost(const vector<int>& pos, int goal) {
    BigInt ans;
    int target = goal;
    for (int disk = (int)pos.size() - 1; disk >= 1; --disk) {
        if (pos[disk] != target) {
            ans.set_bit(disk - 1);
            target = other_peg(pos[disk], target);
        }
    }
    return ans;
}

struct Move {
    bool exists = false;
    int disk = 0;
    int from = 0;
    int to = 0;
};

Move first_move_to_corner(const vector<int>& pos, int goal) {
    vector<pair<int, int>> frames;
    int target = goal;
    for (int disk = (int)pos.size() - 1; disk >= 2; --disk) {
        if (pos[disk] == target) {
            continue;
        }
        frames.push_back({disk, target});
        target = other_peg(pos[disk], target);
    }

    if ((int)pos.size() == 1) {
        return {};
    }
    if (pos[1] != target) {
        return {true, 1, pos[1], target};
    }
    if (frames.empty()) {
        return {};
    }
    const int disk = frames.back().first;
    const int dest = frames.back().second;
    return {true, disk, pos[disk], dest};
}

BigInt solve_case(int n, const string& k_bits) {
    string bits(n, '0');
    const int offset = n - (int)k_bits.size();
    for (int i = 0; i < (int)k_bits.size(); ++i) {
        bits[offset + i] = k_bits[i];
    }

    vector<int> before_pos(n + 1, 0);
    int src = 0;
    int aux = 1;
    int dst = 2;
    for (int disk = n, idx = 0; disk >= 1; --disk, ++idx) {
        if (bits[idx] == '0') {
            before_pos[disk] = src;
            const int new_aux = dst;
            dst = aux;
            aux = new_aux;
        } else {
            before_pos[disk] = dst;
            const int new_src = aux;
            aux = src;
            src = new_src;
        }
    }

    vector<int> rod0;
    vector<int> rod1;
    vector<int> rod2;
    rod0.reserve(n);
    rod1.reserve(n);
    rod2.reserve(n);
    for (int disk = n; disk >= 1; --disk) {
        if (before_pos[disk] == 0) {
            rod0.push_back(disk);
        } else if (before_pos[disk] == 1) {
            rod1.push_back(disk);
        } else {
            rod2.push_back(disk);
        }
    }
    rod2.insert(rod2.end(), rod0.begin(), rod0.end());
    rod0.clear();

    vector<int> pos(n + 1, 0);
    for (int disk : rod1) {
        pos[disk] = 1;
    }
    for (int disk : rod2) {
        pos[disk] = 2;
    }

    int bad = 0;
    for (int i = 0; i + 1 < (int)rod2.size(); ++i) {
        if (rod2[i] < rod2[i + 1]) {
            bad = rod2[i + 1];
            break;
        }
    }

    if (bad == 0) {
        return standard_cost(pos, 2);
    }

    vector<char> trapped(bad, false);
    int trapped_small = 0;
    for (int disk : rod2) {
        if (disk == bad) {
            break;
        }
        if (disk < bad) {
            trapped[disk] = true;
            ++trapped_small;
        }
    }

    vector<int> a_disks;
    a_disks.reserve(bad - 1 - trapped_small);
    for (int disk = 1; disk < bad; ++disk) {
        if (!trapped[disk]) {
            a_disks.push_back(disk);
        }
    }

    const int a_size = (int)a_disks.size();
    vector<int> pos_a(a_size + 1, 0);
    for (int i = 0; i < a_size; ++i) {
        pos_a[i + 1] = pos[a_disks[i]];
    }

    const int b_size = n - bad + 1;
    vector<int> pos_b(b_size + 1, 0);
    for (int disk = bad; disk <= n; ++disk) {
        pos_b[disk - bad + 1] = pos[disk];
    }

    auto build_full = [&](const vector<int>& current_b, int peg_a) {
        vector<int> full(n + 1, 0);
        for (int disk = 1; disk < bad; ++disk) {
            full[disk] = trapped[disk] ? 2 : peg_a;
        }
        for (int disk = bad; disk <= n; ++disk) {
            full[disk] = current_b[disk - bad + 1];
        }
        return full;
    };

    const BigInt cost_b_goal = standard_cost(pos_b, 2);
    if (cost_b_goal.is_zero()) {
        BigInt best;
        bool has_best = false;
        for (int peg_a : {0, 1}) {
            const int peg_d = other_peg(2, peg_a);
            vector<int> moved_b = pos_b;
            moved_b[1] = peg_d;

            BigInt total = standard_cost(pos_a, peg_a);
            total.add_small(1);
            total.add(standard_cost(build_full(moved_b, peg_a), 2));

            if (!has_best || total.compare(best) < 0) {
                best = total;
                has_best = true;
            }
        }
        return best;
    }

    const Move first = first_move_to_corner(pos_b, 2);
    const int init_spare = other_peg(first.from, first.to);
    BigInt answer = standard_cost(pos_a, init_spare);

    vector<int> after_b = pos_b;
    after_b[first.disk] = first.to;

    int final_spare = init_spare;
    if (first.disk == 1) {
        answer.add_small(1);
    } else {
        const Move second = first_move_to_corner(after_b, 2);
        after_b[second.disk] = second.to;
        final_spare = other_peg(second.from, second.to);
        answer.set_bit(a_size);
        answer.add_small(1);
    }

    answer.add(standard_cost(build_full(after_b, final_spare), 2));
    return answer;
}

void solve() {
    int n;
    string k;
    cin >> n >> k;
    cout << solve_case(n, k).to_binary() << '\n';
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
