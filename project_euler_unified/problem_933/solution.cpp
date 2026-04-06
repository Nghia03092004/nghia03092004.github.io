#include <bits/stdc++.h>
using namespace std;
int FACT[]={1,1,2,6,24,120,720,5040,40320,362880};
int dfs(int n){int s=0;while(n){s+=FACT[n%10];n/=10;}return s;}
int main(){
    const int N=1000000;
    vector<int> L(N+1,0);
    for(int n=1;n<=N;n++){
        unordered_set<int> seen;
        int cur=n;
        while(!seen.count(cur)){seen.insert(cur);cur=dfs(cur);}
        L[n]=seen.size();
    }
    long long ans=0;
    for(int i=1;i<=N;i++) ans+=L[i];
    cout<<ans<<endl;
    return 0;
}
