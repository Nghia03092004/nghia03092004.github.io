#include <bits/stdc++.h>
using namespace std;
long long euler_phi(long long n){
    long long r=n,t=n;
    for(long long p=2;p*p<=t;p++){
        if(t%p==0){while(t%p==0)t/=p;r-=r/p;}
    }
    if(t>1) r-=r/t;
    return r;
}
long long power(long long a,long long b,long long m){
    long long r=1; a%=m;
    while(b>0){if(b&1) r=r*a%m; a=a*a%m; b>>=1;}
    return r;
}
long long tower(long long a,int n,long long m){
    if(m==1) return 0;
    if(n==1) return a%m;
    long long phi=euler_phi(m);
    long long exp=tower(a,n-1,phi);
    return power(a,exp,m);
}
int main(){
    cout<<tower(2,100,1000000007LL)<<endl;
}
