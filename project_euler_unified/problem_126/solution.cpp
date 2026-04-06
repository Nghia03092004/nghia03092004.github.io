#include <bits/stdc++.h>
using namespace std;

int main() {
    const int LIMIT = 20000;
    vector<int> C(LIMIT + 1, 0);

    // f(a,b,c,k) = 2(ab+bc+ac) + 4(a+b+c)(k-1) + 4(k-1)(k-2)
    for (int a = 1; 2LL * a * a <= LIMIT; a++) {
        for (int b = a; 2LL * (a * b + b * b) <= LIMIT; b++) {
            for (int c = b; 2LL * ((long long)a * b + (long long)b * c + (long long)a * c) <= LIMIT; c++) {
                long long base = 2LL * (a * b + b * c + a * c);
                long long edge = 4LL * (a + b + c);
                for (int k = 1; ; k++) {
                    long long val = base + edge * (k - 1) + 4LL * (k - 1) * (k - 2);
                    if (val > LIMIT) break;
                    C[(int)val]++;
                }
            }
        }
    }

    for (int n = 1; n <= LIMIT; n++) {
        if (C[n] == 1000) {
            cout << n << endl;
            return 0;
        }
    }

    return 0;
}
