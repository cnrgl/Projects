#include <iostream>
#include <math.h>
using namespace std;

bool prime(long int num)
{    
bool test=true;
    for(int i=1;i<sqrt(num);i+=2){
    if(num%i==0)
    {test=false;break;}}
    test=(num%2==0)&&(num=!2)?false:true;
    return test;
}
void mersenn(long int in)
{
    if(prime(in)==true){
    if(!(fmod(log(in+1)/log(2),2)==0))
    cout<<"it is not a mersenne prime";
    else
    cout<<in<<" is a mersenne prime";}
    else
    cout<<"it is not a prime";
}
void mersenn(int s1,int s2)
{
   for(int i=s1;i<=s2;i++)
   {
   cout<<"mersenne numbers in given range=";
       if(prime(i)==true)
       {
           if(fmod((log(i+1)/log(2)),1)==0){
        cout<<i<<",";
         
        }
       }
       else
       cout<<"none";
   }
}
int main(void) {
    long int input1;
    int input2=0;
    float mersenne;
    cin>>input1>>input2;
    if(input2==0){
        mersenn(input1);
      }
    else
    {
        mersenn(input1,input2);
    }
    return 0;
}
