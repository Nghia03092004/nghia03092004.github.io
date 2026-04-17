#include <bits/stdc++.h>
using namespace std;

int main() {
    // Langton's Ant simulation
    // Directions: 0=up, 1=right, 2=down, 3=left
    int dx[] = {0, 1, 0, -1};
    int dy[] = {1, 0, -1, 0};

    set<pair<int,int>> black; // set of black squares
    int x = 0, y = 0, dir = 0;

    long long N = 1000000000000000000LL; // 10^18

    // Simulate enough to pass the transient and detect the cycle
    // The highway starts around step 10000, period 104, +12 black per period
    int sim_steps = 20000; // well past transient

    vector<int> black_count(sim_steps + 1);
    black_count[0] = 0;

    for (int step = 0; step < sim_steps; step++) {
        pair<int,int> pos = {x, y};
        if (black.count(pos)) {
            // On black: flip to white, turn left, move forward
            black.erase(pos);
            dir = (dir + 3) % 4; // turn left
        } else {
            // On white: flip to black, turn right, move forward
            black.insert(pos);
            dir = (dir + 1) % 4; // turn right
        }
        x += dx[dir];
        y += dy[dir];
        black_count[step + 1] = (int)black.size();
    }

    // Verify the period: check that from some point, every 104 steps adds 12
    // Find a good starting point
    int period = 104;
    int gain = 12;

    // Find L such that the pattern is stable
    int L = sim_steps;
    // Make sure L is aligned: find L such that (N - L) % period can be handled
    // We want L where the highway is definitely active
    // Check: black_count[L] - black_count[L - period] should equal gain
    // for several consecutive periods

    // Verify
    bool stable = true;
    for (int t = sim_steps; t >= sim_steps - 10 * period; t -= period) {
        if (t - period < 0) break;
        if (black_count[t] - black_count[t - period] != gain) {
            stable = false;
            break;
        }
    }

    if (!stable) {
        // Fallback: shouldn't happen
        cout << "Pattern not detected!" << endl;
        return 1;
    }

    long long remaining = N - L;
    long long full_cycles = remaining / period;
    long long leftover = remaining % period;

    long long answer = (long long)black_count[L] + full_cycles * gain;

    // Add the leftover: black_count[L + leftover] - black_count[L]
    // But we already simulated up to L = sim_steps, and leftover < 104
    // We need to simulate leftover more steps... but we didn't save state.
    // Actually leftover < period, and we know the pattern:
    // black_count[L + leftover] - black_count[L] = black_count[L - period + leftover] - black_count[L - period]
    // (since the pattern repeats)

    long long delta = black_count[L - period + (int)leftover] - black_count[L - period];
    answer += delta;

    cout << answer << endl;
    return 0;
}
