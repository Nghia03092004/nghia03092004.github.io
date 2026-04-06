#include <bits/stdc++.h>
using namespace std;

int main() {
    auto S = [](long long m, long long N) -> long long {
        long long p = (N - 1) / m;
        return m * p * (p + 1) / 2;
    };
    cout << S(3, 1000) + S(5, 1000) - S(15, 1000) << endl;
    return 0;
}
