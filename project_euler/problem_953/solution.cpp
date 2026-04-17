#include <bits/stdc++.h>
using namespace std;
int main(){
    // C(16,8) = 12870
    int n=16;
    // All balanced binary inputs of length 16 eventually sort to 00000000 11111111
    // The answer is C(16,8) since all balanced inputs reach sorted state
    long long ans=1;
    for(int i=0;i<8;i++) ans=ans*(16-i)/(i+1);
    cout<<ans<<endl;
    return 0;
}
