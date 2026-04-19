#include <bits/stdc++.h>
using namespace std;

namespace {

struct Tablet {
    int index;
    int balance;
    int min_prefix;
};

bool appendable(int current_balance, const Tablet& tablet) {
    return current_balance + tablet.min_prefix >= 0;
}

void solve() {
    int n;
    cin >> n;

    vector<Tablet> positive;
    vector<Tablet> negative;
    positive.reserve(n);
    negative.reserve(n);

    long long total_balance = 0;

    for (int i = 1; i <= n; ++i) {
        string s;
        cin >> s;

        int balance = 0;
        int min_prefix = 0;
        for (char ch : s) {
            balance += (ch == '(' ? 1 : -1);
            min_prefix = min(min_prefix, balance);
        }

        total_balance += balance;
        Tablet tablet = {i, balance, min_prefix};
        if (balance >= 0) {
            positive.push_back(tablet);
        } else {
            negative.push_back(tablet);
        }
    }

    if (total_balance != 0) {
        cout << "impossible\n";
        return;
    }

    sort(positive.begin(), positive.end(), [](const Tablet& lhs, const Tablet& rhs) {
        return lhs.min_prefix > rhs.min_prefix;
    });

    sort(negative.begin(), negative.end(), [](const Tablet& lhs, const Tablet& rhs) {
        return lhs.balance - lhs.min_prefix > rhs.balance - rhs.min_prefix;
    });

    vector<int> order;
    order.reserve(n);

    int current_balance = 0;
    for (const Tablet& tablet : positive) {
        if (!appendable(current_balance, tablet)) {
            cout << "impossible\n";
            return;
        }
        current_balance += tablet.balance;
        order.push_back(tablet.index);
    }

    for (const Tablet& tablet : negative) {
        if (!appendable(current_balance, tablet)) {
            cout << "impossible\n";
            return;
        }
        current_balance += tablet.balance;
        order.push_back(tablet.index);
    }

    if (current_balance != 0) {
        cout << "impossible\n";
        return;
    }

    for (int index : order) {
        cout << index << '\n';
    }
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
