#include <bits/stdc++.h>
using namespace std;

struct BIT {
    int n;
    vector<int> tree;

    explicit BIT(int n = 0) : n(n), tree(n + 1, 0) {}

    void update(int idx, int delta) {
        for (; idx <= n; idx += idx & -idx) {
            tree[idx] += delta;
        }
    }

    int query_prefix(int idx) const {
        int sum = 0;
        for (; idx > 0; idx -= idx & -idx) {
            sum += tree[idx];
        }
        return sum;
    }

    int query_range(int left, int right) const {
        if (left > right) {
            return 0;
        }
        return query_prefix(right) - query_prefix(left - 1);
    }
};

struct BIT3D {
    int nx, ny, nz;
    vector<int> tree;

    BIT3D(int nx, int ny, int nz) : nx(nx), ny(ny), nz(nz),
                                    tree((nx + 1) * (ny + 1) * (nz + 1), 0) {}

    int index(int x, int y, int z) const {
        return (x * (ny + 1) + y) * (nz + 1) + z;
    }

    void update(int x, int y, int z, int delta) {
        for (int i = x; i <= nx; i += i & -i) {
            for (int j = y; j <= ny; j += j & -j) {
                for (int k = z; k <= nz; k += k & -k) {
                    tree[index(i, j, k)] += delta;
                }
            }
        }
    }

    int query_prefix(int x, int y, int z) const {
        if (x <= 0 || y <= 0 || z <= 0) {
            return 0;
        }
        x = min(x, nx);
        y = min(y, ny);
        z = min(z, nz);
        int sum = 0;
        for (int i = x; i > 0; i -= i & -i) {
            for (int j = y; j > 0; j -= j & -j) {
                for (int k = z; k > 0; k -= k & -k) {
                    sum += tree[index(i, j, k)];
                }
            }
        }
        return sum;
    }

    int query_box(int x1, int y1, int z1, int x2, int y2, int z2) const {
        if (x1 > x2 || y1 > y2 || z1 > z2) {
            return 0;
        }
        return query_prefix(x2, y2, z2)
             - query_prefix(x1 - 1, y2, z2)
             - query_prefix(x2, y1 - 1, z2)
             - query_prefix(x2, y2, z1 - 1)
             + query_prefix(x1 - 1, y1 - 1, z2)
             + query_prefix(x1 - 1, y2, z1 - 1)
             + query_prefix(x2, y1 - 1, z1 - 1)
             - query_prefix(x1 - 1, y1 - 1, z1 - 1);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int B, N, D, M;
    cin >> B >> N >> D >> M;

    if (B == 1) {
        vector<int> x(N);
        for (int i = 0; i < N; ++i) {
            cin >> x[i];
        }
        sort(x.begin(), x.end());

        long long ans = 0;
        int left = 0;
        for (int right = 0; right < N; ++right) {
            while (x[right] - x[left] > D) {
                ++left;
            }
            ans += right - left;
        }
        cout << ans << '\n';
        return 0;
    }

    if (B == 2) {
        vector<int> u(N), v(N);
        for (int i = 0; i < N; ++i) {
            int x, y;
            cin >> x >> y;
            u[i] = x + y;
            v[i] = x - y;
        }

        vector<int> coords = v;
        sort(coords.begin(), coords.end());
        coords.erase(unique(coords.begin(), coords.end()), coords.end());

        auto get_index = [&](int value) {
            return static_cast<int>(lower_bound(coords.begin(), coords.end(), value) -
                                    coords.begin()) + 1;
        };

        vector<int> order(N);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int a, int b) {
            return u[a] < u[b];
        });

        BIT bit(static_cast<int>(coords.size()));
        long long ans = 0;
        int left = 0;

        for (int idx = 0; idx < N; ++idx) {
            int cur = order[idx];
            while (u[cur] - u[order[left]] > D) {
                bit.update(get_index(v[order[left]]), -1);
                ++left;
            }

            int lo = get_index(v[cur] - D);
            int hi = static_cast<int>(upper_bound(coords.begin(), coords.end(), v[cur] + D) -
                                      coords.begin());
            ans += bit.query_range(lo, hi);
            bit.update(get_index(v[cur]), 1);
        }

        cout << ans << '\n';
        return 0;
    }

    vector<int> f1(N), f2(N), f3(N), f4(N);
    for (int i = 0; i < N; ++i) {
        int x, y, z;
        cin >> x >> y >> z;
        f1[i] = x + y + z;
        f2[i] = x + y - z;
        f3[i] = x - y + z;
        f4[i] = x - y - z;
    }

    const int min23 = 2 - M;
    const int max23 = 2 * M - 1;
    const int min4 = 1 - 2 * M;
    const int max4 = M - 2;
    const int size23 = max23 - min23 + 1;
    const int size4 = max4 - min4 + 1;

    auto idx23 = [&](int value) {
        return value - min23 + 1;
    };
    auto idx4 = [&](int value) {
        return value - min4 + 1;
    };

    vector<int> order(N);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int a, int b) {
        return f1[a] < f1[b];
    });

    BIT3D bit(size23, size23, size4);
    long long ans = 0;
    int left = 0;

    for (int idx = 0; idx < N; ++idx) {
        int cur = order[idx];
        while (f1[cur] - f1[order[left]] > D) {
            int rem = order[left++];
            bit.update(idx23(f2[rem]), idx23(f3[rem]), idx4(f4[rem]), -1);
        }

        int x1 = idx23(max(min23, f2[cur] - D));
        int x2 = idx23(min(max23, f2[cur] + D));
        int y1 = idx23(max(min23, f3[cur] - D));
        int y2 = idx23(min(max23, f3[cur] + D));
        int z1 = idx4(max(min4, f4[cur] - D));
        int z2 = idx4(min(max4, f4[cur] + D));

        ans += bit.query_box(x1, y1, z1, x2, y2, z2);
        bit.update(idx23(f2[cur]), idx23(f3[cur]), idx4(f4[cur]), 1);
    }

    cout << ans << '\n';
    return 0;
}
