#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1234567891LL;

long long power(long long base, long long exp, long long mod){
    long long result = 1;
    base %= mod;
    while(exp > 0){
        if(exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

int main(){
    // List starts as [2, 3, ..., n]. Each round: replace smallest by its square.
    // After many rounds, each element a becomes a^(2^t) for some t.
    // Key insight: element with value v, after being squared t times, becomes v^(2^t).
    // The smallest element gets squared. With n-1 elements [2..n], after enough rounds,
    // each element cycles through being the smallest.
    //
    // Initially: values are 2, 3, ..., n. Represented as (base, exponent) pairs.
    // Element i has base b_i and current value b_i^(2^e_i).
    // Comparison: b_i^(2^e_i) vs b_j^(2^e_j) => 2^e_i * log(b_i) vs 2^e_j * log(b_j)
    //
    // We track (log_value, base, exponent) in a priority queue.
    // After enough iterations, the pattern stabilizes: elements cycle in order of log(base).

    const int N = 10000;
    const long long M = 10000000000000000LL; // 10^16

    // Use a priority queue with (log_value, index)
    // Each element: base b, exponent e, value = b^(2^e)
    // log_value = 2^e * log(b)

    int n_elem = N - 1; // elements 2, 3, ..., N

    // Store as (base, exponent)
    vector<int> base_val(n_elem);
    vector<long long> exponent(n_elem);
    vector<long double> log_val(n_elem);

    for(int i = 0; i < n_elem; i++){
        base_val[i] = i + 2;
        exponent[i] = 0;
        log_val[i] = logl((long double)(i + 2));
    }

    // Priority queue: min-heap by log_value
    // (log_value, index)
    priority_queue<pair<long double, int>, vector<pair<long double, int>>, greater<>> pq;
    for(int i = 0; i < n_elem; i++){
        pq.push({log_val[i], i});
    }

    // Simulate until a cycle is detected
    // After some rounds, the order stabilizes. Once every element has been squared
    // at least once, the cycle length is n_elem (each element is squared once per cycle).
    //
    // Key insight: after initial transient, elements are squared in a fixed cyclic order.
    // Sorted by log(base)/2^e, once all exponents are large enough, the order is determined
    // by log(base) alone (since 2^e dominates).
    //
    // Actually, once the smallest element (after squaring) becomes larger than the
    // second smallest, the order is fixed. The elements get squared in cyclic order:
    // the one with smallest current log-value.
    //
    // After the transient, in each "super-round" of n_elem steps, each element is squared
    // exactly once. So after the transient, if we've done T rounds, and have R remaining,
    // each element's exponent increases by floor(R/n_elem) more full cycles plus some extras.

    // Simulate until we detect the cyclic pattern
    long long rounds_done = 0;

    // Detect cycle: track which element is smallest for consecutive rounds
    vector<int> order_history;
    bool cycle_found = false;
    int cycle_start = 0;
    int cycle_len = 0;

    // Simulate up to a reasonable number of rounds
    long long max_sim = min(M, (long long)n_elem * 200);

    for(long long r = 0; r < max_sim; r++){
        auto [lv, idx] = pq.top();
        pq.pop();

        order_history.push_back(idx);

        // Square this element
        exponent[idx]++;
        log_val[idx] *= 2.0L;

        pq.push({log_val[idx], idx});
        rounds_done++;

        // Check for cycle: look for repeating pattern of length n_elem
        if((int)order_history.size() >= 2 * n_elem){
            int sz = order_history.size();
            bool is_cycle = true;
            for(int j = 0; j < n_elem; j++){
                if(order_history[sz - 1 - j] != order_history[sz - 1 - j - n_elem]){
                    is_cycle = false;
                    break;
                }
            }
            if(is_cycle){
                cycle_found = true;
                cycle_len = n_elem;
                cycle_start = sz - 2 * n_elem;
                break;
            }
        }
    }

    if(cycle_found){
        // We've done rounds_done rounds. Remaining: M - rounds_done
        long long remaining = M - rounds_done;

        // In each cycle of cycle_len, each element in the cycle order gets squared once
        long long full_cycles = remaining / cycle_len;
        long long extra = remaining % cycle_len;

        // Each element gets full_cycles more squarings
        for(int i = 0; i < n_elem; i++){
            exponent[i] += full_cycles;
        }

        // Apply extra rounds
        // The cycle order (last cycle_len elements of order_history)
        vector<int> cycle_order(order_history.end() - cycle_len, order_history.end());
        for(long long r = 0; r < extra; r++){
            int idx = cycle_order[r % cycle_len];
            exponent[idx]++;
        }
    }
    // else: we've simulated all M rounds directly (M was small enough)

    // Compute answer: sum of base_val[i]^(2^exponent[i]) mod MOD
    // base_val[i]^(2^exponent[i]) mod MOD
    // Since MOD is prime, by Fermat's little theorem: a^(MOD-1) = 1 mod MOD
    // So a^(2^e) mod MOD = a^(2^e mod (MOD-1)) mod MOD

    long long ans = 0;
    for(int i = 0; i < n_elem; i++){
        // Compute 2^exponent[i] mod (MOD-1)
        long long exp_mod = power(2, exponent[i], MOD - 1);
        long long val = power((long long)base_val[i], exp_mod, MOD);
        ans = (ans + val) % MOD;
    }

    cout << ans << endl;

    return 0;
}
