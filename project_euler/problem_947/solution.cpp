#include <bits/stdc++.h>
using namespace std;
int main(){
    int moves[]={1,2,4};
    int G[200]={};
    for(int n=1;n<200;n++){
        set<int> reach;
        for(int m:moves) if(n>=m) reach.insert(G[n-m]);
        int mex=0; while(reach.count(mex)) mex++;
        G[n]=mex;
    }
    // Find period
    int per=0;
    for(int p=1;p<100;p++){
        bool ok=true;
        for(int i=10;i<60;i++) if(G[i]!=G[i+p]){ok=false;break;}
        if(ok){per=p;break;}
    }
    long long N=1000000000LL;
    int p_in_per=0;
    // Count P-positions in one period starting from index that's in steady state
    for(int i=0;i<per;i++) if(G[10+i]==0) p_in_per++;
    // Count for n=1..N
    long long total_p=0;
    // Align: n=1..N
    // Compute directly for first few, then use period
    int offset=10; // steady state starts around here
    // Direct count for n=1..offset-1
    for(int i=1;i<min((long long)offset,N+1);i++) if(G[i]==0) total_p++;
    if(N>=offset){
        long long remaining=N-offset+1;
        total_p+=(remaining/per)*p_in_per;
        long long rem=remaining%per;
        for(int i=0;i<rem;i++) if(G[offset+i]==0) total_p++;
    }
    cout<<N-total_p<<endl;
    return 0;
}
