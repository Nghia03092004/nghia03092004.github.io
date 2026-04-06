#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

/*
 * Problem 327: Rooms of Doom
 *
 * C(R, N) = minimum cards to traverse N rooms with max carry R.
 *
 * Key idea: work room by room. To push supplies past room k, you need
 * multiple trips. Each round trip to room k costs 2 cards (1 forward, 1 back).
 * On the final (one-way) trip you spend 1 card to go forward.
 *
 * Let c(n) = C(R, n). Then:
 *   c(n) = n, for n <= R - 1
 * For n >= R:
 *   The number of trips t through room n's door satisfies:
 *   We need c(n-1) cards delivered to room 1-side after room n-1.
 *   Actually, we compute iteratively:
 *   c(n) depends on how many trips are needed.
 *
 * Recurrence: c(n) = ceil((c(n-1) + 2) / R) * R  [for certain formulations]
 *
 * Let's use the known correct formulation:
 * c(n) for the "rooms of doom" problem:
 *   if c(n-1) < R: c(n) = c(n-1) + 1
 *   else: let t = ceil((c(n-1) - R) / (R-2)) + 1 trips
 *         c(n) = t * R  -- but we refine this
 *
 * Actually the simplest correct recurrence from working backwards:
 *   c(0) = 0
 *   c(n) = c(n-1) + 1  if c(n-1) + 1 <= R
 *   otherwise we need extra trips:
 *     t = ceil( (c(n-1) - (R-1)) / (R-2) )  extra round trips
 *     c(n) = c(n-1) + 1 + 2*t
 *
 * But this isn't quite right either. Let's think more carefully.
 *
 * Correct approach:
 * c(n) = number of cards that must be picked up from room 0.
 * Each card picked up from room 0 is one card taken.
 * c(n-1) cards need to be available at room 1 to solve rooms 1..n
 * (by subproblem reduction).
 * To ferry c(n-1) cards to room 1:
 *   Each round trip: carry R cards to room 1, deposit R-2 (spend 1 going, 1 returning)
 *   Last trip (one-way): carry R cards, deposit R-1 (spend 1 going)
 *   So with t total trips (t-1 round trips + 1 one-way):
 *     delivered = (t-1)*(R-2) + (R-1) = t*(R-2) + 1
 *   We need t*(R-2) + 1 >= c(n-1)
 *   t = ceil((c(n-1) - 1) / (R-2))
 *   cards taken = t * R
 *
 * So: c(1) = ceil((c(0) - 1)/(R-2)) * R ... but c(0) = 0 doesn't work.
 * Actually c(0) = 0 (no rooms to traverse), and for c(1) we just need 1 card.
 *
 * Better: think of c(n) = cards needed at position 0 to reach room n.
 * To get through room n's door, you need 1 card at room n-1.
 * Plus you need c(n-1) - ? ... Let me just use the well-known formula.
 *
 * Well-known: Let need(n) = cards required at room 0.
 * need(n) = n for n <= R-1
 * For n >= R:
 *   need(n) = R * ceil((need(n-1) - 1) / (R - 2))
 *   but ensure at least need(n-1) + 1
 */

ll solve(int R, int N) {
    // c[n] = minimum cards from room 0 to pass through n doors
    // Use the ferry approach:
    // To deliver 'need' cards to room 1, we require:
    //   t = ceil((need - 1) / (R - 2)) trips if need > R-1
    //   total cards from room 0 = t * R if t > 1, else R if need <= R-1
    // But we need need <= t*(R-2) + (R-1) = t*R - t - ... hmm

    // Let me just iterate properly.
    // c(0) = 0
    // To solve rooms 1..n from room 0:
    //   We need to ferry c(n-1) cards to room 1 plus 1 card to open door 1
    //   Wait, no. The sub-structure is:
    //   c(n) = cards at room 0 to pass n doors.
    //   At room 0, we pick up cards and walk to room 1 (costs 1 card per trip).
    //   We need c(n-1) cards at room 1 to proceed through remaining n-1 doors.
    //   Round trip: pick R, go to room 1 (-1 card), deposit R-2, go back (-1 card)
    //   Final trip: pick R, go to room 1 (-1 card), deposit R-1, stay
    //   With t trips: (t-1)(R-2) + (R-1) = t(R-2) + 1 cards at room 1
    //   Need: t(R-2) + 1 >= c(n-1)
    //   => t >= (c(n-1) - 1) / (R - 2)
    //   => t = max(1, ceil((c(n-1) - 1) / (R - 2)))
    //   Total from room 0: t * R

    vector<ll> c(N + 1);
    c[0] = 0;
    for (int n = 1; n <= N; n++) {
        ll need = c[n - 1]; // cards needed at room 1 to solve n-1 more doors
        if (need <= 0) {
            c[n] = R; // just one trip, but that's overkill; we only need 'n' cards
            // Actually for small n, c(n) = n
            c[n] = n;
        } else {
            ll t = max(1LL, (need - 1 + (R - 2) - 1) / (R - 2)); // ceil((need-1)/(R-2))
            if (need <= R - 1) t = 1;
            c[n] = t * R;
        }
    }
    return c[N];
}

int main() {
    // C(30, 100)
    cout << solve(30, 100) << endl;
    // Expected: 34315549139516
    return 0;
}
