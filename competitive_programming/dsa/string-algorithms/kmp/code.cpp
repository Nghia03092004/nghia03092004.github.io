vector<int> prefix_function(const string& s) {
    vector<int> pi(s.size(), 0);
    for (int i = 1; i < (int)s.size(); ++i) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) ++j;
        pi[i] = j;
    }
    return pi;
}

vector<int> kmp_search(const string& text, const string& pattern) {
    if (pattern.empty()) return {};

    vector<int> pi = prefix_function(pattern);
    vector<int> occurrences;
    int matched = 0;

    for (int i = 0; i < (int)text.size(); ++i) {
        while (matched > 0 && text[i] != pattern[matched]) {
            matched = pi[matched - 1];
        }
        if (text[i] == pattern[matched]) ++matched;
        if (matched == (int)pattern.size()) {
            occurrences.push_back(i - matched + 1);
            matched = pi[matched - 1];
        }
    }

    return occurrences;
}
