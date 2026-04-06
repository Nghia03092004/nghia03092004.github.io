#include <bits/stdc++.h>
using namespace std;

int main(){
    long long LIMIT = 100000000000000LL; // 10^14
    vector<long long> results;

    // x(x+1) * y(y+1) <= LIMIT, with x <= y
    // x(x+1) <= sqrt(LIMIT) ~ 10^7
    for(long long x = 1; x*(x+1)*(x)*(x+1) <= LIMIT; x++){
        long long px = x * (x + 1);
        for(long long y = x; ; y++){
            long long py = y * (y + 1);
            long long val = px * py;
            if(val > LIMIT) break;
            results.push_back(val);
        }
    }

    sort(results.begin(), results.end());
    long long count = unique(results.begin(), results.end()) - results.begin();

    cout << count << endl;
    return 0;
}
