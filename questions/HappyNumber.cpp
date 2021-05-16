//In number theory,a b-happy number is a natural number in a given number base b that eventually 
//reaches 1 when iterated over the perfect digital invariant function for p=2
//Those numbers that do not end in 1 are b-unhappy numbers (or b-sad numbers).
#include <stdio.h>

int main() {

int in,i=0,b1,b2,b3=0;
scanf("%d",&in);

while((i<5)&&(in>1)){
    b1=in%10;
    b2=(in-b1)/10;
    if(b2>9)
    {
        b3=b2/10;
        b2=b2%10;
    }
    in=b1*b1+b2*b2+b3*b3;
    b3=0;
    i++;
    printf("%d",in);
}
if(in==1)
printf("happy");
else
printf("unhappy");

    return 0;
}
