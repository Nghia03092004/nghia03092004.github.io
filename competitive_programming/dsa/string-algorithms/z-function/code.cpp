vector<int> z_function(const string& s) {
    int n = (int)s.size();
    vector<int> z(n, 0);
    for (int i = 1, l = 0, r = 0; i < n; ++i) {
        if (i <= r) {
            z[i] = min(r - i + 1, z[i - l]);
        }
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
            ++z[i];
        }
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
    return z;
}

vector<int> find_occurrences_z(const string& pattern, const string& text) {
    string merged = pattern + "#" + text;
    vector<int> z = z_function(merged);
    vector<int> positions;
    int m = (int)pattern.size();
    for (int i = m + 1; i < (int)merged.size(); ++i) {
        if (z[i] >= m) {
            positions.push_back(i - m - 1);
        }
    }
    return positions;
}
