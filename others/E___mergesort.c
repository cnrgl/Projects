#include <stdio.h>

int *merge(int *left,int *right,int len1,int len2)
{
	int *result,i=0;
	result=(int*)malloc((len1+len2)*sizeof(int));
	int x=0,y=0;	
	
	while((len1>x)&&(len2>y))
	{
		if(left[x]<=right[y])
			{
				result[i]=left[x];
				x++;
			}
		else 
			{
				result[i]=right[y];
				y++;
			}
			i++;
	}
	if(len1>x)
		for(;x<len1;x++)
			result[i++]=right[x];
	else if(len2>y)
		for(;y<len2;y++)
			result[i++]=left[y];
				
	return result;
}
int *mergesort(int *arr,int len)
{
	int *left,*right,*result,i,mid=len/2;
	
	left=(int*)malloc(mid*sizeof(int));
	right=(int*)malloc(mid*sizeof(int));
	
	if(len<=1)
	{
		return arr[0];
	}
	for(i=0;i<mid;i++)
	{
		left[i]=arr[i];
		right[i]=arr[mid+i];
	}
	left=mergesort(left,mid);
	right=mergesort(right,mid);
	result=merge(left,right,mid,(len-mid));
	return result;
}
int main(void)
{
	
	
	
	
	
	return 0;
}
