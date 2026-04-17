#include <bits/stdc++.h>
using namespace std;
bool is_prime(long long n){
    if(n<2) return false;
    if(n<4) return true;
    if(n%2==0||n%3==0) return false;
    for(long long i=5;i*i<=n;i+=6)
        if(n%i==0||n%(i+2)==0) return false;
    return true;
}
int main(){
    long long best=0;
    for(int k:{1,4,7}){
        vector<int> d;
        for(int i=1;i<=k;i++) d.push_back(i);
        sort(d.begin(),d.end());
        do{
            long long n=0;
            for(int x:d) n=n*10+x;
            if(is_prime(n)) best=max(best,n);
        }while(next_permutation(d.begin(),d.end()));
    }
    cout<<best<<endl;
    return 0;
}
