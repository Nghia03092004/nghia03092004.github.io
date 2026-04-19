#include <bits/stdc++.h>
using namespace std;

namespace {

vector<int> build_all_curves(int curves) {
    if (curves == 4) {
        return {1, 2, 3, 0};  // LLLL
    }

    int a = 2;
    int b = curves / 4 - 2;
    vector<int> dirs;
    dirs.reserve(curves);
    for (int i = 0; i < a; ++i) {
        dirs.push_back(1);
        dirs.push_back(0);
    }
    for (int i = 0; i < b; ++i) {
        dirs.push_back(3);
        dirs.push_back(0);
    }
    for (int i = 0; i < a; ++i) {
        dirs.push_back(3);
        dirs.push_back(2);
    }
    for (int i = 0; i < b; ++i) {
        dirs.push_back(1);
        dirs.push_back(2);
    }
    return dirs;
}

vector<int> build_mod0(int straights, int curves) {
    int a = straights / 2 + 1;
    int q = (curves - 4) / 4;

    vector<int> dirs;
    dirs.reserve(straights + curves);
    dirs.insert(dirs.end(), a, 1);  // N^a
    dirs.push_back(2);              // W
    for (int i = 0; i < q; ++i) {
        dirs.push_back(3);          // S
        dirs.push_back(2);          // W
    }
    dirs.insert(dirs.end(), a, 3);  // S^a
    dirs.push_back(0);              // E
    for (int i = 0; i < q; ++i) {
        dirs.push_back(1);          // N
        dirs.push_back(0);          // E
    }
    return dirs;
}

vector<int> build_mod2(int straights, int curves) {
    int a = 2;
    int b = straights / 2 + 1;
    int p = (curves - 6) / 4;

    vector<int> dirs;
    dirs.reserve(straights + curves);
    dirs.insert(dirs.end(), a, 1);  // N^2
    for (int i = 0; i < p; ++i) {
        dirs.push_back(2);          // W
        dirs.push_back(1);          // N
    }
    dirs.insert(dirs.end(), b, 2);  // W^b
    for (int i = 0; i < p + 1; ++i) {
        dirs.push_back(3);          // S
        dirs.push_back(0);          // E
    }
    dirs.push_back(3);              // S
    dirs.insert(dirs.end(), b - 1, 0);  // E^(b-1)
    return dirs;
}

string directions_to_tracks(const vector<int>& dirs) {
    string answer;
    answer.reserve(dirs.size());

    int prev = dirs.back();
    for (int dir : dirs) {
        if (dir == prev) {
            answer.push_back('S');
        } else if (dir == (prev + 1) % 4) {
            answer.push_back('L');
        } else {
            answer.push_back('R');
        }
        prev = dir;
    }
    return answer;
}

void solve() {
    int s, c;
    cin >> s >> c;

    int use_s = s - (s & 1);
    int use_c = c - (c & 1);

    vector<int> dirs;
    if (use_s < 2) {
        int use_curves;
        if (use_c < 12) {
            use_curves = 4;
        } else {
            use_curves = use_c - (use_c % 4);
        }
        dirs = build_all_curves(use_curves);
    } else if (use_c % 4 == 0) {
        dirs = build_mod0(use_s, use_c);
    } else {
        dirs = build_mod2(use_s, use_c);
    }

    cout << directions_to_tracks(dirs) << '\n';
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
