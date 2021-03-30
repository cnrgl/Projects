#include <stdio.h>
#include <time.h>

int main(void)
{
	int i,j,k,n=0,*array,best=0,sum;
	time_t start,end;
	
	printf("give number of array length:");
	scanf("%d",&n);
	
	array=(int*)malloc(n*sizeof(int));
	
	printf("give numbers:\n");
	for(i=0;i<n;i++)
		scanf("%d",&array[i]);

	///brutal force
	time(&start);
	for(i=0;i<n;i++)
		for(j=i;j<n;j++)
		{
			sum=0;
			for(k=i;k<j;k++)
				sum+=array[k];
			if(sum>best)
				best=sum;	
	    }
	time(&end);    
	   printf("with brutal force(%.4lf sec)\n",difftime(start,end)); 
	//kadane
	sum=array[0];
	best=array[0];
	
	time(&start);
	for(i=1;i<n;i++)
	{
		if((array[i]+sum)>array[i])//if number which exist in left of array[i] is decrease,then will start new subarray 
			sum+=array[i];
		else if((sum+array[i])<array[i])	
			sum=array[i];
		if(sum>best)
			best=sum;
	}
	time(&end);
	
	printf("with kadane (%.4lf sec)\n",difftime(start,end));
	printf("best subarray sum is=%d",best);
	
	return 0;
}
