#include <bits/stdc++.h>
using namespace std;
int main(){
    long long N=1000000000000LL;
    int sqN=(int)sqrt((double)N);
    while((long long)(sqN+1)*(sqN+1)<=N) sqN++;
    vector<int> mu(sqN+1,0);
    mu[1]=1;
    vector<bool> is_p(sqN+1,true);
    vector<int> primes;
    for(int i=2;i<=sqN;i++){
        if(is_p[i]){primes.push_back(i);mu[i]=-1;}
        for(int p:primes){
            if((long long)i*p>sqN) break;
            is_p[i*p]=false;
            if(i%p==0){mu[i*p]=0;break;}
            else mu[i*p]=-mu[i];
        }
    }
    long long ans=0;
    for(int k=1;k<=sqN;k++) if(mu[k]) ans+=mu[k]*(N/((long long)k*k));
    cout<<ans<<endl;
    return 0;
}
