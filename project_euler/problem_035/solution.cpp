#include <bits/stdc++.h>
using namespace std;

int main() {
    const int N = 1000000;
    vector<bool> is_prime(N, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i < N; i++)
        if (is_prime[i])
            for (int j = i * i; j < N; j += i)
                is_prime[j] = false;

    int count = 0;
    for (int p = 2; p < N; p++) {
        if (!is_prime[p]) continue;
        int k = 0, tmp = p;
        while (tmp > 0) { k++; tmp /= 10; }
        int pw = 1;
        for (int i = 0; i < k - 1; i++) pw *= 10;
        bool circular = true;
        int r = p;
        for (int i = 0; i < k; i++) {
            if (r < 0 || r >= N || !is_prime[r]) { circular = false; break; }
            r = (r % 10) * pw + r / 10;
        }
        if (circular) count++;
    }
    cout << count << endl;  // 55
    return 0;
}
