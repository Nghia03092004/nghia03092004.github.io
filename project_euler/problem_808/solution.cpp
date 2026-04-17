#include <bits/stdc++.h>
using namespace std;

int main(){
    // We need to find primes p such that p^2 is not a palindrome
    // and reverse(p^2) is also a perfect square of a prime.
    // We need the first 50 such reversible prime squares, sorted, and their sum.

    const long long LIMIT = 100000000LL; // sieve up to 10^8
    // p^2 can be up to 10^16, so we need primes up to ~10^8

    // Sieve of Eratosthenes
    vector<bool> is_prime(LIMIT + 1, true);
    is_prime[0] = is_prime[1] = false;
    for(long long i = 2; i * i <= LIMIT; i++){
        if(is_prime[i]){
            for(long long j = i*i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    auto reverseNum = [](long long n) -> long long {
        long long rev = 0;
        while(n > 0){
            rev = rev * 10 + n % 10;
            n /= 10;
        }
        return rev;
    };

    auto isPalindrome = [&](long long n) -> bool {
        return n == reverseNum(n);
    };

    auto isqrt = [](long long n) -> long long {
        long long s = (long long)sqrt((double)n);
        while(s * s > n) s--;
        while((s+1)*(s+1) <= n) s++;
        return s;
    };

    vector<long long> results;

    for(long long p = 2; p <= LIMIT && results.size() < 100; p++){
        if(!is_prime[p]) continue;
        long long sq = p * p;
        if(isPalindrome(sq)) continue;
        long long rev = reverseNum(sq);
        long long sr = isqrt(rev);
        if(sr * sr != rev) continue;
        if(sr <= 1 || sr > LIMIT) continue;
        if(!is_prime[sr]) continue;
        results.push_back(sq);
    }

    sort(results.begin(), results.end());

    if(results.size() >= 50){
        long long sum = 0;
        for(int i = 0; i < 50; i++){
            sum += results[i];
        }
        cout << sum << endl;
    } else {
        cout << "Found only " << results.size() << " reversible prime squares" << endl;
        long long sum = 0;
        for(auto x : results) sum += x;
        cout << "Partial sum: " << sum << endl;
    }

    return 0;
}
