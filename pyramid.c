#include <stdio.h>
#include <stdlib.h>
int is_prime(int n)
{
    for(int i=n/2;i>=2;i--)
    {
        if(n%i==0)
            return 0;
    }
    return 1;
}
int main(void) {
  
int *mat=NULL,i,j,n,nrow,tmp;

FILE *fil=fopen("pyramid.txt","r");
if(fil==NULL)
{
	printf("file couldnt open!!!");
	return -1;
}


	//file reading
	//read line by line
	while(fscanf(fil,"%d",&tmp)==1)
	{
			nrow=(++n)*(n-1)/2;
			mat=(int*)realloc(mat,(nrow+n)*sizeof(int));
			mat[nrow]=tmp;
			for(i=1;i<n;i++)
				fscanf(fil,"%d",&mat[i+nrow]);
	}
	
	
	for(i=n;i>1;i--)//search rows
	{
		nrow=i*(i-1)/2;
		for(j=0;j<i-1;j++)
		{
			if(is_prime(mat[nrow-i+j+1]))//check destination member is prime or not
				break;
			if(mat[nrow+j]>mat[nrow+j+1])//choosing one path
				mat[nrow-i+j+1]+=mat[nrow+j];
			else if(!is_prime(mat[nrow+j+1]))
				mat[nrow-i+j+1]+=mat[nrow+j+1];
		}
	}
	
	//final operation -> check first two route
	if((mat[1]>mat[2])&&(!is_prime(mat[1])))
		mat[0]+=mat[1];
	else if((mat[1]<mat[2])&&(!is_prime(mat[2])))			
		mat[0]+=mat[2];
	else
	{
		printf("path closed with primes\n");
		return 0;
	}
		
  printf("max sum is=%d",mat[0]);

	fclose(fil);
		
return 0;	
	//	find_path(mat,2,0);//finding path/version1 has some errors
}


//version1 part
//int find_path(int mat[],int row,int col)
//{
//    nrow=(row*(row-1)/2);
// 
//	if(row>n)
//    {
//        printf("-> %d\n",sumn);
//        if(sumn>sum)
//           sum=sumn;
//           sumn=0;
//        return 1;
//    }
//    
//        printf("\nbellek%d\n",&mat[col+nrow]);
//   		 
//    	printf(" (%d,%d)",row,col);    
//	    if(!is_prime(mat[col+nrow]))
//        {
//            sumn+=mat[col+nrow];
//            
//			printf("val=%d \n",mat[col+nrow]);//...
//            find_path(mat,row+1,col);
//        }
//        printf("\nbellek%d %d\n",&mat[col+nrow+1],mat[col+nrow+1]);
//        if(!is_prime(mat[col+nrow+1]))//second option
//    	{
//    		sumn+=mat[col+nrow+1];
//         	printf("val2=%d \n",mat[col+nrow+1]);//...
//            find_path(mat,row+1,col);
//		}
//		return 1;
