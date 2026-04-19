#include <bits/stdc++.h>
using namespace std;

namespace {

vector<int> even_palindrome_radii(const string& s) {
    int n = int(s.size());
    vector<int> d2(n, 0);
    int left = 0;
    int right = -1;

    for (int center = 0; center < n; ++center) {
        int radius = 0;
        if (center <= right) {
            int mirror = left + right - center + 1;
            radius = min(d2[mirror], right - center + 1);
        }
        while (center - radius - 1 >= 0 &&
               center + radius < n &&
               s[center - radius - 1] == s[center + radius]) {
            ++radius;
        }
        d2[center] = radius;
        if (center + radius - 1 > right) {
            left = center - radius;
            right = center + radius - 1;
        }
    }
    return d2;
}

int max_left_reach(const string& s) {
    vector<int> d2 = even_palindrome_radii(s);
    int reach = 0;
    int n = int(s.size());

    for (int center = 1; center < n; ++center) {
        if (center - d2[center] <= reach) {
            reach = center;
        }
    }
    return reach;
}

void solve() {
    string s;
    cin >> s;

    int left_removed = max_left_reach(s);
    string remaining = s.substr(left_removed);
    reverse(remaining.begin(), remaining.end());
    int right_removed = max_left_reach(remaining);

    cout << int(remaining.size()) - right_removed << '\n';
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
