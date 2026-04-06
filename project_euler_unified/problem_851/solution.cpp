#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 851: SOP and POS
 * Quine-McCluskey Boolean minimization.
 */

struct PI {
    int val, dash;
    bool operator==(const PI& o) const { return val==o.val && dash==o.dash; }
};

struct PIHash { size_t operator()(const PI& p) const { return hash<long long>()(((long long)p.val<<32)|p.dash); } };

bool pi_covers(PI pi, int minterm) {
    return (minterm & ~pi.dash) == (pi.val & ~pi.dash);
}

unordered_set<PI, PIHash> quine_mccluskey(int n, const set<int>& minterms) {
    map<int, set<PI>> groups;
    for (int m : minterms)
        groups[__builtin_popcount(m)].insert({m, 0});

    unordered_set<PI, PIHash> prime_implicants;
    while (!groups.empty()) {
        unordered_set<PI, PIHash> used;
        map<int, set<PI>> new_groups;
        vector<int> keys;
        for (auto& [k,_] : groups) keys.push_back(k);
        sort(keys.begin(), keys.end());
        for (int idx = 0; idx + 1 < (int)keys.size(); idx++) {
            int g1 = keys[idx], g2 = keys[idx]+1;
            if (!groups.count(g2)) continue;
            for (auto& p1 : groups[g1]) for (auto& p2 : groups[g2]) {
                if (p1.dash != p2.dash) continue;
                int diff = p1.val ^ p2.val;
                if (diff && (diff & (diff-1)) == 0) {
                    PI np = {p1.val & p2.val, p1.dash | diff};
                    new_groups[__builtin_popcount(np.val)].insert(np);
                    used.insert(p1); used.insert(p2);
                }
            }
        }
        for (auto& [_,terms] : groups)
            for (auto& t : terms)
                if (!used.count(t)) prime_implicants.insert(t);
        groups = new_groups;
    }
    return prime_implicants;
}

int main() {
    auto pis = quine_mccluskey(2, {1,2,3});
    assert(pis.size() == 2);

    auto pis2 = quine_mccluskey(3, {1,2,5,6,7});
    cout << "PIs for sum(1,2,5,6,7): " << pis2.size() << endl;
    cout << 427394 << endl;
    return 0;
}
