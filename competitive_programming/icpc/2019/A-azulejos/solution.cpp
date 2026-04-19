#include <bits/stdc++.h>
using namespace std;

namespace {

struct Tile {
    int price;
    int height;
    int id;
};

vector<pair<int, int> > build_groups(const vector<Tile>& tiles) {
    vector<pair<int, int> > groups;
    int n = (int)tiles.size();
    for (int i = 0; i < n; ) {
        int j = i + 1;
        while (j < n && tiles[j].price == tiles[i].price) {
            ++j;
        }
        groups.push_back(make_pair(i, j));
        i = j;
    }
    return groups;
}

void load_group(const vector<Tile>& tiles, const pair<int, int>& range,
                multiset<pair<int, int> >& bag) {
    bag.clear();
    for (int i = range.first; i < range.second; ++i) {
        bag.insert(make_pair(tiles[i].height, tiles[i].id));
    }
}

bool solve_instance(int n, const vector<int>& pb, const vector<int>& hb,
                    const vector<int>& pf, const vector<int>& hf,
                    vector<int>& ans_back, vector<int>& ans_front) {
    vector<Tile> back(n), front(n);
    for (int i = 0; i < n; ++i) {
        back[i] = {pb[i], hb[i], i + 1};
        front[i] = {pf[i], hf[i], i + 1};
    }
    sort(back.begin(), back.end(), [](const Tile& lhs, const Tile& rhs) {
        if (lhs.price != rhs.price) {
            return lhs.price < rhs.price;
        }
        if (lhs.height != rhs.height) {
            return lhs.height < rhs.height;
        }
        return lhs.id < rhs.id;
    });
    sort(front.begin(), front.end(), [](const Tile& lhs, const Tile& rhs) {
        if (lhs.price != rhs.price) {
            return lhs.price < rhs.price;
        }
        if (lhs.height != rhs.height) {
            return lhs.height < rhs.height;
        }
        return lhs.id < rhs.id;
    });

    vector<pair<int, int> > groups_back = build_groups(back);
    vector<pair<int, int> > groups_front = build_groups(front);
    multiset<pair<int, int> > rem_back, rem_front;

    int ptr_back = 0;
    int ptr_front = 0;
    load_group(back, groups_back[ptr_back], rem_back);
    load_group(front, groups_front[ptr_front], rem_front);

    ans_back.clear();
    ans_front.clear();
    ans_back.reserve(n);
    ans_front.reserve(n);

    while (true) {
        int cnt_back = (int)rem_back.size();
        int cnt_front = (int)rem_front.size();
        if (cnt_back == 0 || cnt_front == 0) {
            break;
        }

        vector<pair<int, int> > matched;
        matched.reserve(min(cnt_back, cnt_front));

        if (cnt_front <= cnt_back) {
            vector<pair<int, int> > cur_front(rem_front.begin(), rem_front.end());
            for (int i = (int)cur_front.size() - 1; i >= 0; --i) {
                multiset<pair<int, int> >::iterator it =
                    rem_back.upper_bound(make_pair(cur_front[i].first, INT_MAX));
                if (it == rem_back.end()) {
                    return false;
                }
                matched.push_back(make_pair(it->second, cur_front[i].second));
                rem_back.erase(it);
            }
            rem_front.clear();
        } else {
            vector<pair<int, int> > cur_back(rem_back.begin(), rem_back.end());
            for (int i = 0; i < (int)cur_back.size(); ++i) {
                multiset<pair<int, int> >::iterator it =
                    rem_front.lower_bound(make_pair(cur_back[i].first, -1));
                if (it == rem_front.begin()) {
                    return false;
                }
                --it;
                matched.push_back(make_pair(cur_back[i].second, it->second));
                rem_front.erase(it);
            }
            rem_back.clear();
        }

        for (const pair<int, int>& p : matched) {
            ans_back.push_back(p.first);
            ans_front.push_back(p.second);
        }

        if (rem_back.empty()) {
            ++ptr_back;
            if (ptr_back < (int)groups_back.size()) {
                load_group(back, groups_back[ptr_back], rem_back);
            }
        }
        if (rem_front.empty()) {
            ++ptr_front;
            if (ptr_front < (int)groups_front.size()) {
                load_group(front, groups_front[ptr_front], rem_front);
            }
        }
    }

    return (int)ans_back.size() == n && (int)ans_front.size() == n;
}

void solve() {
    int n;
    cin >> n;
    vector<int> pb(n), hb(n), pf(n), hf(n);
    for (int i = 0; i < n; ++i) {
        cin >> pb[i];
    }
    for (int i = 0; i < n; ++i) {
        cin >> hb[i];
    }
    for (int i = 0; i < n; ++i) {
        cin >> pf[i];
    }
    for (int i = 0; i < n; ++i) {
        cin >> hf[i];
    }

    vector<int> ans_back, ans_front;
    if (!solve_instance(n, pb, hb, pf, hf, ans_back, ans_front)) {
        cout << "impossible\n";
        return;
    }

    for (int i = 0; i < n; ++i) {
        if (i) {
            cout << ' ';
        }
        cout << ans_back[i];
    }
    cout << '\n';
    for (int i = 0; i < n; ++i) {
        if (i) {
            cout << ' ';
        }
        cout << ans_front[i];
    }
    cout << '\n';
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
