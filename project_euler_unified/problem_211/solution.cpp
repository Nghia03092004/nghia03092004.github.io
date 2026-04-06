#include <bits/stdc++.h>
using namespace std;

int main() {
    const int N = 64000000;
    vector<unsigned long long> sigma2(N, 0);

    for (long long d = 1; d < N; d++) {
        long long d2 = d * d;
        for (long long k = d; k < N; k += d)
            sigma2[k] += d2;
    }

    long long answer = 0;
    for (int n = 1; n < N; n++) {
        unsigned long long s = sigma2[n];
        unsigned long long r = (unsigned long long)sqrt((double)s);
        while (r * r > s) r--;
        while ((r + 1) * (r + 1) <= s) r++;
        if (r * r == s)
            answer += n;
    }

    cout << answer << endl;
    return 0;
}
