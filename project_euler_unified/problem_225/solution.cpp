#include <bits/stdc++.h>
using namespace std;

int main(){
    // Find the 124th odd number that does not divide any Tribonacci number.
    // T(1) = T(2) = T(3) = 1, T(n) = T(n-1) + T(n-2) + T(n-3).
    // For each odd m, compute T(n) mod m until 0 is found or cycle detected.

    int target = 124;
    int count = 0;

    for(int m = 1; ; m += 2){
        // Check if m divides any Tribonacci number
        int a = 1 % m, b = 1 % m, c = 1 % m;
        bool divides = false;

        // Check if m = 1 (divides everything)
        if(m == 1){
            // T(1) = 1, and 1 divides 1. So 1 divides a tribonacci number.
            continue;
        }

        // Iterate through the cycle
        for(long long iter = 0; iter < (long long)m * m * m + 10; iter++){
            int next = (a + b + c) % m;
            a = b;
            b = c;
            c = next;
            if(c == 0){
                divides = true;
                break;
            }
            // Check if we've returned to (1, 1, 1)
            if(a == 1 % m && b == 1 % m && c == 1 % m){
                break;
            }
        }

        if(!divides){
            count++;
            if(count == target){
                cout << m << endl;
                return 0;
            }
        }
    }

    return 0;
}
