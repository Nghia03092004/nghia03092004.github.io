#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int X = 2520;

struct Light {
    int x;
    int r;
    int g;
    int p;
    int q;
    int prime;
    int block;
};

int prime_base(int x) {
    if (x <= 1) {
        return -1;
    }
    for (int p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            while (x % p == 0) {
                x /= p;
            }
            return x == 1 ? p : -1;
        }
    }
    return x;
}

void solve() {
    int n;
    cin >> n;
    vector<Light> lights(n);
    vector<int> max_power(101, 1);

    for (int i = 0; i < n; ++i) {
        cin >> lights[i].x >> lights[i].r >> lights[i].g;
        lights[i].p = lights[i].r + lights[i].g;
        int d = __gcd(X, lights[i].p);
        lights[i].q = lights[i].p / d;
        lights[i].prime = prime_base(lights[i].q);
        lights[i].block = -1;
        if (lights[i].q > 1) {
            max_power[lights[i].prime] = max(max_power[lights[i].prime], lights[i].q);
        }
    }

    vector<int> block_of_prime(101, -1);
    vector<int> mod_of_block;
    for (int p = 2; p <= 100; ++p) {
        if (max_power[p] > 1) {
            block_of_prime[p] = static_cast<int>(mod_of_block.size());
            mod_of_block.push_back(max_power[p]);
        }
    }
    for (auto& light : lights) {
        if (light.q > 1) {
            light.block = block_of_prime[light.prime];
        }
    }

    vector<long double> answer(n + 1, 0.0L);
    vector<vector<unsigned char>> alive(mod_of_block.size());
    vector<int> alive_count(mod_of_block.size());
    vector<unsigned char> good(101);

    for (int t0 = 0; t0 < X; ++t0) {
        for (int b = 0; b < static_cast<int>(mod_of_block.size()); ++b) {
            alive[b].assign(mod_of_block[b], 1);
            alive_count[b] = mod_of_block[b];
        }

        long double survive = 1.0L;
        for (int i = 0; i < n; ++i) {
            if (survive == 0.0L) {
                break;
            }

            const Light& light = lights[i];
            if (light.q == 1) {
                int when = (t0 + light.x) % light.p;
                if (when < light.r) {
                    answer[i] += survive;
                    survive = 0.0L;
                    break;
                }
                continue;
            }

            const int q = light.q;
            const int block = light.block;
            const int mod = mod_of_block[block];
            const int current_alive = alive_count[block];
            if (current_alive == 0) {
                survive = 0.0L;
                break;
            }

            for (int k = 0; k < q; ++k) {
                int when = (t0 + light.x + static_cast<int>((1LL * k * X) % light.p)) % light.p;
                good[k] = static_cast<unsigned char>(when >= light.r);
            }

            int red_count = 0;
            for (int u = 0; u < mod; ++u) {
                if (alive[block][u] && !good[u % q]) {
                    ++red_count;
                }
            }

            answer[i] += survive * static_cast<long double>(red_count) / current_alive;
            if (red_count == 0) {
                continue;
            }
            if (red_count == current_alive) {
                survive = 0.0L;
                break;
            }

            for (int u = 0; u < mod; ++u) {
                if (alive[block][u] && !good[u % q]) {
                    alive[block][u] = 0;
                }
            }
            alive_count[block] -= red_count;
            survive *= static_cast<long double>(alive_count[block]) / current_alive;
        }

        answer[n] += survive;
    }

    cout << fixed << setprecision(12);
    for (long double value : answer) {
        cout << static_cast<double>(value / X) << '\n';
    }
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
