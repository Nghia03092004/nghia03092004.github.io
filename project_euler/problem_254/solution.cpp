#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll FACT[] = {1,1,2,6,24,120,720,5040,40320,362880};
const int B = 362880;
const int M = B;

int digit_sum_int(ll x) {
    int s = 0;
    while (x > 0) { s += x % 10; x /= 10; }
    return s;
}

const int NW = (M + 63) / 64;
const int PD = 17;

uint64_t can[PD+1][9*PD+1][NW];
int pow10m[20];

void cyclic_shift_or(uint64_t* dst, const uint64_t* src, int k) {
    if (k == 0) {
        for (int w = 0; w < NW; w++) dst[w] |= src[w];
        return;
    }
    for (int w = 0; w < NW; w++) {
        if (!src[w]) continue;
        for (int b = 0; b < 64; b++) {
            int p = w * 64 + b;
            if (p >= M) break;
            if (!(src[w] & (1ULL << b))) continue;
            int q = (p + k) % M;
            dst[q / 64] |= 1ULL << (q % 64);
        }
    }
}

bool any_set(const uint64_t* bs) {
    for (int w = 0; w < NW; w++) if (bs[w]) return true;
    return false;
}

bool is_feasible(int d, int s, int r) {
    if (s < 0 || d < 0 || s > 9 * d) return false;
    if (d > PD) return true;
    return (can[d][s][r / 64] >> (r % 64)) & 1;
}

ll find_min_x(int D, int target_sum, int target_res) {
    if (target_sum < 1 || target_sum > 9 * D || D <= 0 || D > 18) return -1;
    int rs = target_sum, rr = target_res;
    ll x = 0;
    for (int pos = 0; pos < D; pos++) {
        int rd = D - pos - 1;
        int lo = (pos == 0) ? 1 : 0;
        bool found = false;
        for (int c = lo; c <= 9; c++) {
            if (c > rs) break;
            int ns = rs - c;
            if (ns > 9 * rd || ns < 0) continue;
            int p = pow10m[D - 1 - pos];
            int nr = ((ll)rr - (ll)c * p % M + M) % M;
            if (rd == 0) {
                if (ns == 0 && nr == 0) {
                    x = x * 10 + c;
                    return x;
                }
                continue;
            }
            if (is_feasible(rd, ns, nr)) {
                x = x * 10 + c;
                rs = ns;
                rr = nr;
                found = true;
                break;
            }
        }
        if (!found) return -1;
    }
    return x;
}

// For each residue, store ALL bases (up to min_base_len, among those: all possible dsums and digit tuples)
// This is too complex. Instead, let me store PER RESIDUE:
// - minimum base_len
// - For that minimum base_len: ALL possible (digit_tuple, dsum) pairs
// Then when comparing, use the digit tuple directly.

// Actually, let me simplify: store per residue the minimum base_len and 
// the lex-smallest digit tuple of that length.
// The lex-smallest tuple with a given residue and base_len:
// this requires a more careful BFS.

// APPROACH: BFS layer by layer. At each layer L:
// For newly discovered residues: find the lex-smallest digit tuple.
// For ties (same residue, same layer): lex-smallest tuple wins.
// 
// The digit tuple is non-decreasing. To find lex-smallest:
// maximize 1s, then 2s, etc.
// For a given residue r at level L:
// the lex-smallest tuple is the one with the most 1s, then most 2s, etc.
// This is hard to track in BFS because we build tuples one digit at a time.
// 
// Alternative: for each residue at level L, track the lex-smallest tuple.
// A tuple of length L with non-decreasing digits from 1-8.
// We can represent this as the vector itself.
// But with 362880 residues and tuples up to length 36: memory is huge.
//
// PRACTICAL COMPROMISE: For each residue, store:
// - min_base_len
// - for that length: lex-smallest tuple (as a vector)
// During BFS, when we extend a tuple by adding digit d >= last_digit:
// new_tuple = old_tuple + [d]. Check if it's lex-smaller than current best for the residue.
//
// This requires careful implementation. Let me try a different approach:
// multi-level BFS where we track (residue, last_digit) at each level.

struct State {
    int residue;
    int last_digit; // last digit used (for non-decreasing)
    int dsum;
    vector<int> digits; // the actual digits
};

