#include <bits/stdc++.h>
using namespace std;
int main(){
    const int N=100000;
    vector<bool> sieve(N+1,true);
    sieve[0]=sieve[1]=false;
    for(int i=2;i*i<=N;i++) if(sieve[i]) for(int j=i*i;j<=N;j+=i) sieve[j]=false;
    long long total=0;
    for(int n=4;n<=N;n+=2){
        int cnt=0;
        for(int p=2;p<=n/2;p++) if(sieve[p]&&sieve[n-p]) cnt++;
        total+=cnt;
    }
    cout<<total<<endl;
    return 0;
}
