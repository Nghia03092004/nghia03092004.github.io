// IOI 2009 - Garage
// Straightforward simulation with a set of available spots and a FIFO queue.
// O(M log N) time.
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<int> rate(N + 1);
    for (int i = 1; i <= N; i++) cin >> rate[i];

    vector<int> weight(M + 1);
    for (int i = 1; i <= M; i++) cin >> weight[i];

    // Available spots ordered by number (smallest first).
    set<int> available;
    for (int i = 1; i <= N; i++) available.insert(i);

    queue<int> waiting;          // FIFO queue of cars waiting for a spot
    map<int, int> carSpot;       // car -> assigned spot
    long long revenue = 0;

    int events = 2 * M;
    for (int e = 0; e < events; e++) {
        int x;
        cin >> x;

        if (x > 0) {
            // Car x arrives.
            if (!available.empty()) {
                int spot = *available.begin();
                available.erase(available.begin());
                carSpot[x] = spot;
                revenue += (long long)rate[spot] * weight[x];
            } else {
                waiting.push(x);
            }
        } else {
            // Car -x departs.
            int car = -x;
            int spot = carSpot[car];
            carSpot.erase(car);
            available.insert(spot);

            // Assign the smallest available spot to the next waiting car.
            if (!waiting.empty()) {
                int nextCar = waiting.front();
                waiting.pop();
                int bestSpot = *available.begin();
                available.erase(available.begin());
                carSpot[nextCar] = bestSpot;
                revenue += (long long)rate[bestSpot] * weight[nextCar];
            }
        }
    }

    cout << revenue << "\n";
    return 0;
}
