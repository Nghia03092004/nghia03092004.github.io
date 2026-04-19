#include <bits/stdc++.h>
using namespace std;

namespace {

using cd = complex<double>;
const double PI = acos(-1.0);

void fft(vector<cd>& a, bool invert) {
    int n = static_cast<int>(a.size());

    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) {
            j ^= bit;
        }
        j ^= bit;
        if (i < j) {
            swap(a[i], a[j]);
        }
    }

    for (int len = 2; len <= n; len <<= 1) {
        double angle = 2.0 * PI / len * (invert ? -1.0 : 1.0);
        cd wlen(cos(angle), sin(angle));
        for (int i = 0; i < n; i += len) {
            cd w(1.0, 0.0);
            for (int j = 0; j < len / 2; ++j) {
                cd u = a[i + j];
                cd v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }

    if (invert) {
        for (cd& value : a) {
            value /= n;
        }
    }
}

vector<double> convolution(const vector<double>& a, const vector<double>& b) {
    int n = 1;
    while (n < static_cast<int>(a.size() + b.size() - 1)) {
        n <<= 1;
    }

    vector<cd> fa(n), fb(n);
    for (int i = 0; i < static_cast<int>(a.size()); ++i) {
        fa[i] = a[i];
    }
    for (int i = 0; i < static_cast<int>(b.size()); ++i) {
        fb[i] = b[i];
    }

    fft(fa, false);
    fft(fb, false);
    for (int i = 0; i < n; ++i) {
        fa[i] *= fb[i];
    }
    fft(fa, true);

    vector<double> result(a.size() + b.size() - 1);
    for (int i = 0; i < static_cast<int>(result.size()); ++i) {
        result[i] = fa[i].real();
    }
    return result;
}

void solve() {
    int rp, cp;
    cin >> rp >> cp;

    vector<vector<int>> pattern(rp, vector<int>(cp));
    for (int i = 0; i < rp; ++i) {
        for (int j = 0; j < cp; ++j) {
            cin >> pattern[i][j];
        }
    }

    int rq, cq;
    cin >> rq >> cq;
    vector<vector<int>> mosaic(rq, vector<int>(cq));
    for (int i = 0; i < rq; ++i) {
        for (int j = 0; j < cq; ++j) {
            cin >> mosaic[i][j];
        }
    }

    if (rp > rq || cp > cq) {
        cout << 0 << '\n';
        return;
    }

    int text_len = rq * cq;
    int pattern_len = (rp - 1) * cq + cp;

    vector<double> text(text_len), text_sq(text_len);
    for (int i = 0; i < rq; ++i) {
        for (int j = 0; j < cq; ++j) {
            int idx = i * cq + j;
            text[idx] = mosaic[i][j];
            text_sq[idx] = static_cast<double>(mosaic[i][j]) * mosaic[i][j];
        }
    }

    vector<double> pattern_flat(pattern_len, 0.0);
    vector<double> pattern_sq(pattern_len, 0.0);
    long long pattern_cube_sum = 0;
    for (int i = 0; i < rp; ++i) {
        for (int j = 0; j < cp; ++j) {
            int idx = i * cq + j;
            int value = pattern[i][j];
            pattern_flat[idx] = value;
            pattern_sq[idx] = static_cast<double>(value) * value;
            pattern_cube_sum += 1LL * value * value * value;
        }
    }

    reverse(pattern_flat.begin(), pattern_flat.end());
    reverse(pattern_sq.begin(), pattern_sq.end());

    vector<double> term1 = convolution(pattern_flat, text_sq);
    vector<double> term2 = convolution(pattern_sq, text);

    vector<pair<int, int>> matches;
    int offset = pattern_len - 1;
    for (int r = 0; r + rp <= rq; ++r) {
        for (int c = 0; c + cp <= cq; ++c) {
            int start = r * cq + c;
            double score = term1[start + offset] - 2.0 * term2[start + offset] + pattern_cube_sum;
            if (fabs(score) < 0.5) {
                matches.push_back({r + 1, c + 1});
            }
        }
    }

    cout << matches.size() << '\n';
    for (const pair<int, int>& match : matches) {
        cout << match.first << ' ' << match.second << '\n';
    }
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
