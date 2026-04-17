#include <bits/stdc++.h>
using namespace std;
int main(){
    const int N=100000;
    vector<bool> is_p(N+1,true);
    is_p[0]=is_p[1]=false;
    for(int i=2;i*i<=N;i++) if(is_p[i]) for(int j=i*i;j<=N;j+=i) is_p[j]=false;
    auto factorize=[](int n){
        set<int> f;
        for(int d=2;d*d<=n;d++) while(n%d==0){f.insert(d);n/=d;}
        if(n>1) f.insert(n);
        return f;
    };
    long long S=0;
    for(int p=2;p<=N;p++){
        if(!is_p[p]) continue;
        if(p==2){S+=1;continue;}
        auto facs=factorize(p-1);
        for(int g=2;g<p;g++){
            bool ok=true;
            for(int q:facs){
                long long r=1,base=g,exp=(p-1)/q;
                long long b=base%p;
                long long e=exp;
                r=1;
                long long bb=b;
                while(e>0){if(e&1) r=r*bb%p; bb=bb*bb%p; e>>=1;}
                if(r==1){ok=false;break;}
            }
            if(ok){S+=g;break;}
        }
    }
    cout<<S<<endl;
    return 0;
}
