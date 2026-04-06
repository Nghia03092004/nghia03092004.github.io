#include <bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

// Get all divisors of n greater than 1
vector<int> get_divisors(int n) {
    vector<int> divs;
    for (int i = 2; (long long)i * i <= n; i++) {
        if (n % i == 0) {
            divs.push_back(i);
            if (i != n / i) divs.push_back(n / i);
        }
    }
    divs.push_back(n);
    sort(divs.begin(), divs.end());
    return divs;
}

// Memoized W(n, k, max_factor)
map<tuple<int,int,int>, long long> memo;

long long W(int n, int k, int max_factor) {
    if (k == 0) return (n == 1) ? 1 : 0;
    if (n < 2) return 0;

    auto key = make_tuple(n, k, max_factor);
    auto it = memo.find(key);
    if (it != memo.end()) return it->second;

    long long count = 0;
    vector<int> divs = get_divisors(n);
    for (int d : divs) {
        if (d > max_factor) break;
        count += W(n / d, k - 1, d - 1);
    }

    memo[key] = count;
    return count;
}

long long total_factorizations(int n) {
    long long total = 0;
    for (int k = 1; ; k++) {
        long long w = W(n, k, n);
        if (w == 0) break;
        total += w;
    }
    return total;
}

int main() {
    int N = 1000;
    long long result = 0;
    for (int n = 2; n <= N; n++) {
        result += total_factorizations(n);
    }
    cout << result << endl;
    return 0;
}
