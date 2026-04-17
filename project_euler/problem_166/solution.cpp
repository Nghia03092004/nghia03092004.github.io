#include <bits/stdc++.h>
using namespace std;

int main(){
    // Problem 166: Count 4x4 grids of digits 0-9 where all rows, columns,
    // and both diagonals have the same sum S.
    //
    // Grid:
    // a b c d
    // e f g h
    // i j k l
    // m n o p
    //
    // Given rows 1-3, row 4 is determined by column constraints.
    // We check validity and diagonal constraints.
    //
    // Optimization: for each S, enumerate pairs (row1, row2) as "top half"
    // and store in a map keyed by partial column sums and partial diagonal info.
    // Then enumerate (row3) and derive row4, check constraints.
    //
    // Actually, a direct enumeration with pruning is fast enough.

    long long total = 0;

    for(int S = 0; S <= 36; S++){
        // Generate all 4-tuples of digits summing to S
        vector<array<int,4>> tuples;
        for(int x0 = 0; x0 <= 9 && x0 <= S; x0++)
            for(int x1 = 0; x1 <= 9 && x0+x1 <= S; x1++)
                for(int x2 = 0; x2 <= 9 && x0+x1+x2 <= S; x2++){
                    int x3 = S - x0 - x1 - x2;
                    if(x3 >= 0 && x3 <= 9)
                        tuples.push_back({x0,x1,x2,x3});
                }

        int NT = tuples.size();

        // For meet-in-the-middle: enumerate top 2 rows, store by
        // (col0_partial, col1_partial, col2_partial, col3_partial, diag0_partial, diag1_partial)
        // where diag0 = a+f, diag1 = d+g
        // Key: (c0, c1, c2, c3, d0, d1) where each in [0..18]
        // Use a flat map: key = c0*19^5 + c1*19^4 + c2*19^3 + c3*19^2 + d0*19 + d1

        unordered_map<long long, int> topmap;
        topmap.reserve(NT * NT / 2);

        for(int r1 = 0; r1 < NT; r1++){
            auto &t1 = tuples[r1];
            for(int r2 = 0; r2 < NT; r2++){
                auto &t2 = tuples[r2];
                int c0 = t1[0]+t2[0], c1 = t1[1]+t2[1];
                int c2 = t1[2]+t2[2], c3 = t1[3]+t2[3];
                int d0 = t1[0]+t2[1]; // a + f
                int d1 = t1[3]+t2[2]; // d + g
                long long key = (((((long long)c0*19+c1)*19+c2)*19+c3)*19+d0)*19+d1;
                topmap[key]++;
            }
        }

        // Enumerate bottom 2 rows (row3=i,j,k,l and row4=m,n,o,p)
        // Column constraints: c0+i+m=S => i+m = S-c0, etc.
        // Diag constraints: diag0: k+p, need a+f+k+p=S => k+p = S-d0
        //                   diag1: j+m, need d+g+j+m=S => j+m = S-d1
        // So we need:
        //   bottom_c0 = i+m = S - top_c0
        //   bottom_c1 = j+n = S - top_c1
        //   bottom_c2 = k+o = S - top_c2
        //   bottom_c3 = l+p = S - top_c3
        //   bottom_d0 = k+p = S - top_d0
        //   bottom_d1 = j+m = S - top_d1

        for(int r3 = 0; r3 < NT; r3++){
            auto &t3 = tuples[r3];
            for(int r4 = 0; r4 < NT; r4++){
                auto &t4 = tuples[r4];
                int bc0 = t3[0]+t4[0], bc1 = t3[1]+t4[1];
                int bc2 = t3[2]+t4[2], bc3 = t3[3]+t4[3];
                int bd0 = t3[2]+t4[3]; // k + p
                int bd1 = t3[1]+t4[0]; // j + m

                // We need top values: tc0 = S-bc0, etc.
                int tc0 = S-bc0, tc1 = S-bc1, tc2 = S-bc2, tc3 = S-bc3;
                int td0 = S-bd0, td1 = S-bd1;
                if(tc0<0||tc1<0||tc2<0||tc3<0||td0<0||td1<0) continue;
                if(tc0>18||tc1>18||tc2>18||tc3>18||td0>18||td1>18) continue;

                long long key = (((((long long)tc0*19+tc1)*19+tc2)*19+tc3)*19+td0)*19+td1;
                auto it = topmap.find(key);
                if(it != topmap.end())
                    total += it->second;
            }
        }
    }

    cout << total << endl;
    return 0;
}
