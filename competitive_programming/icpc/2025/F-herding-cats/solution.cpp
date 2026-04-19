#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        int n, m;
        cin >> n >> m;

        vector<int> need_at(m + 1, 0);
        vector<vector<int>> cats_at(m + 1);
        vector<vector<int>> like(n);

        for (int i = 0; i < n; ++i) {
            int p, k;
            cin >> p >> k;
            cats_at[p].push_back(i);
            like[i].resize(k);
            for (int j = 0; j < k; ++j) {
                cin >> like[i][j];
                need_at[like[i][j]] = max(need_at[like[i][j]], p);
            }
        }

        vector<int> bucket(m + 1, 0);
        for (int plant = 1; plant <= m; ++plant) {
            bucket[need_at[plant]]++;
        }

        bool ok = true;
        long long seen = bucket[0];
        for (int pos = 1; pos <= m; ++pos) {
            seen += bucket[pos];
            if (seen < pos) {
                ok = false;
                break;
            }
        }

        vector<int> freq(m + 1, 0), touched;
        for (int pos = 1; pos <= m && ok; ++pos) {
            int group = (int)cats_at[pos].size();
            if (group == 0) {
                continue;
            }
            touched.clear();
            for (int cat : cats_at[pos]) {
                for (int plant : like[cat]) {
                    if (freq[plant] == 0) {
                        touched.push_back(plant);
                    }
                    freq[plant]++;
                }
            }
            bool found = false;
            for (int plant : touched) {
                if (freq[plant] == group && need_at[plant] == pos) {
                    found = true;
                }
                freq[plant] = 0;
            }
            if (!found) {
                ok = false;
            }
        }

        cout << (ok ? "yes" : "no") << '\n';
    }

    return 0;
}
