#include <bits/stdc++.h>
using namespace std;
int main(){
    const int N=1000000;
    const long long MOD=1e9+7;
    vector<bool> is_p(N+1,true); is_p[0]=is_p[1]=false;
    for(int i=2;i*i<=N;i++) if(is_p[i]) for(int j=i*i;j<=N;j+=i) is_p[j]=false;
    vector<int> mu(N+1,0); mu[1]=1;
    vector<bool> ip2(N+1,true); vector<int> primes;
    for(int i=2;i<=N;i++){
        if(ip2[i]){primes.push_back(i);mu[i]=-1;}
        for(int p:primes){
            if((long long)i*p>N) break;
            ip2[i*p]=false;
            if(i%p==0){mu[i*p]=0;break;}
            else mu[i*p]=-mu[i];
        }
    }
    long long S=0;
    for(int p=2;p<=N;p++) if(is_p[p]) S=(S+mu[p-1]+MOD)%MOD;
    cout<<S<<endl;
    return 0;
}
