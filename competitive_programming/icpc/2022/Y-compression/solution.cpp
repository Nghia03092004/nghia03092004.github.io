#include <bits/stdc++.h>
using namespace std;

namespace {

void solve() {
    string s;
    cin >> s;

    bool all_same = true;
    for (char ch : s) {
        if (ch != s[0]) {
            all_same = false;
            break;
        }
    }

    if (all_same) {
        cout << s[0] << '\n';
        return;
    }

    if (s.front() != s.back()) {
        cout << s.front() << s.back() << '\n';
        return;
    }

    char middle = (s.front() == '0' ? '1' : '0');
    cout << s.front() << middle << s.back() << '\n';
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
