#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 316: Numbers in Decimal Expansions
 *
 * g(n) = W(s) - d + 1 where W(s) = sum of 10^k for each k where the
 * length-k prefix of s equals the length-k suffix, and d = number of digits.
 *
 * Compute sum of g(floor(10^16 / n)) for n = 2 to 999999.
 */

int main(){
    // Use unsigned long long; 10^16 fits in ull (max ~1.8 * 10^19)
    const unsigned long long P = 10000000000000000ULL; // 10^16

    long long total = 0;

    for(int n = 2; n <= 999999; n++){
        unsigned long long m = P / n;

        // Convert m to string
        char buf[20];
        int d = sprintf(buf, "%llu", m);

        // Compute W(s) = sum of 10^k for matching prefix/suffix
        long long W = 0;
        for(int k = 1; k <= d; k++){
            // Check if prefix of length k equals suffix of length k
            // prefix: buf[0..k-1], suffix: buf[d-k..d-1]
            bool match = true;
            for(int i = 0; i < k; i++){
                if(buf[i] != buf[d - k + i]){
                    match = false;
                    break;
                }
            }
            if(match){
                long long pw = 1;
                for(int j = 0; j < k; j++) pw *= 10;
                W += pw;
            }
        }

        total += W - d + 1;
    }

    cout << total << endl;
    return 0;
}