int main() {
    pow10m[0] = 1;
    for (int i = 1; i < 20; i++) pow10m[i] = (ll)pow10m[i-1] * 10 % M;
    
    memset(can, 0, sizeof(can));
    can[0][0][0] = 1;
    
    fprintf(stderr, "Precomputing feasibility...\n");
    for (int d = 1; d <= PD; d++) {
        int p = pow10m[d-1];
        for (int s = 0; s <= 9*d; s++) {
            for (int c = 0; c <= min(9, s); c++) {
                int sp = s - c;
                if (sp > 9*(d-1)) continue;
                if (!any_set(can[d-1][sp])) continue;
                int sh = (ll)c * p % M;
                cyclic_shift_or(can[d][s], can[d-1][sp], sh);
            }
        }
        fprintf(stderr, "  d=%d\n", d);
    }
    
    // For each residue: find the base with minimum base_len, 
    // then among those: the one whose (digits, k) gives lex-smallest n.
    // Since k = (x - R) / B, and x depends only on residue, k is determined.
    // n = digits followed by k nines.
    // For same residue and same base_len: all have same k.
    // So lex-smallest n = lex-smallest (digits, 9...9) = lex-smallest digits
    // (since the 9s are the same suffix).
    // Lex-smallest non-decreasing digits of given length L and given sum mod B = r:
    // This is: most 1s as possible, then 2s, etc.
    
    // BFS with (residue, last_digit) states.
    // min_len[r] = minimum number of digits to achieve residue r.
    // For that min_len: best_digits[r] = lex-smallest digit tuple.
    
    // State: (residue, last_digit). 
    // Since digits are non-decreasing and we add one at a time:
    // last_digit restricts which digits we can add next.
    // We track: for each (residue, last_digit): minimum length to reach here.
    // Among same length: lex-smallest prefix.
    
    // Memory: B * 8 states = 2.9M. Manageable.
    
    // dist[r][d] = min length to achieve residue r with last digit d.
    // Then min_base_len[r] = min over d of dist[r][d].
    
    vector<vector<int>> dist(B, vector<int>(9, -1)); // d = 1..8, index d-1
    // Also track dsum and digits
    vector<vector<int>> best_dsum(B, vector<int>(9, INT_MAX));
    vector<vector<vector<int>>> best_digits_arr(B, vector<vector<int>>(9));
    
    // Initial: length 0, residue 0, last_digit 0 (can use any digit 1-8)
    // First step: choose digit d (1-8), residue = d! % B, last_digit = d.
    for (int d = 1; d <= 8; d++) {
        int r = (int)(FACT[d] % B);
        if (dist[r][d-1] == -1) {
            dist[r][d-1] = 1;
            best_dsum[r][d-1] = d;
            best_digits_arr[r][d-1] = {d};
        } else if (dist[r][d-1] == 1) {
            if (d < best_dsum[r][d-1]) {
                best_dsum[r][d-1] = d;
                best_digits_arr[r][d-1] = {d};
            }
        }
    }
    
    vector<pair<int,int>> cur_states; // (residue, last_digit_idx)
    for (int d = 1; d <= 8; d++) {
        int r = (int)(FACT[d] % B);
        cur_states.push_back({r, d-1});
    }
    // Deduplicate
    sort(cur_states.begin(), cur_states.end());
    cur_states.erase(unique(cur_states.begin(), cur_states.end()), cur_states.end());
    
    int level = 1;
    int total_reached = 0;
    
    // Count reached residues
    vector<int> min_len_r(B, -1);
    for (int d = 1; d <= 8; d++) {
        int r = (int)(FACT[d] % B);
        if (min_len_r[r] == -1) min_len_r[r] = 1;
    }
    for (int r = 0; r < B; r++) if (min_len_r[r] >= 0) total_reached++;
    fprintf(stderr, "Level 1: %d states, %d residues reached\n", (int)cur_states.size(), total_reached);
    
    while (!cur_states.empty() && total_reached < B) {
        level++;
        vector<pair<int,int>> next_states;
        
        for (auto& [r, di] : cur_states) {
            int last_d = di + 1; // digit value
            for (int d = last_d; d <= 8; d++) {
                int nr = (r + (int)(FACT[d] % B)) % B;
                int new_dsum = best_dsum[r][di] + d;
                
                if (dist[nr][d-1] == -1) {
                    dist[nr][d-1] = level;
                    best_dsum[nr][d-1] = new_dsum;
                    best_digits_arr[nr][d-1] = best_digits_arr[r][di];
                    best_digits_arr[nr][d-1].push_back(d);
                    next_states.push_back({nr, d-1});
                    if (min_len_r[nr] == -1) {
                        min_len_r[nr] = level;
                        total_reached++;
                    }
                } else if (dist[nr][d-1] == level) {
                    // Same level: compare lex (the digit tuples)
                    vector<int> new_digits = best_digits_arr[r][di];
                    new_digits.push_back(d);
                    if (new_digits < best_digits_arr[nr][d-1]) {
                        best_digits_arr[nr][d-1] = new_digits;
                        best_dsum[nr][d-1] = new_dsum;
                    }
                }
            }
        }
        
        sort(next_states.begin(), next_states.end());
        next_states.erase(unique(next_states.begin(), next_states.end()), next_states.end());
        cur_states = next_states;
        
        fprintf(stderr, "Level %d: %d states, %d/%d residues\n", level, (int)cur_states.size(), total_reached, B);
        if (level > 50) break;
    }
    
    // For each residue: find the best (last_digit, dsum, digits)
    // Best = minimum len, then lex-smallest digits tuple
    struct BaseInfo {
        int len;
        int dsum;
        vector<int> digits;
        ll R;
    };
    
    vector<BaseInfo> base_info(B);
    for (int r = 0; r < B; r++) {
        base_info[r].len = -1;
        for (int di = 0; di < 8; di++) {
            if (dist[r][di] < 0) continue;
            if (base_info[r].len < 0 || dist[r][di] < base_info[r].len ||
                (dist[r][di] == base_info[r].len && best_digits_arr[r][di] < base_info[r].digits)) {
                base_info[r].len = dist[r][di];
                base_info[r].dsum = best_dsum[r][di];
                base_info[r].digits = best_digits_arr[r][di];
                ll R = 0;
                for (int d : base_info[r].digits) R += FACT[d];
                base_info[r].R = R;
            }
        }
    }
    // Residue 0: empty base
    if (base_info[0].len < 0 || 0 < base_info[0].len) {
        base_info[0].len = 0;
        base_info[0].dsum = 0;
        base_info[0].digits.clear();
        base_info[0].R = 0;
    }
    
    // Now solve
    ll total_sg = 0;
    
    for (int i = 1; i <= 150; i++) {
        int target_mod9 = i % 9;
        
        ll best_total = LLONG_MAX;
        vector<int> best_n_prefix; // for lex comparison
        ll best_sg = 0;
        
        for (int D = max(1, (i + 8) / 9); D <= 18; D++) {
            for (int r = 0; r < B; r++) {
                if (base_info[r].len < 0) continue;
                if (r % 9 != target_mod9) continue;
                
                ll x = find_min_x(D, i, r);
                if (x < 0) continue;
                
                ll R = base_info[r].R;
                if (x < R) continue;
                if ((x - R) % B != 0) continue;
                
                ll k = (x - R) / B;
                ll total_digits = (ll)base_info[r].len + k;
                ll sg = (ll)base_info[r].dsum + 9LL * k;
                
                if (total_digits < best_total) {
                    best_total = total_digits;
                    best_sg = sg;
                    best_n_prefix = base_info[r].digits;
                } else if (total_digits == best_total) {
                    // Compare n lexicographically
                    // n1 = best_n_prefix + (best_total - best_n_prefix.size()) nines
                    // n2 = base_info[r].digits + k nines
                    // Both have same total_digits.
                    // Compare digit by digit:
                    const auto& d2 = base_info[r].digits;
                    bool n2_better = false;
                    int L = total_digits;
                    for (int p = 0; p < min((ll)40, (ll)L); p++) {
                        int v1 = (p < (int)best_n_prefix.size()) ? best_n_prefix[p] : 9;
                        int v2 = (p < (int)d2.size()) ? d2[p] : 9;
                        if (v1 != v2) {
                            n2_better = (v2 < v1);
                            break;
                        }
                    }
                    if (n2_better) {
                        best_sg = sg;
                        best_n_prefix = d2;
                    }
                }
            }
            
            // Don't break: larger D with different residue might give smaller total_digits
            // because a base with fewer digits might only work at larger D.
            // At D+1, it might find a solution with much smaller base.
            //
            // So we CANNOT break after the first D. We must check all D values.
            // But the maximum D is 18, so it's bounded.
        }
        
        total_sg += best_sg;
        if (i <= 50 || i >= 140)
            fprintf(stderr, "i=%d: total=%lld, sg=%lld\n", i, best_total, best_sg);
    }
    
    printf("%lld\n", total_sg);
    return 0;
}
