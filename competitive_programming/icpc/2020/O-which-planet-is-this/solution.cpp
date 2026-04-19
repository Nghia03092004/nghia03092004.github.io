#include <bits/stdc++.h>
using namespace std;

namespace {

const long long CIRCLE = 3600000;
const long long START_TOKEN = -1;
const long long SEP_TOKEN = -2;
const long long END_TOKEN = -3;
const long long LAT_SHIFT = 1000000;
const long long GAP_BASE = 4000000;

long long parse_scaled(const string& s) {
    int pos = 0;
    int sign = 1;
    if (s[pos] == '-') {
        sign = -1;
        ++pos;
    } else if (s[pos] == '+') {
        ++pos;
    }

    long long whole = 0;
    while (pos < int(s.size()) && s[pos] != '.') {
        whole = whole * 10 + (s[pos] - '0');
        ++pos;
    }

    long long frac = 0;
    int digits = 0;
    if (pos < int(s.size()) && s[pos] == '.') {
        ++pos;
        while (pos < int(s.size()) && digits < 4) {
            frac = frac * 10 + (s[pos] - '0');
            ++digits;
            ++pos;
        }
        while (digits < 4) {
            frac *= 10;
            ++digits;
        }
    }

    return sign * (whole * 10000 + frac);
}

vector<long long> build_sequence(const vector<pair<long long, long long>>& input_points) {
    vector<pair<long long, long long>> points = input_points;
    sort(points.begin(), points.end());

    vector<pair<long long, vector<long long>>> buckets;
    for (int i = 0; i < int(points.size()); ) {
        int j = i;
        vector<long long> latitudes;
        while (j < int(points.size()) && points[j].first == points[i].first) {
            latitudes.push_back(points[j].second);
            ++j;
        }
        buckets.push_back({points[i].first, move(latitudes)});
        i = j;
    }

    vector<long long> sequence;
    sequence.reserve(points.size() * 2 + buckets.size() * 3);
    int m = int(buckets.size());
    for (int i = 0; i < m; ++i) {
        long long current_lon = buckets[i].first;
        long long next_lon = buckets[(i + 1) % m].first;
        long long gap = (next_lon - current_lon + CIRCLE) % CIRCLE;
        if (gap == 0) {
            gap = CIRCLE;
        }

        sequence.push_back(START_TOKEN);
        for (long long lat : buckets[i].second) {
            sequence.push_back(lat + LAT_SHIFT);
        }
        sequence.push_back(SEP_TOKEN);
        sequence.push_back(GAP_BASE + gap);
        sequence.push_back(END_TOKEN);
    }
    return sequence;
}

bool is_cyclic_shift(const vector<long long>& a, const vector<long long>& b) {
    if (a.size() != b.size()) {
        return false;
    }
    int n = int(b.size());
    vector<int> pi(n, 0);
    for (int i = 1; i < n; ++i) {
        int j = pi[i - 1];
        while (j > 0 && b[i] != b[j]) {
            j = pi[j - 1];
        }
        if (b[i] == b[j]) {
            ++j;
        }
        pi[i] = j;
    }

    int matched = 0;
    for (int i = 0; i < 2 * int(a.size()); ++i) {
        long long token = a[i % a.size()];
        while (matched > 0 && token != b[matched]) {
            matched = pi[matched - 1];
        }
        if (token == b[matched]) {
            ++matched;
        }
        if (matched == n) {
            int start = i - n + 1;
            if (start < int(a.size())) {
                return true;
            }
            matched = pi[matched - 1];
        }
    }
    return false;
}

void solve() {
    int n;
    cin >> n;

    vector<pair<long long, long long>> first(n), second(n);
    for (int i = 0; i < n; ++i) {
        string lat_str, lon_str;
        cin >> lat_str >> lon_str;
        long long lat = parse_scaled(lat_str);
        long long lon = parse_scaled(lon_str);
        if (lon < 0) {
            lon += CIRCLE;
        }
        first[i] = {lon, lat};
    }
    for (int i = 0; i < n; ++i) {
        string lat_str, lon_str;
        cin >> lat_str >> lon_str;
        long long lat = parse_scaled(lat_str);
        long long lon = parse_scaled(lon_str);
        if (lon < 0) {
            lon += CIRCLE;
        }
        second[i] = {lon, lat};
    }

    vector<long long> seq_first = build_sequence(first);
    vector<long long> seq_second = build_sequence(second);
    cout << (is_cyclic_shift(seq_first, seq_second) ? "Same" : "Different") << '\n';
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
