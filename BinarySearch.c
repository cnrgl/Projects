#include <stdio.h>
#include <stdlib.h>
//binary search for fibonacci
int main(void) 
{
    int i,n,intv,f=0,l;
    long int *arr=NULL;

    //setting array
    scanf("%d",&n);
    arr=malloc(n*sizeof(int));

    if(arr==NULL){
        printf("array couldnt allocated");
        return 0;
    }

    arr[0]=1;
    arr[1]=1;

    for(i=2;i<n;i++)
    {
        arr[i]=arr[i-1]+arr[i-2];    
        printf("%d-",arr[i]);
    }
//searching...

    intv=n/2;l=n;//interval setting
    printf("\ninput a number=");
    scanf("%d",&i);

    while(!(arr[intv]==i))
    {
        intv=(l+f)/2;
        if(arr[intv]<i)
            f=intv;
        else
            l=intv;
    
        if((intv-i)*(intv-i)<1)
            break;
        printf("%d",intv);
    }
    printf("number is in=%d",intv+1);
    free(arr);
    return 0;
}
