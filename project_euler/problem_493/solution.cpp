#include <bits/stdc++.h>
using namespace std;

int main(){
    // E[distinct colors] = 7 * (1 - C(60,20)/C(70,20))
    // C(60,20)/C(70,20) = prod_{k=0}^{9} (50-k)/(70-k)

    // Use exact rational arithmetic with long doubles or fractions
    // For precision, compute with exact fractions using __int128 or just careful doubles

    // Actually, let's use exact rational computation
    // Numerator: 50*49*48*47*46*45*44*43*42*41
    // Denominator: 70*69*68*67*66*65*64*63*62*61

    // These fit in unsigned long long
    // num = 50*49*48*47*46*45*44*43*42*41
    // den = 70*69*68*67*66*65*64*63*62*61

    // For exact decimal, use high-precision approach
    // But we can use long double which has ~18 digits of precision

    long double ratio = 1.0L;
    for(int k = 0; k < 10; k++){
        ratio *= (long double)(50 - k) / (long double)(70 - k);
    }

    long double answer = 7.0L * (1.0L - ratio);

    cout << fixed << setprecision(9) << answer << endl;

    return 0;
}
