#include <bits/stdc++.h>
using namespace std;

int main() {
    // Canonical representation: for each a, find primitive base c and power p
    // such that a = c^p. Then a^b = c^(p*b).
    vector<pair<int,int>> canon(101); // canon[a] = (base, power)

    for (int a = 2; a <= 100; a++) {
        canon[a] = {a, 1};
        for (int c = 2; c * c <= a; c++) {
            int val = c, k = 1;
            while (val < a) { val *= c; k++; }
            if (val == a) {
                auto [base, base_k] = canon[c];
                canon[a] = {base, base_k * k};
                break;
            }
        }
    }

    map<int, set<int>> exponents;
    for (int a = 2; a <= 100; a++) {
        auto [base, k] = canon[a];
        for (int b = 2; b <= 100; b++)
            exponents[base].insert(k * b);
    }

    int total = 0;
    for (auto& [base, s] : exponents)
        total += s.size();

    cout << total << endl;
    return 0;
}
