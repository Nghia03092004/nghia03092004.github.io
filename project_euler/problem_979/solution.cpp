#include <bits/stdc++.h>
using namespace std;
int main() {
    long long total = 0;
    for (int n = 1; n <= 10000; n++) {
        int j = 0;
        for (int i = 2; i <= n; i++) j = (j + 3) % i;
        total += j + 1;
    }
    cout << total << endl;
    return 0;
}
