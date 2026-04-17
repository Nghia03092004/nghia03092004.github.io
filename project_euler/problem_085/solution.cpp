#include <bits/stdc++.h>
using namespace std;

int main(){
    // Problem 85: Counting Rectangles
    // R(m,n) = C(m+1,2) * C(n+1,2) = m(m+1)/2 * n(n+1)/2
    // Find area m*n such that R(m,n) is closest to 2,000,000.
    // Answer: 2772

    long long target = 2000000;
    long long best_diff = LLONG_MAX;
    int best_area = 0;

    for(int m = 1; m * (m + 1) / 2 <= target; m++){
        long long tm = (long long)m * (m + 1) / 2;
        double val = 2.0 * target / tm;
        int n = (int)((-1.0 + sqrt(1.0 + 4.0 * val)) / 2.0);

        for(int nn = max(1, n - 1); nn <= n + 2; nn++){
            long long rects = tm * (long long)nn * (nn + 1) / 2;
            long long diff = abs(rects - target);
            if(diff < best_diff){
                best_diff = diff;
                best_area = m * nn;
            }
        }
    }

    cout << best_area << endl;

    return 0;
}
