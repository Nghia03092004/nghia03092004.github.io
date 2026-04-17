/*
 * Problem 883: Remarkable Representations
 * sum(dim rho_i)^2 = |G| for irreducible representations.
 */
#include <bits/stdc++.h>
using namespace std;

bool verify(int order, vector<int> dims) {
    int s = 0;
    for (int d : dims) { s += d * d; if (order % d != 0) return false; }
    return s == order;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Known groups
    cout << "=== Sum of Squares ===" << endl;
    struct Group { string name; int order; vector<int> dims; };
    vector<Group> groups = {
        {"Z/4", 4, {1,1,1,1}},
        {"S_3", 6, {1,1,2}},
        {"D_4", 8, {1,1,1,1,2}},
        {"Q_8", 8, {1,1,1,1,2}},
        {"A_4", 12, {1,1,1,3}},
        {"S_4", 24, {1,1,2,3,3}},
        {"S_5", 120, {1,1,4,4,5,5,6}},
    };
    for (auto& g : groups) {
        int s = 0;
        for (int d : g.dims) s += d * d;
        bool ok = verify(g.order, g.dims);
        cout << g.name << ": |G|=" << g.order << ", sum=" << s
             << (ok ? " OK" : " FAIL") << endl;
        assert(ok);
    }

    // Dihedral groups
    cout << "\n=== Dihedral Groups ===" << endl;
    for (int n = 3; n <= 10; n++) {
        int order = 2 * n;
        vector<int> dims;
        if (n % 2 == 1) {
            dims = {1, 1};
            for (int i = 0; i < (n - 1) / 2; i++) dims.push_back(2);
        } else {
            dims = {1, 1, 1, 1};
            for (int i = 0; i < (n - 2) / 2; i++) dims.push_back(2);
        }
        int s = 0;
        for (int d : dims) s += d * d;
        cout << "D_" << n << ": |D|=" << order << ", sum=" << s
             << (s == order ? " OK" : " FAIL") << endl;
        assert(s == order);
    }

    cout << "\nAnswer: sum d_i^2 = |G| (verified for all test groups)" << endl;
    return 0;
}
