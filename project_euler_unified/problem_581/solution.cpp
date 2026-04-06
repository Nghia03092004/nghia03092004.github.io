#include <bits/stdc++.h>
using namespace std;

int main(){
    // Find sum of all n such that T(n)=n(n+1)/2 is 47-smooth
    // Equivalently, find consecutive 47-smooth numbers (n, n+1) and sum all n

    const vector<int> primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};
    const long long LIMIT = 1200000000000LL; // Upper bound from Stormer's theorem

    // Generate all 47-smooth numbers up to LIMIT using a min-heap
    priority_queue<long long, vector<long long>, greater<long long>> pq;
    set<long long> visited;
    pq.push(1);
    visited.insert(1);

    long long prev = -1;
    long long total = 0;

    while(!pq.empty()){
        long long v = pq.top();
        pq.pop();

        if(v > LIMIT) break;

        // Check if v and prev are consecutive
        if(v == prev + 1 && prev >= 1){
            total += prev; // n = prev, n+1 = v
        }

        prev = v;

        for(int p : primes){
            if(v <= LIMIT / p){ // overflow check
                long long nv = v * p;
                if(visited.find(nv) == visited.end()){
                    visited.insert(nv);
                    pq.push(nv);
                }
            }
        }
    }

    cout << total << endl;
    return 0;
}
