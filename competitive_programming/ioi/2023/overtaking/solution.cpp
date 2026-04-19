#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// IOI 2023 - Overtaking
// N buses + 1 reserve bus travel through M sorting stations.
// At each station, a bus's arrival = max(own expected, max expected of all
// buses that arrived earlier at the previous station).
// For each query Y (reserve bus departure time), compute its final arrival.
//
// Precompute non-reserve buses (ignoring reserve bus's effect on them).
// For each station, maintain sorted arrival times + prefix max of expected
// arrivals. Answer each query in O(M log N) via binary search.

int L_road, N, M;
ll X_speed;
vector<ll> T_dep, W_speed, S_pos;
vector<vector<pair<ll, ll>>> sorted_station; // (arrival, expected_next)
vector<vector<ll>> prefix_max_expected;

void init(int _L, int _N, vector<ll> _T, vector<int> _W,
          int _X, int _M, vector<int> _S) {
    L_road = _L;
    N = _N;
    M = _M;
    X_speed = _X;
    T_dep = _T;
    W_speed.assign(N, 0);
    for (int i = 0; i < N; i++) W_speed[i] = _W[i];
    S_pos.assign(M, 0);
    for (int i = 0; i < M; i++) S_pos[i] = _S[i];

    // Simulate non-reserve buses through all stations
    vector<vector<ll>> bus_arrival(M, vector<ll>(N));
    for (int i = 0; i < N; i++)
        bus_arrival[0][i] = T_dep[i];

    for (int j = 0; j + 1 < M; j++) {
        ll dist = S_pos[j + 1] - S_pos[j];

        // (arrival at j, expected at j+1)
        vector<pair<ll, ll>> arrivals(N);
        for (int i = 0; i < N; i++)
            arrivals[i] = {bus_arrival[j][i], bus_arrival[j][i] + W_speed[i] * dist};

        // Sort by arrival time at station j
        vector<int> order(N);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int a, int b) {
            return arrivals[a].first < arrivals[b].first;
        });

        // Compute actual arrival at j+1 (blocked by slower buses ahead)
        ll running_max = 0;
        for (int idx : order) {
            ll expected = arrivals[idx].second;
            bus_arrival[j + 1][idx] = max(expected, running_max);
            running_max = max(running_max, expected);
        }
    }

    // For each station, precompute sorted arrivals + prefix max of expected next
    sorted_station.resize(M);
    prefix_max_expected.resize(M);

    for (int j = 0; j + 1 < M; j++) {
        ll dist = S_pos[j + 1] - S_pos[j];
        vector<pair<ll, ll>> arr_exp(N);
        for (int i = 0; i < N; i++)
            arr_exp[i] = {bus_arrival[j][i], bus_arrival[j][i] + W_speed[i] * dist};
        sort(arr_exp.begin(), arr_exp.end());
        sorted_station[j] = arr_exp;

        prefix_max_expected[j].resize(N);
        ll mx = 0;
        for (int i = 0; i < N; i++) {
            mx = max(mx, arr_exp[i].second);
            prefix_max_expected[j][i] = mx;
        }
    }
}

ll arrival_time(ll Y) {
    ll t_res = Y;

    for (int j = 0; j + 1 < M; j++) {
        ll dist = S_pos[j + 1] - S_pos[j];
        ll expected = t_res + X_speed * dist;

        // Find last non-reserve bus arriving strictly before t_res at station j
        auto& ss = sorted_station[j];
        int lo = 0, hi = (int)ss.size() - 1, pos = -1;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            if (ss[mid].first < t_res) {
                pos = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }

        ll blocking = 0;
        if (pos >= 0)
            blocking = prefix_max_expected[j][pos];

        t_res = max(expected, blocking);
    }

    return t_res;
}

int main() {
    int _L, _N, _X, _M, Q;
    scanf("%d %d", &_L, &_N);
    vector<ll> T(_N);
    vector<int> W(_N);
    for (int i = 0; i < _N; i++) scanf("%lld %d", &T[i], &W[i]);
    scanf("%d %d", &_X, &_M);
    vector<int> S(_M);
    for (int i = 0; i < _M; i++) scanf("%d", &S[i]);
    init(_L, _N, T, W, _X, _M, S);
    scanf("%d", &Q);
    while (Q--) {
        ll Y;
        scanf("%lld", &Y);
        printf("%lld\n", arrival_time(Y));
    }
    return 0;
}
