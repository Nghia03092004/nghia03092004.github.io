#include <bits/stdc++.h>
using namespace std;
int main(){
    const int N=10000;
    const long long MOD=1e9+7;
    long long D=0;
    for(int r=1;r<=N;r++){
        long long cnt=0;
        long long r2=(long long)r*r;
        for(int x=-r;x<=r;x++){
            long long ymax=(long long)sqrt((double)(r2-(long long)x*x));
            while(ymax*ymax+(long long)x*x>r2) ymax--;
            cnt+=2*ymax+1;
        }
        D=(D+cnt)%MOD;
    }
    cout<<D<<endl;
    return 0;
}
