#include <bits/stdc++.h>
using namespace std;
int main() {
    const int N = 100000;
    vector<int> mu(N+1,0); mu[1]=1;
    vector<bool> is_p(N+1,true); vector<int> primes;
    for(int i=2;i<=N;i++){
        if(is_p[i]){primes.push_back(i);mu[i]=-1;}
        for(int p:primes){
            if((long long)i*p>N)break;
            is_p[i*p]=false;
            if(i%p==0){mu[i*p]=0;break;}
            else mu[i*p]=-mu[i];
        }
    }
    long long M=0, ans=0;
    for(int n=1;n<=N;n++){M+=mu[n]; ans+=abs(M);}
    cout<<ans<<endl;
    return 0;
}
