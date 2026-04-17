#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 280: Ant and Seeds
 *
 * 5x5 grid. Seeds in top row (y=0). Ant starts at center (2,2).
 * Random walk to adjacent cells. Pick up seed when entering a top-row cell
 * with a seed and not carrying. Deposit when entering a bottom-row cell (y=4)
 * that does NOT already have a seed, while carrying.
 *
 * Goal: expected steps to deposit one seed in each bottom-row cell.
 *
 * State: (position, top_mask, bottom_mask, carrying)
 * Conservation: popcount(top) + popcount(bottom) + carrying = 5
 * Terminal: top=0, bottom=31, carrying=0
 *
 * Solved via value iteration on the absorbing Markov chain.
 */

int dx[] = {0,0,1,-1};
int dy[] = {1,-1,0,0};

int main(){
    const int NPOS = 25;
    const int NMASK = 32;
    const int NSTATES = NPOS * NMASK * NMASK * 2;

    auto idx = [](int pos, int top, int bot, int carry) -> int {
        return ((pos * NMASK + top) * NMASK + bot) * 2 + carry;
    };
    auto valid = [](int top, int bot, int carry) -> bool {
        return __builtin_popcount(top) + __builtin_popcount(bot) + carry == 5;
    };

    vector<vector<int>> neighbors(25);
    for (int y = 0; y < 5; y++)
        for (int x = 0; x < 5; x++) {
            int p = y*5+x;
            for (int d = 0; d < 4; d++) {
                int nx = x+dx[d], ny = y+dy[d];
                if (nx>=0 && nx<5 && ny>=0 && ny<5)
                    neighbors[p].push_back(ny*5+nx);
            }
        }

    vector<double> E(NSTATES, 0.0), Enew(NSTATES, 0.0);
    int start = idx(12, 31, 0, 0);

    for (int iter = 0; iter < 100000; iter++) {
        double maxdiff = 0;
        for (int pos = 0; pos < NPOS; pos++)
            for (int top = 0; top < NMASK; top++)
                for (int bot = 0; bot < NMASK; bot++)
                    for (int carry = 0; carry < 2; carry++) {
                        if (!valid(top, bot, carry)) continue;
                        if (top == 0 && bot == 31 && carry == 0) {
                            Enew[idx(pos,top,bot,carry)] = 0;
                            continue;
                        }
                        double val = 1.0;
                        int nn = neighbors[pos].size();
                        for (int np : neighbors[pos]) {
                            int nx = np%5, ny = np/5;
                            int nt = top, nb = bot, nc = carry;
                            if (ny == 0 && nc == 0 && (nt & (1<<nx))) {
                                nc = 1; nt &= ~(1<<nx);
                            } else if (ny == 4 && nc == 1 && !(nb & (1<<nx))) {
                                nc = 0; nb |= (1<<nx);
                            }
                            val += E[idx(np,nt,nb,nc)] / nn;
                        }
                        int i = idx(pos,top,bot,carry);
                        Enew[i] = val;
                        maxdiff = max(maxdiff, abs(val - E[i]));
                    }
        swap(E, Enew);
        if (maxdiff < 1e-10) break;
    }

    printf("%.6f\n", E[start]);
    return 0;
}
