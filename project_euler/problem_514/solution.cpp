#include <bits/stdc++.h>
using namespace std;

// Convex hull and geoboard shape counting
typedef pair<int,int> pii;

long long cross(pii O, pii A, pii B) {
    return (long long)(A.first - O.first) * (B.second - O.second)
         - (long long)(A.second - O.second) * (B.first - O.first);
}

vector<pii> convex_hull(vector<pii> pts) {
    sort(pts.begin(), pts.end());
    pts.erase(unique(pts.begin(), pts.end()), pts.end());
    int n = pts.size();
    if (n < 3) return pts;

    vector<pii> hull;
    // Lower hull
    for (int i = 0; i < n; i++) {
        while (hull.size() >= 2 && cross(hull[hull.size()-2], hull[hull.size()-1], pts[i]) <= 0)
            hull.pop_back();
        hull.push_back(pts[i]);
    }
    // Upper hull
    int lower_size = hull.size() + 1;
    for (int i = n - 2; i >= 0; i--) {
        while ((int)hull.size() >= lower_size && cross(hull[hull.size()-2], hull[hull.size()-1], pts[i]) <= 0)
            hull.pop_back();
        hull.push_back(pts[i]);
    }
    hull.pop_back();
    return hull;
}

// Normalize polygon: translate so min point is at origin, sort
vector<pii> normalize(vector<pii> pts) {
    int mx = INT_MAX, my = INT_MAX;
    for (auto& p : pts) { mx = min(mx, p.first); my = min(my, p.second); }
    for (auto& p : pts) { p.first -= mx; p.second -= my; }
    sort(pts.begin(), pts.end());
    return pts;
}

vector<pii> rotate90(vector<pii> pts, int n) {
    vector<pii> res;
    for (auto& p : pts) res.push_back({p.second, n - 1 - p.first});
    return res;
}

vector<pii> reflectH(vector<pii> pts, int n) {
    vector<pii> res;
    for (auto& p : pts) res.push_back({p.first, n - 1 - p.second});
    return res;
}

vector<pii> canonical(vector<pii> pts, int n) {
    vector<vector<pii>> forms;
    auto cur = pts;
    for (int i = 0; i < 4; i++) {
        forms.push_back(normalize(cur));
        forms.push_back(normalize(reflectH(cur, n)));
        cur = rotate90(cur, n);
    }
    return *min_element(forms.begin(), forms.end());
}

int main() {
    int n;
    cout << "Enter grid size n: ";
    cin >> n;

    vector<pii> grid;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            grid.push_back({i, j});

    // For small n, enumerate triangles (subsets of size 3) as basic shapes
    set<vector<pii>> shapes;
    int total = grid.size();
    for (int i = 0; i < total; i++)
        for (int j = i + 1; j < total; j++)
            for (int k = j + 1; k < total; k++) {
                vector<pii> tri = {grid[i], grid[j], grid[k]};
                // Check non-degenerate (area > 0)
                if (cross(tri[0], tri[1], tri[2]) != 0) {
                    auto c = canonical(tri, n);
                    shapes.insert(c);
                }
            }

    cout << "Distinct triangle shapes on " << n << "x" << n << " geoboard: " << shapes.size() << endl;
    return 0;
}
