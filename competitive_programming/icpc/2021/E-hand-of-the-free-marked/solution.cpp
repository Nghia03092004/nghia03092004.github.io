#include <bits/stdc++.h>
using namespace std;

namespace {

int k;
int m;
vector<long long> counts;
vector<int> chosen;
vector<long double> factorials;
long double total_combinations = 0.0L;
long double answer = 0.0L;

long double choose_ld(long long n, int r) {
    if (r < 0 || r > n) {
        return 0.0L;
    }
    r = min<long long>(r, n - r);
    long double result = 1.0L;
    for (int i = 1; i <= r; ++i) {
        result *= static_cast<long double>(n - r + i);
        result /= static_cast<long double>(i);
    }
    return result;
}

void dfs(int index, int remaining) {
    if (index == m) {
        if (remaining != 0) {
            return;
        }

        long double pattern_count = 1.0L;
        for (int i = 0; i < m; ++i) {
            pattern_count *= choose_ld(counts[i], chosen[i]);
        }

        long double left_size = 0.0L;
        for (int hidden_color = 0; hidden_color < m; ++hidden_color) {
            if (chosen[hidden_color] == 0) {
                continue;
            }

            long double visible_sets = 1.0L;
            for (int i = 0; i < m; ++i) {
                int need = chosen[i] - (i == hidden_color ? 1 : 0);
                visible_sets *= choose_ld(counts[i], need);
            }
            left_size += visible_sets;
        }
        left_size *= factorials[k - 1];

        long double success_fraction = min(1.0L, left_size / pattern_count);
        answer += (pattern_count / total_combinations) * success_fraction;
        return;
    }

    int limit = min<long long>(counts[index], remaining);
    for (int take = 0; take <= limit; ++take) {
        chosen[index] = take;
        dfs(index + 1, remaining - take);
    }
}

void solve() {
    cin >> k >> m;
    counts.resize(m);
    long long n = 0;
    for (int i = 0; i < m; ++i) {
        cin >> counts[i];
        n += counts[i];
    }

    factorials.assign(k + 1, 1.0L);
    for (int i = 1; i <= k; ++i) {
        factorials[i] = factorials[i - 1] * static_cast<long double>(i);
    }

    total_combinations = choose_ld(n, k);
    chosen.assign(m, 0);
    answer = 0.0L;
    dfs(0, k);

    cout << fixed << setprecision(12) << static_cast<double>(answer) << '\n';
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
