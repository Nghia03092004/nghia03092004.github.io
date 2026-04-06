#include <bits/stdc++.h>
using namespace std;

int main(){
    // H(n) for n>=3 follows OEIS A011779
    // Linear recurrence with signature (3,-3,3,-6,6,-3,3,-3,1)
    // Initial values: H(3)=1, H(4)=3, H(5)=6, H(6)=12, H(7)=21, H(8)=33,
    //                 H(9)=51, H(10)=75, H(11)=105

    const int N = 12345;
    vector<long long> H(N + 1, 0);
    // a(k) = H(k+3), we store directly as H[n]
    H[3] = 1; H[4] = 3; H[5] = 6; H[6] = 12; H[7] = 21;
    H[8] = 33; H[9] = 51; H[10] = 75; H[11] = 105;

    for(int n = 12; n <= N; n++){
        H[n] = 3*H[n-1] - 3*H[n-2] + 3*H[n-3] - 6*H[n-4]
             + 6*H[n-5] - 3*H[n-6] + 3*H[n-7] - 3*H[n-8] + H[n-9];
    }

    long long total = 0;
    for(int n = 3; n <= N; n++){
        total += H[n];
    }

    cout << total << endl;
    return 0;
}
