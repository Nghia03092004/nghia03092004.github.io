#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    int k;
    cin >> k;

    vector<int> offset(n + 1, 0);

    auto ask_rotate = [&](int wheel, int delta) {
        cout << wheel << ' ' << delta << '\n' << flush;
        offset[wheel] = ((offset[wheel] + delta) % n + n) % n;
        if (!(cin >> k)) {
            exit(0);
        }
        if (k == 1) {
            exit(0);
        }
    };

    auto rotate_to = [&](int wheel, int target) {
        int cur = offset[wheel];
        int delta = target - cur;
        delta %= n;
        if (delta < 0) {
            delta += n;
        }
        if (delta > n - delta) {
            delta -= n;
        }
        if (delta != 0) {
            ask_rotate(wheel, delta);
        }
    };

    auto scan_wheel = [&](int wheel) {
        vector<int> val(n);
        val[0] = k;
        for (int i = 1; i < n; ++i) {
            ask_rotate(wheel, 1);
            val[i] = k;
        }
        ask_rotate(wheel, 1);
        int best = *min_element(val.begin(), val.end());
        vector<int> mask(n, 0);
        for (int i = 0; i < n; ++i) {
            if (val[i] == best) {
                mask[i] = 1;
            }
        }
        return mask;
    };

    vector<int> first_mask = scan_wheel(1);
    int target_symbol = -1;
    int common_symbol = -1;
    for (int i = 0; i < n; ++i) {
        if (first_mask[i] == 0 && target_symbol == -1) {
            target_symbol = i;
        }
        if (first_mask[i] == 1 && common_symbol == -1) {
            common_symbol = i;
        }
    }
    if (target_symbol == -1) {
        target_symbol = 0;
    }
    if (common_symbol == -1) {
        common_symbol = (target_symbol + 1) % n;
    }

    vector<int> desired(n + 1, 0);
    desired[1] = target_symbol;
    rotate_to(1, target_symbol);

    for (int wheel = 2; wheel <= n; ++wheel) {
        vector<int> mask_with_target = scan_wheel(wheel);
        rotate_to(1, common_symbol);
        vector<int> mask_with_common = scan_wheel(wheel);

        int pos = -1;
        for (int i = 0; i < n; ++i) {
            if (mask_with_target[i] == 1 && mask_with_common[i] == 0) {
                pos = i;
                break;
            }
        }
        if (pos == -1) {
            for (int i = 0; i < n; ++i) {
                if (mask_with_target[i] != mask_with_common[i]) {
                    pos = i;
                    break;
                }
            }
        }
        if (pos == -1) {
            pos = 0;
        }
        desired[wheel] = pos;
        rotate_to(1, target_symbol);
    }

    for (int wheel = 1; wheel <= n; ++wheel) {
        rotate_to(wheel, desired[wheel]);
    }
    return 0;
}
