#include <iostream>
using namespace std;

int main() {
 int n,j=0;
 int limit;
 double x_n,x=1;
 //newtonian iterative method
 //cube root
 cin>>n;
 cin>>limit;
 for(j=0;j<limit;j++)
 {
     x_n=x-((x*x*x)-n)/(3*(x*x));
     x=x_n;
 }
 cout<<x;
    return 0;
}
