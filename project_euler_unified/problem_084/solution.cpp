#include <bits/stdc++.h>
using namespace std;

int main(){
    // Problem 84: Monopoly Odds
    // Two 4-sided dice. Find the 3 most visited squares via Monte Carlo.
    // Answer: 101524

    const int GO = 0, JAIL = 10, G2J = 30;
    const int CC[] = {2, 17, 33};
    const int CH[] = {7, 22, 36};
    const int R[] = {5, 15, 25, 35};
    const int U[] = {12, 28};

    auto isCC = [&](int s){ return s==2||s==17||s==33; };
    auto isCH = [&](int s){ return s==7||s==22||s==36; };

    auto nextR = [&](int s) -> int {
        for(int r : R) if(s < r) return r;
        return R[0];
    };
    auto nextU = [&](int s) -> int {
        for(int u : U) if(s < u) return u;
        return U[0];
    };

    mt19937 rng(42);
    int SIDES = 4;
    long long visits[40] = {};
    int pos = 0;
    int doubles_count = 0;
    int N = 10000000;

    for(int t = 0; t < N; t++){
        int d1 = rng() % SIDES + 1;
        int d2 = rng() % SIDES + 1;
        bool is_double = (d1 == d2);

        if(is_double){
            doubles_count++;
        } else {
            doubles_count = 0;
        }

        if(doubles_count == 3){
            pos = JAIL;
            doubles_count = 0;
            visits[pos]++;
            continue;
        }

        pos = (pos + d1 + d2) % 40;

        if(pos == G2J){
            pos = JAIL;
        }
        else if(isCC(pos)){
            int card = rng() % 16;
            if(card == 0) pos = GO;
            else if(card == 1) pos = JAIL;
        }
        else if(isCH(pos)){
            int card = rng() % 16;
            switch(card){
                case 0: pos = GO; break;
                case 1: pos = JAIL; break;
                case 2: pos = 11; break;
                case 3: pos = 24; break;
                case 4: pos = 39; break;
                case 5: pos = 5; break;
                case 6: pos = nextR(pos); break;
                case 7: pos = nextR(pos); break;
                case 8: pos = nextU(pos); break;
                case 9:
                    pos = (pos - 3 + 40) % 40;
                    if(isCC(pos)){
                        int cc_card = rng() % 16;
                        if(cc_card == 0) pos = GO;
                        else if(cc_card == 1) pos = JAIL;
                    }
                    break;
                default: break;
            }
        }

        visits[pos]++;
    }

    vector<pair<long long,int>> freq;
    for(int i = 0; i < 40; i++)
        freq.push_back({visits[i], i});
    sort(freq.rbegin(), freq.rend());

    string ans;
    for(int k = 0; k < 3; k++){
        int sq = freq[k].second;
        if(sq < 10) ans += "0";
        ans += to_string(sq);
    }
    cout << ans << endl;

    return 0;
}
