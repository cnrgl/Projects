#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main()
{
    vector <int>r;
    vector <int>l;
    int *arr,i,sum=0,len;
    cin>>len;

    arr=new int[len];
    for(i=0;i<len;i++)
    {
        cin>>arr[i];
        sum+=arr[i];
    }

    cout<<sum<<endl;
    sum/=2;
    sort(arr,arr+len);
    reverse(arr,arr+len);//reverse sorted array
    //starting sort
    r.push_back(arr[0]);
    sum-=arr[0];

    for(i=1;i<len;i++)
    {
        if(arr[i]<=sum){
            r.push_back(arr[i]);
            sum-=arr[i];}
    else
        l.push_back(arr[i]);
    }

    //printing arrays
    cout<<"[ ";
    for(i=0;i<r.size();i++)
        cout<<r[i]<<" ";
    cout<<" ]-[ ";
    for(i=0;i<l.size();i++)
        cout<<l[i]<<" ";
    cout<<"]";
    return 0;
}
