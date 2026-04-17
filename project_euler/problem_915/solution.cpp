#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 915: Digit Power Sum Sequences
 *
 * f(n) = sum of 5th powers of digits of n.
 * Sum all n <= 10^6 whose orbit eventually reaches 1.
 *
 * Key observations:
 *   - f(n) < n for n >= 10^7, so orbits contract into [1, 413343].
 *   - Memoize orbit classification: reaches 1 or not.
 *   - Armstrong numbers for p=5: 1, 4150, 4151, 54748, 92727, 93084, 194979.
 *   - 1 is the only fixed point that qualifies.
 *
 * Complexity: O(N) amortized time with O(N) space.
 */

static const int N = 1000000;
static const int CACHE_SIZE = 500000;
static const int POW5[] = {0, 1, 32, 243, 1024, 3125, 7776, 16807, 32768, 59049};

int digit_pow5(int n) {
    int s = 0;
    while (n > 0) {
        s += POW5[n % 10];
        n /= 10;
    }
    return s;
}

// 0 = unknown, 1 = reaches 1, 2 = does not reach 1
int cache[CACHE_SIZE];

bool reaches_one(int n) {
    if (n < CACHE_SIZE && cache[n]) return cache[n] == 1;

    vector<int> path;
    unordered_set<int> visited;
    int x = n;

    while (visited.find(x) == visited.end() && (x >= CACHE_SIZE || !cache[x])) {
        visited.insert(x);
        path.push_back(x);
        x = digit_pow5(x);
    }

    bool result;
    if (x < CACHE_SIZE && cache[x]) {
        result = (cache[x] == 1);
    } else if (x == 1) {
        result = true;
    } else {
        // Found a cycle; check if 1 is in it
        result = false;
        int y = x;
        do {
            if (y == 1) { result = true; break; }
            y = digit_pow5(y);
        } while (y != x);
    }

    for (int v : path) {
        if (v < CACHE_SIZE) {
            cache[v] = result ? 1 : 2;
        }
    }
    return result;
}

int main() {
    memset(cache, 0, sizeof(cache));
    cache[1] = 1; // 1 is a fixed point: f(1) = 1

    long long total = 0;
    for (int n = 1; n <= N; n++) {
        if (reaches_one(n)) {
            total += n;
        }
    }

    cout << total << endl;

    // Verification: Armstrong numbers for p=5
    // These are fixed points of f: n = f(n)
    vector<int> armstrong = {1, 4150, 4151, 54748, 92727, 93084, 194979};
    for (int a : armstrong) {
        assert(digit_pow5(a) == a);
    }

    return 0;
}
