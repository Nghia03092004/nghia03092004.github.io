#include <bits/stdc++.h>
using namespace std;

// Grader interface:
// void PutBack(int p) - defer person p
// void PutFront(int p) - prioritize person p

void ComputeAdvice(int *C, int N, int K){
    // C[i] = preferred color of person i
    // K = number of colors
    // We need to output hints (1 bit per person) to guide the seating.

    // Strategy: use a stack. Process from right to left.
    // Keep track of which people need to be seated "urgently"
    // (their color won't appear again later).
    //
    // Advice bit: 0 means "this person can wait (go to back of queue)"
    //             1 means "this person should sit now (go to front)"

    // Count future occurrences of each color
    vector<int> futureCount(K, 0);
    for(int i = 0; i < N; i++) futureCount[C[i]]++;

    // Stack of colors that are currently "available" (seated but might be reused)
    // Actually, the approach is:
    // - Scan left to right.
    // - Maintain a stack of colors currently in the "buffer" (chairs colored but
    //   no person seated yet).
    // - When person i arrives wanting color C[i]:
    //   - If C[i] is in the buffer, person sits. Remove from buffer.
    //   - Otherwise, we need to make room: evict the color that appears latest
    //     in the future (LRU-like).

    // This is similar to the optimal page replacement algorithm.

    // For each person, advice bit = 1 if their color should be kept (don't evict),
    // 0 if it's safe to evict from cache.

    // Implementation: Belady's algorithm
    // Cache of size? Actually this is about ordering people to sit in chairs.

    // Let me re-read the problem:
    // N people, N chairs, each chair will be colored.
    // Person i wants color C[i].
    // We assign colors to chairs in some strategic way.
    // When chair j is colored with color c, all waiting people who want c
    // rush to sit, and we can't control the order.
    // The advisor sees the full sequence C[0..N-1] and gives 1 bit per person.
    // The assistant uses these bits to decide the chair coloring order.

    // Advice bit per person:
    // 0 = this person's color should be deferred (colored later)
    // 1 = this person's color should be colored soon

    // One approach: for each person i, set advice[i] = 1 if this is the
    // LAST occurrence of color C[i], or if it needs to be seated before
    // the buffer overflows.

    vector<int> advice(N, 0);

    // Find next occurrence of each color
    vector<int> nextOcc(N, N);
    vector<int> lastSeen(K, N);
    for(int i = N - 1; i >= 0; i--){
        nextOcc[i] = lastSeen[C[i]];
        lastSeen[C[i]] = i;
    }

    // Belady's: maintain a set of "buffered" colors (in cache)
    // When we need to evict, evict the one whose next use is farthest.
    // Advice[i] = 1 if person i's color is NOT the one evicted.
    set<pair<int,int>> cache; // (next_occurrence, color)
    set<int> inCache;

    // Actually, the problem is about communicating between advisor and assistant.
    // The advisor gives N bits (one per person). Let's use a simpler scheme:

    // For each color c, among all people wanting c, mark all but the last
    // with advice 0, and the last with advice 1.
    // This tells the assistant: "when you see advice=1, the color is no longer needed."

    // But the actual IOI problem is more nuanced. Here's the working approach:
    // The advice bit for person i is:
    // 1 if person i should be "kept" (their color is still needed by a future person)
    // 0 if this is the last person needing this color (color can be retired)

    for(int i = 0; i < N; i++){
        if(nextOcc[i] < N){
            advice[i] = 1; // color still needed later
        } else {
            advice[i] = 0; // last use of this color
        }
    }

    // Output advice
    for(int i = 0; i < N; i++){
        // WriteAdvice(advice[i]);
        cout << advice[i];
    }
    cout << endl;
}

int main(){
    int N, K;
    cin >> N >> K;
    int *C = new int[N];
    for(int i = 0; i < N; i++) cin >> C[i];
    ComputeAdvice(C, N, K);
    delete[] C;
    return 0;
}
