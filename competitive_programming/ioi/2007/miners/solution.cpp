#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    string s;
    cin >> s;

    // Encode food types: M=0, F=1, G=2, none=3
    auto encode = [](char c) -> int {
        if(c == 'M') return 0;
        if(c == 'F') return 1;
        return 2; // 'G'
    };

    // State: (a1, a2, b1, b2) where a1,a2 are last two for mine 1,
    //        b1,b2 for mine 2. Each in {0,1,2,3}.
    // Pack into single int: a1*64 + a2*16 + b1*4 + b2
    // Total: 4^4 = 256 states

    auto pack = [](int a1, int a2, int b1, int b2) -> int {
        return a1 * 64 + a2 * 16 + b1 * 4 + b2;
    };

    auto bonus = [](int c, int x, int y) -> int {
        // Distinct types among c, x, y (ignoring 3 = none)
        set<int> st;
        st.insert(c);
        if(x != 3) st.insert(x);
        if(y != 3) st.insert(y);
        return st.size();
    };

    vector<int> dp(256, -1);
    dp[pack(3, 3, 3, 3)] = 0; // initial state: no deliveries

    for(int i = 0; i < N; i++){
        int c = encode(s[i]);
        vector<int> ndp(256, -1);

        for(int state = 0; state < 256; state++){
            if(dp[state] == -1) continue;

            int a1 = (state / 64) % 4;
            int a2 = (state / 16) % 4;
            int b1 = (state / 4) % 4;
            int b2 = state % 4;

            // Option 1: send cart to mine 1
            int b1_val = bonus(c, a1, a2);
            int ns1 = pack(c, a1, b1, b2);
            ndp[ns1] = max(ndp[ns1], dp[state] + b1_val);

            // Option 2: send cart to mine 2
            int b2_val = bonus(c, b1, b2);
            int ns2 = pack(a1, a2, c, b1);
            ndp[ns2] = max(ndp[ns2], dp[state] + b2_val);
        }

        dp = ndp;
    }

    int ans = *max_element(dp.begin(), dp.end());
    cout << ans << "\n";

    return 0;
}
