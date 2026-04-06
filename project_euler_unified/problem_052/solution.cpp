#include <bits/stdc++.h>
using namespace std;

string sorted_digits(int n) {
    string s = to_string(n);
    sort(s.begin(), s.end());
    return s;
}

int main() {
    for (int x = 1; ; x++) {
        string sd = sorted_digits(x);
        bool ok = true;
        for (int k = 2; k <= 6; k++) {
            if (sorted_digits(k * x) != sd) {
                ok = false;
                break;
            }
        }
        if (ok) {
            cout << x << endl;
            return 0;
        }
    }
    return 0;
}
