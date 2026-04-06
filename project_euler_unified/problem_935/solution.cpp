#include <bits/stdc++.h>
using namespace std;
int main(){
    const int M=50;
    const long long MOD=1e9+7;
    auto isqs=[](int n){int s=sqrt(n);return s*s==n||(s+1)*(s+1)==n&&(s+1)*(s+1)==n;};
    auto ispsq=[](int n)->bool{int s=(int)sqrt((double)n);while(s*s<n)s++;return s*s==n;};
    set<pair<int,int>> blk;
    for(int x=0;x<=M;x++) for(int y=0;y<=M;y++)
        if(ispsq(x+y)&&__gcd(x,y)>1) blk.insert({x,y});
    vector<vector<long long>> dp(M+1,vector<long long>(M+1,0));
    dp[0][0]=blk.count({0,0})?0:1;
    for(int x=0;x<=M;x++) for(int y=0;y<=M;y++){
        if(x==0&&y==0) continue;
        if(blk.count({x,y})){dp[x][y]=0;continue;}
        long long v=0;
        if(x>0) v+=dp[x-1][y];
        if(y>0) v+=dp[x][y-1];
        dp[x][y]=v%MOD;
    }
    cout<<dp[M][M]<<endl;
    return 0;
}
