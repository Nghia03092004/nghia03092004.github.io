pair<vector<int>, vector<int>> build_suffix_array(string s) {
    s.push_back('$');
    int n = (int)s.size();

    vector<int> sa(n), cls(n);
    {
        vector<pair<char, int>> a(n);
        for (int i = 0; i < n; ++i) {
            a[i] = {s[i], i};
        }
        sort(a.begin(), a.end());
        for (int i = 0; i < n; ++i) {
            sa[i] = a[i].second;
        }
        cls[sa[0]] = 0;
        for (int i = 1; i < n; ++i) {
            cls[sa[i]] = cls[sa[i - 1]] + (a[i].first != a[i - 1].first);
        }
    }

    for (int k = 0; (1 << k) < n; ++k) {
        for (int i = 0; i < n; ++i) {
            sa[i] = (sa[i] - (1 << k) + n) % n;
        }
        vector<int> cnt(n), sa_new(n);
        for (int x : cls) ++cnt[x];
        for (int i = 1; i < n; ++i) cnt[i] += cnt[i - 1];
        for (int i = n - 1; i >= 0; --i) {
            sa_new[--cnt[cls[sa[i]]]] = sa[i];
        }
        sa = sa_new;

        vector<int> cls_new(n);
        cls_new[sa[0]] = 0;
        for (int i = 1; i < n; ++i) {
            pair<int, int> cur = {cls[sa[i]], cls[(sa[i] + (1 << k)) % n]};
            pair<int, int> prev = {cls[sa[i - 1]], cls[(sa[i - 1] + (1 << k)) % n]};
            cls_new[sa[i]] = cls_new[sa[i - 1]] + (cur != prev);
        }
        cls = cls_new;
    }

    sa.erase(sa.begin());
    int m = (int)sa.size();
    vector<int> rank(m), lcp(max(0, m - 1), 0);
    for (int i = 0; i < m; ++i) {
        rank[sa[i]] = i;
    }
    for (int i = 0, k = 0; i < m; ++i) {
        if (rank[i] == m - 1) {
            k = 0;
            continue;
        }
        int j = sa[rank[i] + 1];
        while (i + k < m && j + k < m && s[i + k] == s[j + k]) {
            ++k;
        }
        lcp[rank[i]] = k;
        if (k) --k;
    }
    return {sa, lcp};
}
