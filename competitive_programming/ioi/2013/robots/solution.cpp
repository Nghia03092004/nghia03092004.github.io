#include <bits/stdc++.h>
using namespace std;

int putaway(int A, int B, int T, int X[], int Y[], int W[], int S[]){
    // Sort robot capacities
    sort(X, X + A);
    sort(Y, Y + B);

    // Check if any toy is impossible
    for(int k = 0; k < T; k++){
        bool canWeak = false, canSmall = false;
        if(A > 0 && W[k] < X[A-1]) canWeak = true;
        if(B > 0 && S[k] < Y[B-1]) canSmall = true;
        if(!canWeak && !canSmall) return -1;
    }

    // Sort toys by weight descending (for weak robot assignment)
    vector<int> toyIdx(T);
    iota(toyIdx.begin(), toyIdx.end(), 0);

    // Binary search on number of trips
    auto check = [&](int trips) -> bool {
        // Assign toys to weak robots first (greedily by weight)
        // Sort toys by weight descending
        vector<pair<int,int>> toys(T); // (weight, size) with original index
        for(int i = 0; i < T; i++) toys[i] = {W[i], S[i]};
        sort(toys.begin(), toys.end(), [](auto &a, auto &b){
            return a.first > b.first;
        });

        // Assign to weak robots (sorted by capacity descending)
        vector<bool> assigned(T, false);
        int toyPtr = 0;
        for(int r = A - 1; r >= 0; r--){
            // Robot r has capacity X[r], can take 'trips' toys with weight < X[r]
            int count = 0;
            while(toyPtr < T && count < trips){
                if(!assigned[toyPtr] && toys[toyPtr].first < X[r]){
                    assigned[toyPtr] = true;
                    count++;
                }
                toyPtr++;
            }
            // Reset toyPtr for next robot? No -- we process toys greedily.
            // Actually, the greedy is: robot with largest capacity takes heaviest toys.
            // So we should process from strongest robot.
        }

        // Hmm, let me redo this more carefully.
        // Sort robots descending: X[A-1] >= X[A-2] >= ...
        // Sort toys by weight descending.
        // Robot A-1 (strongest) takes the first 'trips' toys it can handle.
        // Robot A-2 takes the next 'trips' toys, etc.

        fill(assigned.begin(), assigned.end(), false);
        toyPtr = 0;
        for(int r = A - 1; r >= 0; r--){
            int count = 0;
            while(toyPtr < T && count < trips){
                if(toys[toyPtr].first < X[r]){
                    assigned[toyPtr] = true;
                    count++;
                }
                toyPtr++;
            }
        }

        // Collect unassigned toys, sort by size descending
        vector<int> remaining; // indices into toys[]
        for(int i = 0; i < T; i++){
            if(!assigned[i]) remaining.push_back(i);
        }
        sort(remaining.begin(), remaining.end(), [&](int a, int b){
            return toys[a].second > toys[b].second;
        });

        // Assign to small robots
        int remPtr = 0;
        for(int r = B - 1; r >= 0; r--){
            int count = 0;
            while(remPtr < (int)remaining.size() && count < trips){
                if(toys[remaining[remPtr]].second < Y[r]){
                    count++;
                }
                remPtr++;
            }
        }

        return remPtr >= (int)remaining.size();
    };

    int lo = 1, hi = T, ans = T;
    while(lo <= hi){
        int mid = (lo + hi) / 2;
        if(check(mid)){
            ans = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }

    return ans;
}

int main(){
    int A, B, T;
    cin >> A >> B >> T;
    int *X = new int[A], *Y = new int[B], *W = new int[T], *S = new int[T];
    for(int i = 0; i < A; i++) cin >> X[i];
    for(int i = 0; i < B; i++) cin >> Y[i];
    for(int i = 0; i < T; i++) cin >> W[i] >> S[i];
    cout << putaway(A, B, T, X, Y, W, S) << "\n";
    delete[] X; delete[] Y; delete[] W; delete[] S;
    return 0;
}
