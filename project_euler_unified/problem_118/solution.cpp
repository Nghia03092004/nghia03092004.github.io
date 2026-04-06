#include <bits/stdc++.h>
using namespace std;

// Miller-Rabin primality test for numbers up to ~10^9
bool is_prime(long long n) {
    if (n < 2) return false;
    if (n < 4) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (long long i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

int digits[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
// For each bitmask of digits, store all primes formable from those digits
map<int, vector<long long>> mask_primes;

void gen_perms(int mask, vector<int>& ds, int idx, long long num, vector<long long>& primes) {
    if (idx == (int)ds.size()) {
        if (is_prime(num)) primes.push_back(num);
        return;
    }
    for (int i = idx; i < (int)ds.size(); i++) {
        swap(ds[i], ds[idx]);
        gen_perms(mask, ds, idx + 1, num * 10 + ds[idx], primes);
        swap(ds[i], ds[idx]);
    }
}

void precompute() {
    // Enumerate all non-empty subsets of {1..9} (bitmask over 9 bits)
    for (int mask = 1; mask < 512; mask++) {
        vector<int> ds;
        int dsum = 0;
        for (int i = 0; i < 9; i++) {
            if (mask & (1 << i)) {
                ds.push_back(i + 1);
                dsum += (i + 1);
            }
        }
        // Pruning: if digit sum divisible by 3 and subset has >1 digit or the single digit is not 3
        // Actually, just compute all primes and filter
        vector<long long> primes;
        gen_perms(mask, ds, 0, 0, primes);
        // Remove duplicates and sort
        sort(primes.begin(), primes.end());
        primes.erase(unique(primes.begin(), primes.end()), primes.end());
        if (!primes.empty()) {
            mask_primes[mask] = primes;
        }
    }
}

int answer = 0;

void solve(int remaining_mask, long long last_prime) {
    if (remaining_mask == 0) {
        answer++;
        return;
    }
    // Enumerate all non-empty subsets of remaining_mask
    for (int sub = remaining_mask; sub > 0; sub = (sub - 1) & remaining_mask) {
        auto it = mask_primes.find(sub);
        if (it == mask_primes.end()) continue;
        for (long long p : it->second) {
            if (p > last_prime) {
                solve(remaining_mask ^ sub, p);
            }
        }
    }
}

int main() {
    precompute();
    solve(511, 0); // 511 = (1<<9)-1, all 9 digits available
    cout << answer << endl;
    return 0;
}
